#ifndef MAIN_H
#define MAIN_H

#include "./SDL2-2.0.10/include/SDL.h"
#include "./SDL2-2.0.10/include/SDL_main.h"
#include "SDL2-2.0.10/include/SDL_pixels.h"
#include "SDL2-2.0.10/include/SDL_sensor.h"
#include "SDL2-2.0.10/include/SDL_surface.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define _USE_MATH_DEFINES
#define SCREEN_WIDTH 960
#define SCREEN_HEIGHT 720

#define LEVEL_WIDTH 960
#define LEVEL_HEIGHT 720

#define NUMBER_OF_ENEMIES 3

typedef struct {
  double worldTime;
} GameState;

typedef struct {
  double X;
  double Y;
} CameraType;

typedef struct {
  double speed;
  int health;
  double X;
  double Y;
  int direction; // 0 Right, 1 Left
  SDL_Surface *surface_right, *surface_left;
} PlayerType;

typedef struct {
  double speed;
  int alive;
  int health;
  double X;
  double Y;
  int direction; // 0 Right, 1 Left
  SDL_Surface *surface_right, *surface_left;
} EnemyType;

void InitPlayer(PlayerType *player);
void InitNewGame(GameState *state, PlayerType *player, CameraType *camera);
SDL_Surface *FlipSurfaceHorizontal(SDL_Surface *src);

#endif
