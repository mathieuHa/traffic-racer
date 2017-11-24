#ifndef SDL_INTERFACE_H_INCLUDED
#define SDL_INTERFACE_H_INCLUDED

SDL_Surface* SDLI_CreateSurface(char filePath[]);
SDL_Texture* SDLI_CreateTexture(SDL_Renderer* pRenderer, SDL_Surface* pSurface);
SDL_Surface* SDLI_CreateSurfaceFromText(TTF_Font* font, char text[], SDL_Color color);
void charger_Texture(SDL_Renderer* renderer,SDL_Texture** texture, char* chemin);
void affiche_texte (SDL_Renderer* renderer, TTF_Font *police, char str[100], int x, int y);
void affiche_texte_menu (SDL_Renderer* renderer, TTF_Font *police, char str[100], int x, int y);
void demander_nom(SDL_Renderer *renderer, char *nom);
char azerty(char value);
void affiche_texte_Blanc (SDL_Renderer* renderer, TTF_Font *police, char str[100], int x, int y);

#endif // SDL_INTERFACE_H_INCLUDED
