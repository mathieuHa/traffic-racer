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

void ajouter_decors (Decors **l_decors){

    Decors *cour = NULL;
    Decors *prec = NULL;
    Decors *ajout = malloc (sizeof (Decors));

    ajout->route_actuelle = (rand()%2==0) ? NB_ROUTES : -1;
    ajout->pos.x = MAX_X/2 - (TAILLE_ROUTE*NB_ROUTES)/2 + ajout->route_actuelle * TAILLE_ROUTE + DECALAGE_HUD + (TAILLE_ROUTE - LARGEUR_CAR)/2;
    ajout->pos.y = pos_ini_new_decors(*l_decors);
    if (rand()%60 < 10 )ajout->color='b';
    if (rand()%60 >= 10 && rand()%60 < 20)ajout->color='r';
    if (rand()%60 >= 20 && rand()%60 < 30)ajout->color='p';
    if (rand()%60 >= 30 && rand()%60 < 40)ajout->color='m';
    if (rand()%60 >= 40 && rand()%60 < 50)ajout->color='g';
    if (rand()%60 >= 50) ajout->color='y';
    ajout->alive = 1;


    if ((*l_decors) == NULL) // cas de tete de liste premier decors a ajouter
    {
        (*l_decors) = ajout;
        (*l_decors)->suivant = NULL;
    } // if
    else // cas d'ajout en fin
    {
        cour = (*l_decors);
        // on va jusqu'a la fin de la liste
        while (cour->suivant != NULL)
            cour = cour->suivant;
        prec = cour;
        cour = ajout;
        prec->suivant = cour;
        cour->suivant = NULL;
    }

}


void supprimer_decors_lointain (Decors *l_decors, Voiture *v_joueur) {
    Decors *l_temp = l_decors;
    int ret = 0;
    int max = 0;
    Decors *temp = NULL;
    if (v_joueur != NULL) {
        while (l_temp!=NULL) {
            if (l_temp->pos.y > v_joueur->pos.y){ // on comptes les decors derrières
                ret++;
            }
            l_temp = l_temp->suivant;
        }

        while (ret > MAX_CAR_SIDE) {
            l_temp = l_decors;
            temp = NULL;
            while (l_temp!=NULL) {
                if (l_temp->pos.y > v_joueur->pos.y){// on chercher le decors le plus eloigné
                    if (l_temp->pos.y - v_joueur->pos.y > max) {
                        max = l_temp->pos.y - v_joueur->pos.y;
                        temp = l_temp;
                    }
                }
                l_temp = l_temp->suivant;
            }
            // tuer ici
            if (temp != NULL){
                temp->alive = 0;
            }
            ret--;
        }
    }

    // lors du passage de supprimer_voiture_dead elles seront supprimés
    // eviter d'avoir deux fonctions de suppressions
}

int pos_ini_new_decors(Decors *l_decors) {
    int ret = 1000000;
    Decors *l_temp = l_decors;
    
    while (l_temp!=NULL) {
        if (l_temp->pos.y<ret){
            ret = l_temp->pos.y;
        }
        l_temp = l_temp->suivant;
    }
    ret-=2*HAUTEUR_CAR;
    if (ret > 0) ret = -HAUTEUR_CAR - 100;
    return ret;
}


void affiche_liste_decors (Decors *decors){

    if (decors!=NULL){
        printf("Voiture  ");
        printf("pos y : %d\t",decors->pos.y);
        printf("voie : %d\n",decors->route_actuelle);
        affiche_liste_decors(decors->suivant);
    }
    else printf("\n");
}
void deplacer_decors (Decors *l_decors, Voiture *v_joueur) {

    if (l_decors != NULL)
    {
        //l_decors->pos.y-=(0-v_joueur->vitesse)/3.0;
        //l_decors->pos.y+=(int)((float)v_joueur->vitesse);
        l_decors->pos.y+=(int)((float)v_joueur->vitesse*(float)(HAUTEUR_R-1)/(float)(v_joueur->vitesse_max*3));
        deplacer_decors (l_decors->suivant,v_joueur);
    }
}

int nombre_decors_devant (Decors *l_decors, Voiture *v_joueur) {
    int ret = 0;
    while (l_decors!=NULL) {
        if (l_decors->pos.y < v_joueur->pos.y)
            ret++;
        l_decors = l_decors->suivant;
    }
    return ret;
}



