#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "define.h"
#include "voiture.h"
#include "decors.h"
#include "game.h"


//******************************************************
//
// FONCTIONS DE CHARGEMENTS
//
//******************************************************

void ajouter_voiture (Voiture **l_voiture){

    Voiture *cour = NULL;
    Voiture *prec = NULL;
    Voiture *ajout = malloc (sizeof (Voiture));

    ajout->route_actuelle = rand()%NB_ROUTES;
    ajout->pos.x = MAX_X/2 - (TAILLE_ROUTE*NB_ROUTES)/2 + ajout->route_actuelle * TAILLE_ROUTE + DECALAGE_HUD + (TAILLE_ROUTE - LARGEUR_CAR)/2;
    ajout->pos_cible.x = ajout->pos.x;
    ajout->pos.y = pos_ini_new_car(*l_voiture);//check_car(ajout->route_actuelle,*l_voiture);
    ajout->vitesse = 100;
    ajout->vitesse_max = 100 + rand ()%100;
    if (rand()%60 < 10 )ajout->color='b';
    if (rand()%60 >= 10 && rand()%60 < 20)ajout->color='r';
    if (rand()%60 >= 20 && rand()%60 < 30)ajout->color='p';
    if (rand()%60 >= 30 && rand()%60 < 40)ajout->color='m';
    if (rand()%60 >= 40 && rand()%60 < 50)ajout->color='g';
    if (rand()%60 >= 50) ajout->color='y';
    ajout->depasser = 0;
    ajout->alive = 1;
    ajout->explosion = 0;


    if ((*l_voiture) == NULL) // cas de tete de liste première voiture a ajouter
    {
        (*l_voiture) = ajout;
        (*l_voiture)->suivant = NULL;
    } // if
    else // cas d'ajout en fin
    {
        cour = (*l_voiture);
        // on va jusqu'a la fin de la liste
        while (cour->suivant != NULL)
            cour = cour->suivant;
        prec = cour;
        cour = ajout;
        prec->suivant = cour;
        cour->suivant = NULL;
    }

}
        
void gestion_auto (Voiture *l_voiture) {
    Voiture *l1 = l_voiture;
    Voiture *l2 = l1;
    float dy;
    int gauche, droite;

    while (l1!=NULL) {
        l2=l_voiture;
        l1->yd = 100000000.0;
        l1->yg = 100000000.0;
        l1->ybd = 100000000.0;
        l1->ybg = 100000000.0;
        l1->ym = 100000000.0;

        while (l2!=NULL) {
            if (l2!=l1) {
                dy = l1->pos.y - l2->pos.y;
                if (dy>=0.0){
                    if (l1->route_actuelle == l2->route_actuelle) {
                        if (l1->ym>dy)  l1->ym = dy;
                    }
                    else if (l1->route_actuelle == l2->route_actuelle+1) {
                        if (l1->yg>dy)  l1->yg = dy; 
                    }
                    else if (l1->route_actuelle == l2->route_actuelle-1) {
                        if (l1->yd>dy) { l1->yd = dy; }
                    }
                }
                else if (dy<=0.0){
                    if (l1->route_actuelle == l2->route_actuelle+1) {
                        if (l1->ybg>-dy)  l1->ybg = -dy;
                        
                    }
                    else if (l1->route_actuelle == l2->route_actuelle-1) {
                        if (l1->ybd>-dy)  l1->ybd = -dy; 
                    }
                }
            }
            l2=l2->suivant;
        } // while 
        gauche = 0;
        droite = 0;
        
        if (l1->ym < 20 + 2*l1->vitesse + HAUTEUR_CAR) {
            if (l1->yg > l1->ym && l1->ybg > (120 + HAUTEUR_CAR) && l1->route_actuelle>0) {
                gauche = 1;
            }
            if (l1->yd > l1->ym && l1->ybd > (120 + HAUTEUR_CAR) && l1->route_actuelle<NB_ROUTES-1) {
                droite = 1;
            }
            if (gauche==1 && droite==1) {
                if (l1->yg > l1->yd){
                    l1->route_actuelle--;
                    l1->pos_cible.x-=TAILLE_ROUTE;
                }
                else {
                    l1->route_actuelle++; 
                    l1->pos_cible.x+=TAILLE_ROUTE;
                }
            }
            else if (gauche==1) {
                l1->route_actuelle--;
                l1->pos_cible.x-=TAILLE_ROUTE;

            }
            else if (droite==1) {
                l1->route_actuelle++;
                l1->pos_cible.x+=TAILLE_ROUTE;
            }
            else if (l1->vitesse>=6){
                l1->vitesse-=6;
            }
        } // if obstacle
        else if (l1->vitesse < l1->vitesse_max)
            l1->vitesse+=3;
    l1=l1->suivant;
    }
}

void gestion_manuel (Voiture *l_voiture){
	Voiture *l1 = l_voiture;
	Voiture *l2 = l1;
	float dy;

    while (l1!=NULL) {
        l2=l_voiture;
        l1->ym = 100000000.0;
        if (l1->alive==1){
        	while (l2!=NULL) {
	            if (l2!=l1 && l2->alive==1) {
	                dy = l1->pos.y - l2->pos.y;
	                if (dy>=0.0)
	                    if (l1->route_actuelle == l2->route_actuelle) 
	                        if (l1->ym>dy)  l1->ym = dy;
	            }
	            l2=l2->suivant;
        	} // while 
        
	        if (l1->ym < 20 + 2*l1->vitesse + HAUTEUR_CAR) {
	            if (l1->vitesse>=6){
	                l1->vitesse-=6;
	            }
	        } // if obstacle
	        else if (l1->vitesse < l1->vitesse_max)
	            l1->vitesse+=3;
		}
    	l1=l1->suivant;
    }

}

