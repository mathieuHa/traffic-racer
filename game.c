#include <stdio.h>
#include <math.h>
#include <time.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

#include "SDL_Interface.h"
#include "voiture.h"
#include "define.h"
#include "score.h"
#include "decors.h"
#include "game.h"


static SDL_Texture* t_v_joueur = NULL;
static SDL_Texture* t_voiture = NULL;
static SDL_Texture* t_voiture_blue = NULL;
static SDL_Texture* t_voiture_red = NULL;
static SDL_Texture* t_voiture_pink = NULL;
static SDL_Texture* t_voiture_brown = NULL;
static SDL_Texture* t_voiture_green = NULL;
static SDL_Texture* t_voiture_yellow = NULL;
static SDL_Texture* t_route = NULL;
static SDL_Texture* t_bg_hud = NULL;
static SDL_Texture* t_decors1 = NULL;
static SDL_Texture* t_decors2 = NULL;
static SDL_Texture* t_decors3 = NULL;
static SDL_Texture* t_decors4 = NULL;
static SDL_Texture* t_decors5 = NULL;
static SDL_Texture* t_decors6 = NULL;
static SDL_Texture* t_explosion = NULL;
static SDL_Texture* t_coeurR = NULL;
static SDL_Texture* t_coeurD = NULL;
static Mix_Chunk *son_klaxon = NULL;
static Mix_Chunk *son_crash = NULL;
static int keyR;
static int keyL; // gérer la répétition sinon délay
static int keyU;
static int keyD;
static int keyS;
static int DY;
static int time_B;
static int time_D;
static int time_A;
static int time_C;
static int points;
static int best_score;
static int duree_game;
static int collisions;
static int time_pop;
static int vie_joueur;


// fonction main de l'état de jeu
void mainGame(SDL_Renderer* renderer, char* appState, TTF_Font *police)
{

    Voiture *v_joueur = malloc (sizeof(Voiture));
    Voiture *l_voiture = NULL;
    Decors *l_decors  = NULL;

    v_joueur->color=' ';
    v_joueur->depasser = 1;
    v_joueur->alive = 1;
    v_joueur->vitesse = 0;
    v_joueur->vitesse_max = 250;
    v_joueur->pos.y = MAX_Y - 300;
    v_joueur->route_actuelle = 0;
    v_joueur->pos.x = MAX_X/2 - (TAILLE_ROUTE*NB_ROUTES)/2 + v_joueur->route_actuelle * TAILLE_ROUTE + DECALAGE_HUD + (TAILLE_ROUTE - LARGEUR_CAR)/2;
    v_joueur->pos_cible.x = v_joueur->pos.x;
    v_joueur->pos_ini.x = v_joueur->pos.x;
    v_joueur->suivant = NULL;

    int mode = (*appState);
    vie_joueur = 5;
    if (*appState==2) {
        l_voiture = v_joueur;
        vie_joueur = 1;
    }
    
    points = 0;
    best_score = getBestScore();
    duree_game = 0;
    collisions = 0;
    time_pop = 200;

    time_B = SDL_GetTicks();
    time_D = SDL_GetTicks();
    time_A = SDL_GetTicks();
    time_C = SDL_GetTicks();
    duree_game = SDL_GetTicks();

    charger_tout (renderer);
    

    while(*appState==2 || *appState==3)
    {
        updateGame(&l_voiture, &l_decors, v_joueur, appState);
        if (SDL_GetTicks() - time_A > 40) { 
        	eventsHandlerGame(appState,v_joueur, l_voiture);
            renderGame(renderer,v_joueur, l_voiture, l_decors, police);
            time_A = SDL_GetTicks();
        }
    }

    supprimer_liste_voiture (&l_voiture);
    supprimer_liste_decors (&l_decors);
    clean_game ();

    if (mode==3) {
        char sc[100];
        demander_nom(renderer, sc);
        add_score(points, (int)((SDL_GetTicks() - duree_game)/1000), sc);
    }
}

