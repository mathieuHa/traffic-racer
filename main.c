#include <stdio.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include "define.h"
#include "menu.h"
#include "voiture.h"
#include "decors.h"
#include "game.h"
#include "score.h"

int main(int argc, char* argv[])
{

    SDL_Window* mainWindow = NULL; // Ecran principal
    char appRunning = 1; // 0=QUITTER, 1=MENU, 2=JEUAUTO 3=JEUMANUEL, 4=SCORE

    // init SDL
    if(SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        fprintf(stderr,"Erreur lors de l'initialisation de la SDL\n");
        return -1;
    }
    if (TTF_Init () == -1)
    {
        fprintf(stderr,"Erreur lors de l'initialisation de SDL_TTF : %s\n", TTF_GetError());
        return -1;
    }
    TTF_Font *police_jeu = TTF_OpenFont("comic.otf", 20);
    TTF_Font *police_menu = TTF_OpenFont("comic.otf", 24);

    // init fenetre principale
    mainWindow = SDL_CreateWindow("Projet C",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,MAX_X,MAX_Y,SDL_WINDOW_SHOWN);

    if(!mainWindow)
    {
        fprintf(stderr,"Erreur de création de la fenêtre : %s\n",SDL_GetError());
        SDL_Quit();
        return -3;
    }
    // init renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(mainWindow,-1,SDL_RENDERER_ACCELERATED);

    if(!renderer)
    {
        fprintf(stderr,"Erreur de création du renderer : %s\n",SDL_GetError());
        SDL_DestroyWindow(mainWindow);
        SDL_Quit();
        return -3;
    }

    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) == -1) //Initialisation de l'API Mixer
    {
        printf("%s", Mix_GetError());
    }

    Mix_Music *musique = NULL;
    musique = Mix_LoadMUS("musique.mp3");
    if (musique == NULL) printf("erreur chargement musique\n");
    Mix_PlayMusic(musique, -1); //Jouer infiniment la musique
    Mix_AllocateChannels(10);
    Mix_Volume(1, MIX_MAX_VOLUME/2);

    while(appRunning)
    {
        if(appRunning == 1) {

            mainMenu(renderer, &appRunning, police_menu);
        }
        else if(appRunning == 2 || appRunning == 3) {

            mainGame(renderer, &appRunning, police_jeu);
        }
        else if (appRunning == 4) {
            afficher_score(renderer,&appRunning);
        }
    }

    Mix_FreeMusic(musique);
    Mix_CloseAudio();
    TTF_CloseFont(police_jeu);
    TTF_CloseFont(police_menu);
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(mainWindow);
    SDL_Quit();

    return 0;
}
