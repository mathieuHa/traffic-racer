#ifndef SC0RE_H_INCLUDED
#define SCORE_H_INCLUDED

void add_score (int score, int time, char nom[100]);
int getBestScore ();
void affiche_score (SDL_Renderer* renderer, char *appState);
void afficher_score(SDL_Renderer* renderer, char *appState);



#endif // 