void eventsHandlerGame(char* appState, Voiture* v_joueur, Voiture *l_voiture)
{
	// autorise le déplacement de plusieurs directions en même temps
	// sauf avant et arrière
	if (keyS==1){
        Mix_PlayChannel(2, son_klaxon, 0);
	}
	if (keyR==1) {
		move_car(v_joueur,'r');
	}
	if (keyL==1) {
		move_car(v_joueur,'l');
	}
	if (keyU==1) {
		move_car(v_joueur,'u');
	}
	else if (keyD==1) {
		move_car(v_joueur,'d');
	}
    SDL_Event event;
    while (SDL_PollEvent(&event)){
        // Detecte si on clique sur la croix
        if( event.type == SDL_QUIT )
        {
            //On quitte le programme
            *appState = 1;
        }
        // Detecte si une touche est appuyee 
        else if( event.type == SDL_KEYDOWN )
        {
            if ( event.key.keysym.sym == SDLK_ESCAPE )
            {
                *appState = 1;
            }
            if (*appState==3) {
                if ( event.key.keysym.sym == SDLK_RIGHT )
                {
                    keyR=1;
                }
                else if ( event.key.keysym.sym == SDLK_LEFT )
                {
                    keyL=1;
                }
                else if ( event.key.keysym.sym == SDLK_UP )
                {
                    keyU=1;
                }
                else if ( event.key.keysym.sym == SDLK_DOWN )
                {
                    keyD=1;
                }
                else if ( event.key.keysym.sym == SDLK_SPACE){
                	keyS=1;
                }
            }
        }
        // Detecte si une touche est laché
        else if( event.type == SDL_KEYUP )
        {
            if (*appState==3) {
                if ( event.key.keysym.sym == SDLK_RIGHT )
                {
                    keyR=0;
                }
                else if ( event.key.keysym.sym == SDLK_LEFT )
                {
                    keyL=0;
                }
                else if ( event.key.keysym.sym == SDLK_UP )
                {
                    keyU=0;
                }
                else if ( event.key.keysym.sym == SDLK_DOWN )
                {
                    keyD=0;
                }
                else if ( event.key.keysym.sym == SDLK_SPACE )
                {
                    keyS=0;
                }
            }
        }
    }

}

void updateGame(Voiture **l_voiture, Decors **l_decors, Voiture *v_joueur, char* appState)
{
    if (SDL_GetTicks() - time_B > time_pop && nombre_voiture_devant(*l_voiture, v_joueur) < MAX_CAR_SIDE) {
        ajouter_voiture (l_voiture);
        time_B = SDL_GetTicks();
    }

    if (SDL_GetTicks() - time_C > time_pop && nombre_decors_devant(*l_decors, v_joueur) < MAX_CAR_SIDE){
        ajouter_decors (l_decors);
        time_C = SDL_GetTicks();
    }

    if (SDL_GetTicks() - time_D > 50) {
        if (*appState == 2 && (*l_voiture)!=NULL) {
            deplacer_joueur (v_joueur);
            gestion_auto(*l_voiture);
            deplacer_voitureAuto (*l_voiture, v_joueur);
        } else {
        	deplacer_voiture (*l_voiture, v_joueur);
        	gestion_manuel(*l_voiture);
        }
        test_collisions(*l_voiture, v_joueur, appState);
        deplacer_decors (*l_decors, v_joueur);
        add_point(*l_voiture,v_joueur);
        supprimer_voiture_lointaine (*l_voiture, v_joueur);
        supprimer_decors_lointain (*l_decors, v_joueur);
        supprimer_voiture_dead(l_voiture);
        supprimer_decors_dead(l_decors);
        time_D = SDL_GetTicks();
    }

}

void affiche_expl (SDL_Renderer *renderer, Voiture v) {
	SDL_Rect  rectExplo = {v.pos.x+LARGEUR_CAR/2-100/2,v.pos.y+HAUTEUR_CAR/2-100/2,100,100};
	SDL_Rect  pos_image;
	int x,y;
	printf("%d\n",v.explosion);
	x = v.explosion%9;
	y = (int)v.explosion/7;

	pos_image.x = x*100;
	pos_image.y = y*100;
	pos_image.w = 100;
	pos_image.h = 100;
	SDL_RenderCopy(renderer, t_explosion, &pos_image, &rectExplo);

}


