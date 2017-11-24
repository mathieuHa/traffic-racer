#include <stdio.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "menu.h"
#include "SDL_Interface.h"
#include "define.h"

int getBestScore () {
	FILE *fic = NULL;
	fic = fopen ("score.txt","r");
	if (fic == NULL) {printf("erreur ouverture de fichier 'score.txt'\n"); return -1;}
	int best = 0;
	if (fscanf(fic,"%d", &best)!=1) {
		printf("erreur de lecture\n");
		fclose (fic);
		return -1;
	}
	else return best;
}

void affiche_score () {
	FILE *fic = NULL;
	fic = fopen ("score.txt","r");
	if (fic == NULL) {printf("erreur ouverture de fichier 'score.txt'\n"); exit(-1);}
	int temp[5];
	int cpt = 0;
	while (cpt < 10){
		if (fscanf(fic,"%d %d %d %d %d", &temp[0], &temp[1], &temp[2], &temp[3], &temp[4])!=5) {
			printf("erreur de lecture\n"); 
			fclose (fic); 
			exit (-1);
		}
		cpt++;
	}
	fclose(fic);
}


void add_score (int score, int time, char nom[100]) {
	FILE *fic = NULL;
	FILE *fic_temp = NULL;
	fic = fopen ("score.txt","r");
	fic_temp = fopen ("score.temp","w+");
	if (fic == NULL) {printf("erreur ouverture de fichier 'score.txt'\n"); exit(-1);}
	if (fic_temp == NULL) {printf("erreur ouverture de fichier 'score.temp'\n"); exit(-1);}
	int t_score;
	int t_time;
	char t_nom[100];
	int trouver = 0;
	int i;
	for (i=0; i<10; i++) {
		if (fscanf(fic,"%d %d %s", &t_score, &t_time, t_nom)!=3) {
			printf("erreur de lecture\n"); 
			fclose (fic); 
			exit (-1);
		}
		if (t_score <= score && trouver == 0){
			fprintf(fic_temp, "%d %d %s\n",score, time, nom);
			trouver = 1;
			// ajouter le score ici
		}		
		fprintf(fic_temp, "%d %d %s\n",t_score, t_time, t_nom);
	}
	fclose (fic);
	fclose (fic_temp);
	fic = NULL;
	fic_temp = NULL;

	fic = fopen ("score.txt","w+");
	fic_temp = fopen ("score.temp","r");
	if (fic == NULL) {printf("erreur ouverture de fichier 'score.txt'\n"); exit(-1);}
	if (fic_temp == NULL) {printf("erreur ouverture de fichier 'score.temp'\n"); exit(-1);}

	for (i=0; i<10; i++) {
		if (fscanf(fic_temp,"%d %d %s", &t_score, &t_time, t_nom)!=3) {
			printf("erreur de lecture\n"); 
			fclose (fic_temp); 
			exit (-1);
		}
		fprintf(fic, "%d %d %s\n",t_score, t_time, t_nom);
	}
	fclose(fic);
	fclose(fic_temp);
}

void afficher_score(SDL_Renderer* renderer, char *appState)
{
    SDL_Event event;
    TTF_Font *police = NULL;
    int i;
    int t_score;
	int t_time;
	char t_nom[100];

	FILE *fic = NULL;
	fic = fopen ("score.txt","r");
	if (fic == NULL) {printf("erreur ouverture de fichier 'score.txt'\n"); exit(-1);}

    SDL_SetRenderDrawColor(renderer, 0,  0 , 0, 0);

    police = TTF_OpenFont("comic.otf", 40); // chargement de la police comic dans police
    SDL_RenderClear(renderer);

    char str[100];
    affiche_texte_Blanc (renderer, police, "Nom", 40, 20);
    affiche_texte_Blanc (renderer, police, "Score", 300, 20);
    affiche_texte_Blanc (renderer, police, "Temps", 580, 20);

    for (i=1; i<=10; i++) {
		if (fscanf(fic,"%d %d %s", &t_score, &t_time, t_nom)!=3) {
			printf("erreur de lecture\n"); 
			fclose (fic); 
			exit (-1);
		}

		affiche_texte_Blanc (renderer, police, t_nom, 40, 20+i*MAX_Y/12);
 	    sprintf(str, "%8d", t_score);    
    	affiche_texte_Blanc (renderer, police, str, 270, 20+i*MAX_Y/12);
 	    sprintf(str, "%8d", t_time);    
    	affiche_texte_Blanc (renderer, police, str, 570, 20+i*MAX_Y/12);
    }

    SDL_RenderPresent(renderer);
    while (*appState==4){
    while (SDL_PollEvent(&event))
    {
        
        switch(event.type)
        {
            case SDL_QUIT:  
                *appState = 1;
                break;
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym)
                {
                    case SDLK_ESCAPE: // veut quitter les scores (solution de secours)
		                *appState = 1;
                        break;
                    default:
                        break;
                } // switch
                    default:
                        break;
                break;
        } // switch

	}
	}

    TTF_CloseFont(police); // on efface la police
}
