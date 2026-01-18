#ifndef UTILS_H
#define UTILS_H

#include "main.h"

int InitSDLContext();
int CreateWinRenderer(SDL_Window **window, SDL_Renderer **renderer);
void ConfigureGraphics(SDL_Window *window, SDL_Renderer *renderer);
int InitializeSDL(SDL_Window **window, SDL_Renderer **renderer);
void CopyPixelsFlipped(SDL_Surface *src, SDL_Surface *dst);
SDL_Surface *FlipSurfaceHorizontal(SDL_Surface *source);
void LoadSingleFrame(const char *prefix, int index, SDL_Surface **destRight,
                     SDL_Surface **destLeft);
void LoadAnimationSequence(const char *prefix, int count,
                           SDL_Surface **destRight, SDL_Surface **destLeft);
void FreeSurfaceArray(SDL_Surface **arr, int count);
void FreeEnemyAssets(EnemyAssets *assets);
void FreePlayerMemory(PlayerType *player);
void Cleanup(SDL_Window *window, SDL_Renderer *renderer, SDL_Surface *screen,
             SDL_Texture *scrtex, SDL_Surface *charset, PlayerType *player,
             EnemyType *enemies, EnemyAssets *assets);

#endif