void afficher_explosion (SDL_Renderer *renderer, Voiture *l_voiture, Voiture *v_joueur) {
	Voiture *l1 = l_voiture;

	while (l1!=NULL) {
		if (v_joueur != l1){
			if (l1->alive==0){
				if (l1->explosion<60){
					affiche_expl (renderer,*l1);
					l1->explosion+=2;
				}
				else {
					l1->alive=-1;
				}
			}
		}
		l1=l1->suivant;
	}

	if (v_joueur!=NULL) {
		if (v_joueur->alive==0){
			if (v_joueur->explosion<60){
				affiche_expl (renderer,*v_joueur);
				v_joueur->explosion+=2;
			} else {
				v_joueur->alive=-1;
			}
		}
		
	}

}

void renderGame(SDL_Renderer* renderer, Voiture *v_joueur, Voiture *l_voiture, Decors *l_decors, TTF_Font *police)
{
    if (v_joueur != NULL) {
    // ici on efface l'écran
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);

    // affichage de la route
    int route;
    int y = 0;

    for (y = -1; y < MAX_Y/HAUTEUR_R+1; y++)
    {
        for (route = 0; route < NB_ROUTES; route++)
        {
            SDL_Rect rectRoute = {MAX_X/2 - (TAILLE_ROUTE*(NB_ROUTES-2*route))/2+DECALAGE_HUD, y * HAUTEUR_R + DY, TAILLE_ROUTE, HAUTEUR_R};
            SDL_RenderCopy (renderer, t_route, NULL, &rectRoute);
        }     
    }

    // défilement de la route 
    DY+=(int)((float)v_joueur->vitesse*(float)(HAUTEUR_R-1)/(float)(v_joueur->vitesse_max*3));
    if (DY>=HAUTEUR_R) DY = 0;
    else if (DY<=-1) DY=DY+HAUTEUR_R;

    
    SDL_Rect rectFond = {10, 10, 200, 180};
    SDL_RenderCopy (renderer, t_bg_hud, NULL, &rectFond);
    affiche_vie(renderer);

    // affichage du fond des textes
    // affichage du texte de vitesse
    char str[100];
    sprintf(str, "Vitesse : %d", v_joueur->vitesse);    
    affiche_texte (renderer, police, str, 20, 20);
    sprintf(str, "Points : %d", points);    
    affiche_texte (renderer, police, str, 20, 50);
    sprintf(str, "Best Score : %d", best_score);    
    affiche_texte (renderer, police, str, 20, 80);
    sprintf(str, "Temps de jeu : %d", (int)((SDL_GetTicks() - duree_game)/1000));    
    affiche_texte (renderer, police, str, 20, 110);
    sprintf(str, "Collisions : %d", collisions);    
    affiche_texte (renderer, police, str, 20, 140);

    afficher_voitures (l_voiture, renderer);
    afficher_decors (l_decors, renderer);
    SDL_Rect rectVoitureJoueur = {v_joueur->pos.x, v_joueur->pos.y, LARGEUR_CAR, HAUTEUR_CAR};
    SDL_RenderCopy (renderer, t_v_joueur, NULL, &rectVoitureJoueur);

    afficher_explosion(renderer, l_voiture, v_joueur);

	SDL_RenderPresent(renderer);
    }
}


void affiche_vie (SDL_Renderer *renderer) {
	int vie = 0;
	SDL_Rect rectCoeur = {0,300,30,30};
	for (vie = 0; vie<vie_joueur; vie++) {
		rectCoeur.x = 10 + vie * 35;
		SDL_RenderCopy(renderer, t_coeurR, NULL, &rectCoeur);
	}

	for (; vie<5; vie++){
		rectCoeur.x = 10 + vie * 35;
		SDL_RenderCopy(renderer, t_coeurD, NULL, &rectCoeur);
	}
}

void add_point (Voiture *l_voiture, Voiture *v_joueur) {
    Voiture *l_temp = l_voiture;

    while (l_temp != NULL && v_joueur != NULL) {
        if (l_temp->depasser==0 && l_temp->pos.y  >= v_joueur->pos.y) {
            points+=v_joueur->vitesse-l_temp->vitesse;
            l_temp->depasser=1;
        }
        l_temp=l_temp->suivant;
    }    
}

