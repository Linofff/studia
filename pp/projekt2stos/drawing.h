#ifndef DRAWING_H
#define DRAWING_H

#include "main.h"

const char *GetInputName(InputType in);
const char *GetStateName(int state);
void DrawOutline(SDL_Surface *screen, int x, int y, int w, int h, Uint32 color);
void DrawStringScaled(SDL_Surface *screen, int x, int y, const char *text,
                      SDL_Surface *charset, int scale);
SDL_Surface *GetPlayerSprite(PlayerType *player);
void DrawPlayerDebugOverlay(SDL_Surface *screen, SDL_Surface *charset,
                            PlayerType *player);
void DrawPlayerAttackDebug(SDL_Surface *screen, PlayerType *player,
                           CameraType *camera, SDL_Surface *currSurf, int red);
void DrawPlayer(SDL_Surface *screen, SDL_Surface *charset, PlayerType *player,
                CameraType *camera, GameStateType *gameState, int red,
                int green);
SDL_Surface *GetEnemySprite(EnemyType *enemy);
void DrawSingleEnemy(EnemyType *enemy, SDL_Surface *screen, CameraType *camera,
                     int yellow, int debugMode);
void DrawEnemies(EnemyType *enemies, SDL_Surface *screen, CameraType *camera,
                 int yellow, GameStateType *gameState);
void DrawBackground(SDL_Surface *screen, CameraType *camera);
void DrawHUD(SDL_Surface *screen, SDL_Surface *charset, PlayerType *player,
             GameStateType *gameState, double fps);
void DrawMultiplier(SDL_Surface *screen, SDL_Surface *charset,
                    PlayerType *player);
void DrawGame(SDL_Renderer *renderer, SDL_Surface *screen, SDL_Texture *scrtex,
              SDL_Surface *charset, PlayerType *player, EnemyType *enemies,
              CameraType *camera, GameStateType *gameState, double fps);
void DrawMenu(SDL_Renderer *renderer, SDL_Surface *screen, SDL_Texture *scrtex,
              SDL_Surface *charset, const char *title, const char *subtitle);
void DrawGameOver(SDL_Renderer *renderer, SDL_Surface *screen,
                  SDL_Texture *scrtex, SDL_Surface *charset, int score);
void DrawString(SDL_Surface *screen, int x, int y, const char *text,
                SDL_Surface *charset);
void DrawSurface(SDL_Surface *screen, SDL_Surface *sprite, int x, int y);
void DrawPixel(SDL_Surface *surface, int x, int y, Uint32 color);
void DrawLine(SDL_Surface *screen, int x, int y, int l, int dx, int dy,
              Uint32 color);
void DrawRectangle(SDL_Surface *screen, int x, int y, int l, int k,
                   Uint32 outlineColor, Uint32 fillColor);

#endif
