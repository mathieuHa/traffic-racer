#include <stdio.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "menu.h"
#include "SDL_Interface.h"
#include "define.h"

static SDL_Texture* t_etatA = NULL;
static SDL_Texture* t_etatB = NULL;
static SDL_Texture* t_logo  = NULL;
static int touche;
static SDL_Rect aut;
static SDL_Rect man;
static SDL_Rect sco;
static SDL_Rect qui;
static SDL_Rect mouse;

// fonction main de l'état de jeu
void mainMenu(SDL_Renderer* renderer, char* appState, TTF_Font *police)
{
	charger_tout_menu (renderer);
	touche = 1;

    while(*appState==1)
    {
        renderMenu(renderer, police);
        eventsHandlerMenu(appState);
        updateMenu();
    }
    clean_menu();
}

void eventsHandlerMenu(char* appState)
{
    SDL_Event event;
    SDL_WaitEvent(&event);

    // Detecte si on clique sur la croix
    if( event.type == SDL_QUIT )
    {
        //On quitte le programme
        *appState = 0;
    }
    // Detecte si une touche est appuyee
    else if( event.type == SDL_KEYDOWN )
    {
        if ( event.key.keysym.sym == SDLK_ESCAPE )
        {
            *appState = 0;
        }
        else if (event.key.keysym.sym == SDLK_DOWN){
            touche++;
            if (touche==5) touche=1;
        }
        else if (event.key.keysym.sym == SDLK_UP){
            touche--;
            if (touche==0) touche=4;

        }
        else if (event.key.keysym.sym == SDLK_RETURN){
            (*appState) = (touche+1)%5;                
        }
    }
    else if ( event.type == SDL_MOUSEMOTION ) {
        mouse.x = event.button.x;
        mouse.y = event.button.y;
        if (collision(mouse,aut)) touche = 1;
        if (collision(mouse,man)) touche = 2;
        if (collision(mouse,sco)) touche = 3;
        if (collision(mouse,qui)) touche = 4;
        
    }

    else if ( event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT) {
        mouse.x = event.button.x;
        mouse.y = event.button.y;
        if (collision(mouse,aut)) (*appState) = 2;
        if (collision(mouse,man)) (*appState) = 3;
        if (collision(mouse,sco)) (*appState) = 4;
        if (collision(mouse,qui)) (*appState) = 0;
        
    }
}

void updateMenu()
{
	
}

void renderMenu(SDL_Renderer* renderer, TTF_Font *police)
{
	SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);

    SDL_Rect rect_logo = {0, 50, MAX_X, 130};
    SDL_RenderCopy (renderer, t_logo, NULL, &rect_logo);

	affiche_texte_centre (renderer, police, 1, "Automatique");
	affiche_texte_centre (renderer, police, 2, "Manuel");
	affiche_texte_centre (renderer, police, 3, "Score");
    affiche_texte_centre (renderer, police, 4, "Quitter");

    SDL_RenderPresent(renderer);
}

void charger_tout_menu (SDL_Renderer* renderer) {
    charger_Texture (renderer, &t_etatA, "etatA.PNG");
    charger_Texture (renderer, &t_etatB, "etatB.PNG");
    charger_Texture (renderer, &t_logo, "logo.png");
    int hauteur = 70;
    int longueur = 3*MAX_X/5;
    aut.x = MAX_X/2-longueur/2;     aut.y = 2*MAX_Y/6-hauteur/2;        aut.w = longueur;       aut.h = hauteur;
    man.x = MAX_X/2-longueur/2;     man.y = 3*MAX_Y/6-hauteur/2;      man.w = longueur;       man.h = hauteur;
    sco.x = MAX_X/2-longueur/2;     sco.y = 4*MAX_Y/6-hauteur/2;      sco.w = longueur;       sco.h = hauteur;
    qui.x = MAX_X/2-longueur/2;     qui.y = 5*MAX_Y/6-hauteur/2;      qui.w = longueur;       qui.h = hauteur;
    mouse.x = 0;                    mouse.y = 0;                      mouse.w = 1;            mouse.h = 1;
}

void affiche_texte_centre (SDL_Renderer* renderer, TTF_Font *police, int etage, char str[100]) {
	int hauteur = 70;
    int longueur = 3*MAX_X/5;
	SDL_Rect rect = {MAX_X/2-longueur/2,(etage+1)*MAX_Y/6-hauteur/2,longueur, hauteur};
    switch (etage) {
        case 1:
            if (touche==etage)    SDL_RenderCopy (renderer, t_etatA, NULL, &rect);
            else            SDL_RenderCopy (renderer, t_etatB, NULL, &rect);
        break;

        case 2:
         if (touche==etage)    SDL_RenderCopy (renderer, t_etatA, NULL, &rect);
            else            SDL_RenderCopy (renderer, t_etatB, NULL, &rect);
        break;

        case 3:
         if (touche==etage)    SDL_RenderCopy (renderer, t_etatA, NULL, &rect);
            else            SDL_RenderCopy (renderer, t_etatB, NULL, &rect);
        break;

        case 4:
         if (touche==etage)    SDL_RenderCopy (renderer, t_etatA, NULL, &rect);
            else            SDL_RenderCopy (renderer, t_etatB, NULL, &rect);
        break;
    }
    affiche_texte_menu (renderer, police, str, rect.x+rect.w/2, rect.y+rect.h/2);
}

int collision (SDL_Rect a, SDL_Rect b) {
    if ((a.x >= b.x + b.w)      // trop à droite
    || (a.x + a.w <= b.x) // trop à gauche
    || (a.y >= b.y + b.h) // trop en bas
    || (a.y + a.h <= b.y))  // trop en haut
          return 0; 
   else
          return 1; 
}

void clean_menu () {
    if (t_etatB!=NULL) SDL_DestroyTexture(t_etatB);
    if (t_etatA!=NULL) SDL_DestroyTexture(t_etatA);
    if (t_logo!=NULL) SDL_DestroyTexture(t_logo);
}