int collision_voiture(Voiture v1,Voiture v2)
{
   if ((v2.pos.x >= v1.pos.x + LARGEUR_CAR - 10)      // trop à droite
    || (v2.pos.x + LARGEUR_CAR - 10 <= v1.pos.x) // trop à gauche
    || (v2.pos.y >= v1.pos.y + HAUTEUR_CAR - 5) // trop en bas
    || (v2.pos.y + HAUTEUR_CAR -5 <= v1.pos.y))  // trop en haut
          return 0; 
   else
          return 1; 
}

void printSizeCar(Voiture v1){
	printf("SIZE car\n");
	printf("x= %3d  y=  %3d",v1.pos.x,v1.pos.y);
	printf("w= %3d  h=  %3d\n",v1.pos.x+LARGEUR_CAR,v1.pos.y+HAUTEUR_CAR);
}

void test_collisions (Voiture *l_voiture, Voiture *v_joueur, char *appState) {
    Voiture *lv1 = l_voiture;
    Voiture *lv2 = l_voiture;

    while (lv1!=NULL) {
        lv2 = l_voiture;

        while (lv2!=NULL) {
            if (lv1!=lv2){
                if (lv1->alive==1 && lv2->alive==1)
                    if (collision_voiture(*lv1, *lv2)==1){
                        Mix_PlayChannel(1, son_crash, 0);
                        lv1->alive=0;
                        lv2->alive=0;
                        lv1->vitesse=0;
                        lv2->vitesse=0;
                        collisions++;
                    }
            }
            lv2=lv2->suivant;
        }
        if (v_joueur != NULL && *appState==3){
            if (lv1->alive==1 && v_joueur->alive==1)
                        if (collision_voiture(*lv1, *v_joueur)==1){
                        	Mix_PlayChannel(1, son_crash, 0);
                            lv1->alive=0;
                        	lv1->vitesse=0;
                            v_joueur->alive=0;
                            v_joueur->vitesse=0;
                            collisions++;
                        }
        }
        lv1=lv1->suivant;
    }
    if (v_joueur!=NULL) {
        if (v_joueur->alive==-1) {
			if (vie_joueur>1){
    			v_joueur->explosion=0;
				v_joueur->alive=1;
				vie_joueur--;
			} else {
				*appState=1;
			}
        }
    }
}

int nombre_voitures (Voiture *l_voiture) {
    int ret = 0;
    while (l_voiture!=NULL) {
        l_voiture = l_voiture->suivant;
        ret++;
    }
    return ret;
}

int nombre_decors (Decors *l_decors) {
    int ret = 0;
    while (l_decors!=NULL) {
        l_decors = l_decors->suivant;
        ret++;
    }
    return ret;
}

void supprimer_voiture_dead (Voiture **l_voiture) {
    Voiture *cour = NULL;
    Voiture *prec = NULL;
    Voiture *temp = NULL;

    while ((*l_voiture) !=NULL && (*l_voiture)->alive==-1) { // tetes de liste
        temp = *l_voiture;
        printf("supprimé en tete\n");
        fflush(stdout);
        (*l_voiture)=(*l_voiture)->suivant;
        free(temp);
        temp=NULL;
    } // on sort quand tous les premiers successif mort sont supprimés 
    if (*l_voiture==NULL){
        //printf("liste vide\n");
    }
    prec = *l_voiture; // n'a pas besoin d'etre supprimé
    if (prec!=NULL) {
        cour = prec->suivant;
        while (cour!=NULL) {
            if (cour->alive==-1) {
                temp = cour;
                prec->suivant = cour->suivant;
                free(temp);
                temp = NULL;
                printf("supprimé au milieu\n");
            }
            else {
                prec = cour;
            }
            cour = prec->suivant;
        }
    }
}

void supprimer_decors_dead (Decors **l_decors) {
    Decors *cour = NULL;
    Decors *prec = NULL;
    Decors *temp = NULL;

    while ((*l_decors) !=NULL && (*l_decors)->alive==0) { // tetes de liste
        temp = *l_decors;
        //printf("supprimé en tete\n");
        fflush(stdout);
        (*l_decors)=(*l_decors)->suivant;
        free(temp);
        temp=NULL;
    } // on sort quand tous les premiers successif mort sont supprimés 
    if (*l_decors==NULL){
        //printf("liste vide\n");
    }
    prec = *l_decors; // n'a pas besoin d'etre supprimé
    if (prec!=NULL) {
        cour = prec->suivant;
        while (cour!=NULL) {
            if (cour->alive==0) {
                temp = cour;
                prec->suivant = cour->suivant;
                free(temp);
                temp = NULL;
                //printf("supprimé au milieu\n");
            }
            else {
                prec = cour;
            }
            cour = prec->suivant;
        }
    }
}

