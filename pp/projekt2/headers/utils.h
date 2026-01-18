#ifndef UTILS_H
#define UTILS_H

#include "main.h"

void Cleanup(SDL_Window *window, SDL_Renderer *renderer, SDL_Surface *screen,
             SDL_Texture *scrtex, SDL_Surface *charset, PlayerType *player,
             EnemyType *enemies, EnemyAssets *assets);
int InitializeSDL(SDL_Window **window, SDL_Renderer **renderer);
SDL_Surface *FlipSurfaceHorizontal(SDL_Surface *source);
void LoadAnimationSequence(const char *prefix, int count,
                           SDL_Surface **destRight, SDL_Surface **destLeft);

#endif
