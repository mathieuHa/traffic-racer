#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

void mainMenu(SDL_Renderer* renderer, char* appState, TTF_Font *police);
void eventsHandlerMenu(char* appState);
void updateMenu();
void renderMenu(SDL_Renderer* renderer, TTF_Font *police);
void charger_tout_menu (SDL_Renderer* renderer);
void affiche_texte_centre (SDL_Renderer* renderer, TTF_Font *police, int etage, char str[100]);
int collision (SDL_Rect a, SDL_Rect b);
void clean_menu ();



#endif // MENU_H_INCLUDED