void supprimer_liste_voiture (Voiture **l_voiture) {
    Voiture *temp = NULL;

    while ((*l_voiture) !=NULL) { // tetes de liste
        temp = *l_voiture;
        //printf("supprimé en tete\n");
        fflush(stdout);
        (*l_voiture)=(*l_voiture)->suivant;
        free(temp);
        temp=NULL;
    } // on sort quand tous les élements de la liste sont supprimés 
    if (*l_voiture==NULL){
        printf("liste vide\n");
    }
}

void supprimer_liste_decors (Decors **l_decors) {
    Decors *temp = NULL;

    while ((*l_decors) !=NULL) { // tetes de liste
        temp = *l_decors;
        //printf("supprimé en tete\n");
        fflush(stdout);
        (*l_decors)=(*l_decors)->suivant;
        free(temp);
        temp=NULL;
    } // on sort quand tous les élements de la liste sont supprimés 
    if (*l_decors==NULL){
        printf("liste vide\n");
    }
}


void afficher_voitures (Voiture *l_voiture, SDL_Renderer* renderer){
    if (l_voiture!=NULL) {
        if (l_voiture->pos.y > 0 && l_voiture->pos.y < MAX_Y)
        {
            SDL_Rect r_voiture;
            r_voiture.x = l_voiture->pos.x;
            r_voiture.y = l_voiture->pos.y;
            r_voiture.w = LARGEUR_CAR;
            r_voiture.h = HAUTEUR_CAR;

            switch (l_voiture->color){
                case 'g':
                    SDL_RenderCopy (renderer, t_voiture_green, NULL, &r_voiture);
                break;
                case 'b':
                    SDL_RenderCopy (renderer, t_voiture_blue, NULL, &r_voiture);
                break;
                case 'm':
                    SDL_RenderCopy (renderer, t_voiture_brown, NULL, &r_voiture);
                break;
                case 'y':
                    SDL_RenderCopy (renderer, t_voiture_yellow, NULL, &r_voiture);
                break;
                case 'r':
                    SDL_RenderCopy (renderer, t_voiture_red, NULL, &r_voiture);
                break;
                case 'p':
                    SDL_RenderCopy (renderer, t_voiture_pink, NULL, &r_voiture);
                break;
                default:
                	SDL_RenderCopy (renderer, t_voiture_pink, NULL, &r_voiture);
                break;

            }
        }
        afficher_voitures (l_voiture->suivant, renderer);
    }
}

void afficher_decors (Decors *l_decors, SDL_Renderer* renderer){
    if (l_decors!=NULL) {
        if (l_decors->pos.y > 0 && l_decors->pos.y < MAX_Y)
        {
            SDL_Rect r_decors;
            r_decors.x = l_decors->pos.x;
            r_decors.y = l_decors->pos.y;
            r_decors.w = LARGEUR_CAR;
            r_decors.h = HAUTEUR_CAR;
            
            switch (l_decors->color){
                case 'g':
                    SDL_RenderCopy (renderer, t_decors1, NULL, &r_decors);
                break;
                case 'b':
                    SDL_RenderCopy (renderer, t_decors2, NULL, &r_decors);
                break;
                case 'm':
                    SDL_RenderCopy (renderer, t_decors3, NULL, &r_decors);
                break;
                case 'y':
                    SDL_RenderCopy (renderer, t_decors4, NULL, &r_decors);
                break;
                case 'r':
                    SDL_RenderCopy (renderer, t_decors6, NULL, &r_decors);
                break;
                case 'p':
                    SDL_RenderCopy (renderer, t_decors5, NULL, &r_decors);
                break;
            }
        }
        afficher_decors (l_decors->suivant, renderer);
    }
}