void supprimer_voiture_lointaine (Voiture *l_voiture, Voiture *v_joueur) {
    Voiture *l_temp = l_voiture;
    int ret = 0;
    int max = 0;
    Voiture *temp = NULL;
    if (v_joueur != NULL) {
        while (l_temp!=NULL) {
            if (l_temp->pos.y > v_joueur->pos.y){ // on comptes les voitures derrières
                ret++;
            }
            l_temp = l_temp->suivant;
        }

        while (ret > MAX_CAR_SIDE) {
            l_temp = l_voiture;
            temp = NULL;
            while (l_temp!=NULL) {
                if (l_temp->pos.y > v_joueur->pos.y){// on chercher la voiture la plus eloigné
                    if (l_temp->pos.y - v_joueur->pos.y > max) {
                        max = l_temp->pos.y - v_joueur->pos.y;
                        temp = l_temp;
                    }
                }
                l_temp = l_temp->suivant;
            }
            // tuer ici
            if (temp != NULL){
                temp->alive = -1;
            }
            ret--;
        }
    }

    // lors du passage de supprimer_voiture_dead elles seront supprimés
    // eviter d'avoir deux fonctions de suppressions
}

int pos_ini_new_car(Voiture *l_voiture) {
    int ret = 1000000;
    int vit = 0;
    Voiture *l_temp = l_voiture;
    
    while (l_temp!=NULL) {
        if (l_temp->pos.y<ret){
            ret = l_temp->pos.y;
            vit = l_temp->vitesse;
        }
        l_temp = l_temp->suivant;
    }
    ret-=150+2*vit;
    if (ret > 0) ret = -100;
    return ret;
}

void affiche_liste_voiture (Voiture *voiture){
    if (voiture!=NULL){
        printf("Voiture  ");
        printf("v : %03d\t",voiture->vitesse);
        printf("pos y : %d\t",voiture->pos.y);
        printf("voie : %d\n",voiture->route_actuelle);
        affiche_liste_voiture(voiture->suivant);
    }
    else printf("\n");
}
void echanger(int tableau[], int a, int b)
{
    int temp = tableau[a];
    tableau[a] = tableau[b];
    tableau[b] = temp;
}

int nombre_voiture_devant (Voiture *l_voiture, Voiture *v_joueur) {
    int ret = 0;
    while (l_voiture!=NULL) {
        if (l_voiture->pos.y < v_joueur->pos.y)
            ret++;
        l_voiture = l_voiture->suivant;
    }
    return ret;
}


void deplacer_voiture (Voiture *l_voiture, Voiture *v_joueur){

    if (l_voiture != NULL)
    {
    	if (l_voiture->alive==1){
    		 l_voiture->pos.y-=(l_voiture->vitesse-v_joueur->vitesse)/5.0;
	        if (l_voiture->pos.x < l_voiture->pos_cible.x) {
	            l_voiture->pos.x+=25;
	            if (l_voiture->pos.x>l_voiture->pos_cible.x) {
	                l_voiture->pos.x=l_voiture->pos_cible.x;
	            }
	        }
	        else if (l_voiture->pos.x > l_voiture->pos_cible.x) {
	            l_voiture->pos.x-=25;
	            if (l_voiture->pos.x<l_voiture->pos_cible.x) {
	                l_voiture->pos.x=l_voiture->pos_cible.x;
	            }
       	 	}
    	}
        deplacer_voiture (l_voiture->suivant,v_joueur);
    }
}

void deplacer_voitureAuto (Voiture *l_voiture, Voiture *v_joueur){

    if (l_voiture != NULL)
    {
    	if (l_voiture->alive==1){
    		l_voiture->pos.y-=(l_voiture->vitesse-v_joueur->vitesse)/5.0;
    	}
        deplacer_voiture (l_voiture->suivant,v_joueur);
    }
}

void deplacer_joueur (Voiture *v_joueur) {
    if (v_joueur != NULL) {
        if (v_joueur->pos.x < v_joueur->pos_cible.x) {
                v_joueur->pos.x+=25;
                if (v_joueur->pos.x>v_joueur->pos_cible.x) {
                    v_joueur->pos.x=v_joueur->pos_cible.x;
                }
            }
        else if (v_joueur->pos.x > v_joueur->pos_cible.x) {
            v_joueur->pos.x-=25;
            if (v_joueur->pos.x<v_joueur->pos_cible.x) {
                v_joueur->pos.x=v_joueur->pos_cible.x;
            }
        }
    }
}

void placer_joueur (Voiture *v_joueur) {
    int i;
    int deb, fin;

    for (i=0; i<NB_ROUTES; i++) {
        deb = MAX_X/2 - (TAILLE_ROUTE*NB_ROUTES)/2 + i * TAILLE_ROUTE + DECALAGE_HUD;
        fin = MAX_X/2 - (TAILLE_ROUTE*NB_ROUTES)/2 + (i+1) * TAILLE_ROUTE + DECALAGE_HUD;
        if (v_joueur->pos.x > deb && v_joueur->pos.x < fin) v_joueur->route_actuelle = i;
    }
}
