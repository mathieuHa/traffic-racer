#ifndef DECORS_H_INCLUDED
#define DECORS_H_INCLUDED

#include "define.h"


//******************************************************
//
// DEFINITION DES STRUCTURES
//
//******************************************************

typedef struct Decors Decors;
struct Decors
{
    Point pos;
    int alive;
    int route_actuelle;
    char color; // b blue, y yellow, g green, p pink, r red, m brown, g grise
    struct Decors *suivant;
};

void ajouter_decors (Decors **l_decors);
int pos_ini_new_decors(Decors *l_decors);
void affiche_liste_decors (Decors *decors);
void deplacer_decors (Decors *l_decors, Voiture *v_joueur);
int nombre_decors_devant (Decors *l_decors, Voiture *v_joueur);
void supprimer_decors_lointain (Decors *l_decors, Voiture *v_joueur);




#endif // DECORS_H_INCLUDED
