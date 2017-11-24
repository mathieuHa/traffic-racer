#include <stdio.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "SDL_Interface.h"

SDL_Surface* SDLI_CreateSurface(char filePath[]) {

	SDL_Surface* pSurface = IMG_Load(filePath);

	if(!pSurface) {
		printf("Echec du chargement de %s\n",filePath);
		return NULL;
	}

	return pSurface;
}

/* Fonction pour créer une Texture */
SDL_Texture* SDLI_CreateTexture(SDL_Renderer* pRenderer, SDL_Surface* pSurface) {

	SDL_Texture* pTexture = SDL_CreateTextureFromSurface(pRenderer,pSurface);

	if(!pTexture) {
		printf("Echec de la création de la texture\n");
		return NULL;
	}

	return pTexture;
}

SDL_Texture* SDLI_CreateTextureFrom() {
	return NULL;
}

SDL_Surface* SDLI_CreateSurfaceFromText(TTF_Font* font, char text[], SDL_Color color) {
	SDL_Surface* surface = TTF_RenderText_Solid(font,text,color);
 	return surface;
}

void charger_Texture(SDL_Renderer* renderer,SDL_Texture** texture, char* chemin)
{
    SDL_Surface *surface = SDLI_CreateSurface(chemin);
    (*texture) = SDLI_CreateTexture(renderer, surface);
    SDL_FreeSurface(surface);
}

void affiche_texte (SDL_Renderer* renderer, TTF_Font *police, char str[100], int x, int y) {
    // affichage du texte 
    if (police==NULL) printf("erreur de création de la police : %s\n",TTF_GetError());
    SDL_Color couleurNoire = {0, 0, 0};
    SDL_Surface* s_text = NULL;
    SDL_Texture* t_text = NULL;
    int textW, textH;
    s_text = SDLI_CreateSurfaceFromText (police, str, couleurNoire);
    t_text = SDLI_CreateTexture (renderer, s_text);
    if (s_text!=NULL) SDL_FreeSurface (s_text);
    SDL_QueryTexture (t_text, NULL, NULL, &textW, &textH);
    SDL_Rect rectText = {x, y, textW, textH};
    SDL_RenderCopy (renderer, t_text, NULL, &rectText);
    SDL_DestroyTexture (t_text);
}

void affiche_texte_Blanc (SDL_Renderer* renderer, TTF_Font *police, char str[100], int x, int y) {
    // affichage du texte 
    if (police==NULL) printf("erreur de création de la police : %s\n",TTF_GetError());
    SDL_Color couleurBlanc = {255, 255, 255};
    SDL_Surface* s_text = NULL;
    SDL_Texture* t_text = NULL;
    int textW, textH;
    s_text = SDLI_CreateSurfaceFromText (police, str, couleurBlanc);
    t_text = SDLI_CreateTexture (renderer, s_text);
    if (s_text!=NULL) SDL_FreeSurface (s_text);
    SDL_QueryTexture (t_text, NULL, NULL, &textW, &textH);
    SDL_Rect rectText = {x, y, textW, textH};
    SDL_RenderCopy (renderer, t_text, NULL, &rectText);
    SDL_DestroyTexture (t_text);
}

void affiche_texte_menu (SDL_Renderer* renderer, TTF_Font *police, char str[100], int x, int y) {
    // affichage du texte 
    if (police==NULL) printf("erreur de création de la police : %s\n",TTF_GetError());
    SDL_Color couleurBlanc = {0, 0, 0};
    SDL_Surface* s_text = NULL;
    SDL_Texture* t_text = NULL;
    int textW, textH;
    s_text = SDLI_CreateSurfaceFromText (police, str, couleurBlanc);
    t_text = SDLI_CreateTexture (renderer, s_text);
    if (s_text!=NULL) SDL_FreeSurface (s_text);
    SDL_QueryTexture (t_text, NULL, NULL, &textW, &textH);
    SDL_Rect rectText = {x-textW/2, y-textH/2, textW, textH};
    SDL_RenderCopy (renderer, t_text, NULL, &rectText);
    if (t_text!=NULL) SDL_DestroyTexture (t_text);
}

char azerty(char value)
{
    switch (value)              // on test la touche enfoncé par le joueur
    {
        //case 'a':
        //    return 'q';         // on retourne la lettre azerty qui correspond au qwerty
        //    break;
                case '.':
            return 'm';
            break;
        //case 'q':
        //    return 'a';
        //    break;
        case ';':
            return 'm';
            break;
        case ' ':               // on transfome les espaces en underscore
            return '_';
            break;
        default :               // ou la lettre est la même et on la renvoi tel quelle
            return value;
        break;
    }
} // azerty()

