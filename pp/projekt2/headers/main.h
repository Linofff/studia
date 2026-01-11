#ifndef MAIN_H
#define MAIN_H

#include "../SDL2-2.0.10/include/SDL.h"
#include "../SDL2-2.0.10/include/SDL_main.h"
#include "../SDL2-2.0.10/include/SDL_pixels.h"
#include "../SDL2-2.0.10/include/SDL_sensor.h"
#include "../SDL2-2.0.10/include/SDL_surface.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define _USE_MATH_DEFINES
#define SCREEN_WIDTH 960
#define SCREEN_HEIGHT 720

#define LEVEL_WIDTH 960
#define LEVEL_HEIGHT 720

#define NUMBER_OF_ENEMIES 4

#define GRAVITY 2000
#define JUMP_FORCE 500
#define PLAYER_MOVE_SPEED 250

#define BUFFER_SIZE 10
#define BUFFER_WINDOW 1 // 1s cleanup
#define MAX_COMBO_LEN 5

#define ATTACK_LIGHT_TIME 0.2
#define ATTACK_HEAVY_TIME 0.5
#define ATTACK_LIGHT_TIME_DELAY 0.2
#define ATTACK_HEAVY_TIME_DELAY 0.5
#define HITBOX_H 50

typedef struct {
  double X;
  double Y;
} CameraType;

typedef enum {
  IN_NONE,
  IN_LEFT,
  IN_RIGHT,
  IN_UP,
  IN_DOWN,
  IN_LIGHT,
  IN_HEAVY
} InputType;

enum PlayerState {
  IDLE,
  RUNNING,
  JUMPING,
  ATTACK_LIGHT,
  ATTACK_HEAVY,

  COMBO_TRIPLE, // (i) 3x same button
  COMBO_MIX,    // (ii) Sequence X -> Y
  DASH          // (iii) Dash
};

typedef struct {
  InputType input;
  double timestamp;
} InputEvent;

typedef struct {
  InputEvent events[BUFFER_SIZE];
  int head; // Points to the next empty slot
  int count;
} InputBuffer;

typedef struct {
  double worldTime;
  int debugMode; // (Point D) Developer Mode Toggle
} GameState;

typedef struct {
  double speed;
  int health;
  int direction; // 0 Right, 1 Left

  double dy;
  double X, Y;
  double landingY;

  int onGround;
  int score;
  int multiplier;
  double multiplierTimer;

  int state;
  double attackTimer;
  double cooldownTimer;

  InputBuffer buffer;
  int prev_keys[SDL_NUM_SCANCODES];

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

  int type;
  int ai_state;
  double ai_timer;
  double chargeDirX;
  double chargeDirY;
  double strun_timer;
} EnemyType;

#endif
