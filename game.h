#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

void mainGame(SDL_Renderer* renderer, char* appState, TTF_Font *police);
void eventsHandlerGame(char* appState, Voiture *v_joueur, Voiture *l_voiture);
void updateGame(Voiture **l_voiture, Decors **l_decors, Voiture *v_joueur, char * appState);
void renderGame(SDL_Renderer* renderer,Voiture *v_joueur, Voiture *l_voiture, Decors *l_decors, TTF_Font *police);
//void charger_Texture(SDL_Renderer* renderer,SDL_Texture** texture, char* chemin);
void charger_tout (SDL_Renderer* renderer);
void afficher_voitures (Voiture *l_voiture, SDL_Renderer* renderer);
void afficher_decors (Decors *l_decors, SDL_Renderer* renderer);
void move_car (Voiture* v_joueur, char direction);
void add_point (Voiture *l_voiture, Voiture *v_joueur);
void test_collisions (Voiture *l_voiture, Voiture *v_joueur, char *appState);
int collision_Voiture(Voiture v1,Voiture v2);
void supprimer_voiture_dead (Voiture **l_voiture);
void supprimer_decors_dead (Decors **l_decors);
int nombre_voitures (Voiture *l_voiture);
int nombre_decors (Decors *l_decors);
void supprimer_liste_voiture (Voiture **l_voiture);
void supprimer_liste_decors (Decors **l_decors);
void afficher_explosion (SDL_Renderer *renderer, Voiture *l_voiture, Voiture *v_joueur);
void clean_game ();
void affiche_expl (SDL_Renderer *renderer, Voiture v);
void affiche_vie (SDL_Renderer *renderer);






#endif // GAME_H_INCLUDED