void demander_nom(SDL_Renderer *renderer, char *nom)
{
    SDL_Event event;

    SDL_Surface *s_text_nom = NULL;  // création des surfaces texte chargement des polices
    SDL_Texture *t_text_nom = NULL;
    SDL_Surface *s_text_question = NULL;  // création des surfaces texte chargement des polices
    SDL_Texture *t_text_question = NULL;
    TTF_Font *police   = NULL;
    SDL_Color couleurBlanche = {255, 255, 255}; // choix des couleurs
    SDL_Color couleurRouge   = {255, 0 , 0};
    SDL_Rect pos_text_question;
    SDL_Rect pos_text_nom;
    int textW, textH;
    char car;
    int choix = 1;
    int i;

    police = TTF_OpenFont("comic.otf", 40);

    // on efface l'ecran affiche le logo réefface l'ecran
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // on demande au joueur d'entrer le nom de la carte

    char str[100];
    strcpy(str,"nom du joueur : ");
    s_text_question = SDLI_CreateSurfaceFromText(police,str,couleurBlanche);
    t_text_question = SDLI_CreateTexture(renderer,s_text_question);
    SDL_QueryTexture(t_text_question,NULL,NULL,&textW,&textH);
    pos_text_question.h = textH;
    pos_text_question.w = textW;
    pos_text_question.x = 100;
    pos_text_question.y = 400;
    pos_text_nom.x = pos_text_question.x + 350;   // centrage et alignement de entrer votre nom : et du nom entré
    pos_text_nom.y = pos_text_question.y;
    SDL_RenderCopy(renderer,t_text_question,NULL,&pos_text_question);
    SDL_FreeSurface(s_text_question);
    SDL_RenderPresent(renderer);

    //SDL_Delay(2000);
    i = 0;
    nom[i] = '\0';   // le nom de la carte est initialisé au caractère NULL '\0'

    while (choix)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
            case SDL_QUIT:
                choix = 0;
                break;
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym)
                {
                    case SDLK_ESCAPE: // Veut sortir du menu
                        if (nom[0] != '\0')  // on l'y autorise seulement si il a rentré au moins un caractère
                            choix = 0;
                        else
                            SDL_RenderCopy(renderer,t_text_question,NULL,&pos_text_question); // sinon on lui réaffiche "entrer le nom : "

                        break;
                    case SDLK_RETURN:               // quitter les scores une fois le nom entré
                        if (nom[0] != '\0')  // on l'y autorise seulement si il a rentré au moins un caractère
                            choix = 0;
                        else
                            SDL_RenderCopy(renderer,t_text_question,NULL,&pos_text_question); // sinon on lui réaffiche "entrer votre nom : "

                        break;
                    case SDLK_BACKSPACE:    // autorise la correction avec la touche backspace
                        if (i > 0)          // on efface la dernière lettre
                        {                   // seulement si il y a deja un caractère d'écrit
                            i--;
                            nom[i] = '\0';   // on remplace par le caractère null
                            if (t_text_nom != NULL)
                                SDL_DestroyTexture(t_text_nom); // on efface la dernière lettre
                            s_text_nom = SDLI_CreateSurfaceFromText(police,nom,couleurRouge);
                            t_text_nom = SDLI_CreateTexture(renderer,s_text_nom);
                            SDL_QueryTexture(t_text_nom,NULL,NULL,&textW,&textH);
                            pos_text_nom.h = textH;
                            pos_text_nom.w = textW;
                            SDL_FreeSurface(s_text_nom);
                            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                            SDL_RenderClear(renderer);
                            SDL_RenderCopy(renderer,t_text_question,NULL,&pos_text_question);
                            SDL_RenderCopy(renderer,t_text_nom,NULL,&pos_text_nom);
                            SDL_DestroyTexture(t_text_nom);
                            t_text_nom = NULL;
                        }

                        break;
                    default:    // pour tout autre caractère entré
                        car = event.key.keysym.sym;     // on le test avec azerty() (qwerty en azerty)
                        //printf("caractere ajouté %c\n",car);

                        if ((((car >= 'a') && (car <= 'z')) || (car == '_')) && (i < 12)) // on accepte seulement certain caractères
                        {
                            nom[i] = car;    // ajout du caractère
                            i++;
                            nom[i] = '\0';   // on initialise le prochain au caractère null
                            if (t_text_nom != NULL)
                                SDL_DestroyTexture(t_text_nom);
                            s_text_nom = SDLI_CreateSurfaceFromText(police,nom,couleurRouge);
                            t_text_nom = SDLI_CreateTexture(renderer,s_text_nom);
                            SDL_QueryTexture(t_text_nom,NULL,NULL,&textW,&textH);
                            pos_text_nom.h = textH;
                            pos_text_nom.w = textW;
                            SDL_FreeSurface(s_text_nom);
                            SDL_RenderCopy(renderer,t_text_nom,NULL,&pos_text_nom);
                            SDL_DestroyTexture(t_text_nom);
                            t_text_nom = NULL;
                        } //if ()
                        break;
                } // switch
                    default:
                        break;
                break;
        } // switch
        SDL_RenderPresent(renderer);
    } // while

    // on libère les surfaces crées
    if (t_text_nom != NULL)
        SDL_DestroyTexture(t_text_nom);
    if (t_text_question != NULL)
        SDL_DestroyTexture(t_text_question);
    TTF_CloseFont(police); // fermeture de la police
}
