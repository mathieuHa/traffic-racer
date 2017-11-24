#ifndef VOITURE_H_INCLUDED
#define VOITURE_H_INCLUDED

#include "define.h"


//******************************************************
//
// DEFINITION DES STRUCTURES
//
//******************************************************

typedef struct Voiture Voiture;
struct Voiture
{
    Point pos;
    Point pos_ini;
    Point pos_cible;
    int alive;
    int vitesse;
    int vitesse_max;
    int route_actuelle;
    int depasser;
    int explosion;
    char color; // b blue, y yellow, g green, p pink, r red, m brown, g grise
    float yd;
    float yg;
    float ym;
    float ybg;
    float ybd;
    struct Voiture *suivant;
};

void ajouter_voiture (Voiture **voiture);
void affiche_liste_voiture (Voiture *voiture);
void deplacer_voitureAuto (Voiture *l_voiture, Voiture *v_joueur);
void deplacer_voiture (Voiture *l_voiture, Voiture *v_joueur);
void echanger(int tableau[], int a, int b);
void quickSort(int tableau[], int debut, int fin);
int check_car (int route_actuelle, Voiture *l_voiture);
void gestion_manuel (Voiture *l_voiture);
void gestion_auto (Voiture *l_voiture);
int pos_ini_new_car(Voiture *l_voiture);
void deplacer_joueur (Voiture *v_joueur);
int nombre_voiture_devant (Voiture *l_voiture, Voiture *v_joueur);
void supprimer_voiture_lointaine (Voiture *l_voiture, Voiture *v_joueur);
void placer_joueur (Voiture *v_joueur);



#endif // VOITURE_H_INCLUDED
