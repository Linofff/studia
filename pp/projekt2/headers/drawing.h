#include "main.h"

void DrawString(SDL_Surface *screen, int x, int y, const char *text,
                SDL_Surface *charset);
void DrawSurface(SDL_Surface *screen, SDL_Surface *sprite, int x, int y);
void DrawPixel(SDL_Surface *surface, int x, int y, Uint32 color);
void DrawLine(SDL_Surface *screen, int x, int y, int l, int dx, int dy,
              Uint32 color);
void DrawRectangle(SDL_Surface *screen, int x, int y, int l, int k,
                   Uint32 outlineColor, Uint32 fillColor);
SDL_Surface *FlipSurfaceHorizontal(SDL_Surface *source);
void DrawGame(SDL_Renderer *renderer, SDL_Surface *screen, SDL_Texture *scrtex,
              SDL_Surface *charset, PlayerType *player, EnemyType *enemies,
              CameraType *camera, GameStateType *gameState, double fps);
void DrawMenu(SDL_Renderer *renderer, SDL_Surface *screen, SDL_Texture *scrtex,
              SDL_Surface *charset, const char *title, const char *subtitle);
void DrawGameOver(SDL_Renderer *renderer, SDL_Surface *screen,
                  SDL_Texture *scrtex, SDL_Surface *charset, int score);