void move_car (Voiture* v_joueur, char direction)
{
    if (v_joueur!=NULL && v_joueur->alive==1) {
        switch (direction)
        {
            case 'r':
                if (v_joueur->pos.x < v_joueur->pos_ini.x + (NB_ROUTES-1)*TAILLE_ROUTE)
                    v_joueur->pos.x+=(2+(int)(sqrt(abs(v_joueur->vitesse))));
                break;
            case 'l':
             if (v_joueur->pos.x > v_joueur->pos_ini.x)
                    v_joueur->pos.x-=(2+(int)(sqrt(abs(v_joueur->vitesse))));
                break;
            case 'u':
                if (v_joueur->vitesse < v_joueur->vitesse_max)
                    v_joueur->vitesse+=5;
                break;
            case 'd':
                if (v_joueur->vitesse > -v_joueur->vitesse_max)
                    v_joueur->vitesse-=8;
                break;
        }
    }
}

void charger_tout (SDL_Renderer* renderer)
{
    charger_Texture (renderer, &t_v_joueur, "voiture_grise.png");
    charger_Texture (renderer, &t_route, "route.jpg");
    charger_Texture (renderer, &t_voiture, "voiture_blue.png");
    charger_Texture (renderer, &t_voiture_blue, "voiture_blue.png");
    charger_Texture (renderer, &t_voiture_red, "voiture_red.png");
    charger_Texture (renderer, &t_voiture_pink, "voiture_pink.png");
    charger_Texture (renderer, &t_voiture_brown, "voiture_brown.png");
    charger_Texture (renderer, &t_voiture_green, "voiture_green.png");
    charger_Texture (renderer, &t_voiture_yellow, "voiture_yellow.png");
    charger_Texture (renderer, &t_bg_hud, "etatA.PNG");
    charger_Texture (renderer, &t_decors1, "arbre1.png");
    charger_Texture (renderer, &t_decors2, "arbre2.png");
    charger_Texture (renderer, &t_decors3, "arbre3.png");
    charger_Texture (renderer, &t_decors4, "arbre4.png");
    charger_Texture (renderer, &t_decors5, "arbre5.png");
    charger_Texture (renderer, &t_decors6, "arbre6.png");
    charger_Texture (renderer, &t_explosion, "explosion.png");
    charger_Texture (renderer, &t_coeurR, "coeurR.png");
    charger_Texture (renderer, &t_coeurD, "coeurD.png");
    son_crash = Mix_LoadWAV("crash.wav");
    son_klaxon = Mix_LoadWAV("klaxon.wav");
}

void clean_game () {
    if (t_v_joueur!=NULL) SDL_DestroyTexture(t_v_joueur);
    if (t_route!=NULL) SDL_DestroyTexture(t_route);
    if (t_voiture!=NULL) SDL_DestroyTexture(t_voiture);
    if (t_voiture_blue!=NULL) SDL_DestroyTexture(t_voiture_blue);
    if (t_voiture_red!=NULL) SDL_DestroyTexture(t_voiture_red);
    if (t_voiture_pink!=NULL) SDL_DestroyTexture(t_voiture_pink);
    if (t_voiture_brown!=NULL) SDL_DestroyTexture(t_voiture_brown);
    if (t_voiture_green!=NULL) SDL_DestroyTexture(t_voiture_green);
    if (t_voiture_yellow!=NULL) SDL_DestroyTexture(t_voiture_yellow);
    if (t_bg_hud!=NULL) SDL_DestroyTexture(t_bg_hud);   
    if (t_decors1!=NULL) SDL_DestroyTexture(t_decors1);   
    if (t_decors2!=NULL) SDL_DestroyTexture(t_decors2);   
    if (t_decors3!=NULL) SDL_DestroyTexture(t_decors2);   
    if (t_decors4!=NULL) SDL_DestroyTexture(t_decors2);   
    if (t_decors5!=NULL) SDL_DestroyTexture(t_decors2);   
    if (t_decors6!=NULL) SDL_DestroyTexture(t_decors2);   
    if (t_explosion!=NULL) SDL_DestroyTexture(t_explosion);   
    if (t_coeurD!=NULL) SDL_DestroyTexture(t_coeurD);   
    if (t_coeurR!=NULL) SDL_DestroyTexture(t_coeurR);   
    if (son_klaxon!=NULL) Mix_FreeChunk(son_klaxon);
    if (son_crash!=NULL) Mix_FreeChunk(son_crash);
}