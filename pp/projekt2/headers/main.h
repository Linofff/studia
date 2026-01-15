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

// #define LEVEL_WIDTH 1920
// #define LEVEL_HEIGHT 1440

#define NUMBER_OF_ENEMIES 4

#define PLAYER_HEALTH 500

#define GRAVITY 2000
#define JUMP_FORCE 500
#define PLAYER_MOVE_SPEED 250

#define BUFFER_SIZE 10
#define BUFFER_WINDOW 1 // 1s cleanup
#define MAX_COMBO_LEN 5

#define ATTACK_LIGHT_TIME 0.2
#define ATTACK_HEAVY_TIME 0.5
#define ATTACK_LIGHT_TIME_DELAY 0.75
#define ATTACK_HEAVY_TIME_DELAY 1.5
#define ATTACK_LIGHT_COMBO_DELAY 2.0
#define ATTACK_MIX_COMBO_DELAY 3.0
#define ATTACK_DASH_COMBO_DELAY 1.0

#define ATTACK_LIGHT_DAMAGE 10
#define ATTACK_HEAVY_DAMAGE 30
#define ATTACK_LIGHT_COMBO_DAMAGE 20
#define ATTACK_MIX_COMBO_DAMAGE 35
#define ATTACK_DASH_COMBO_DAMAGE 15

#define ENEMY_STUN 1.0

#define HITBOX_H 50

typedef struct {
  int damage;
} AttackType;

typedef struct {
  double X;
  double Y;
} CameraType;

typedef enum { STATE_MENU, STATE_GAME, STATE_GAMEOVER } GameStateEnum;

typedef enum {
  IN_NONE,
  IN_JUMP,
  IN_LEFT,
  IN_RIGHT,
  IN_UP,
  IN_DOWN,
  IN_LIGHT,
  IN_HEAVY
} InputType;

#define RIGHT 0
#define LEFT 1
#define UP 2
#define DOWN 3

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
  GameStateEnum currentState;
} GameState;

#define MAX_WALK_FRAMES 8
#define MAX_ATTACK_FRAMES 19
#define MAX_DASH_FRAMES 6
#define MAX_AIR_FRAMES 6

typedef struct {
  double speed;
  int health;
  int direction;
  int facingLeft;

  double dy;
  double X, Y;
  double landingY;

  int onGround;
  int score;
  int multiplier;
  double multiplierTimer;

  int state;
  double attackTimer;
  double basicCooldownTimer;
  double comboCooldownTimer;

  double animSpeed;
  int maxFrames;
  int multiplierScale;

  AttackType attack;

  InputBuffer buffer;
  int prev_keys[SDL_NUM_SCANCODES];

  SDL_Surface *walk_frames_right[MAX_WALK_FRAMES];
  SDL_Surface *walk_frames_left[MAX_WALK_FRAMES];
  SDL_Surface *attack_frames_rigth[MAX_ATTACK_FRAMES];
  SDL_Surface *attack_frames_left[MAX_ATTACK_FRAMES];
  SDL_Surface *dash_frames_rigth[MAX_DASH_FRAMES];
  SDL_Surface *dash_frames_left[MAX_DASH_FRAMES];
  SDL_Surface *air_frames_rigth[MAX_DASH_FRAMES];
  SDL_Surface *air_frames_left[MAX_DASH_FRAMES];

  int currentFrame;
  double animTimer;
} PlayerType;

typedef struct {
  double speed;
  int alive;
  int health;
  double X;
  double Y;
  int direction; // 0 Right, 1 Left

  // CHANGED: Replaced single surface with array
  SDL_Surface *walk_frames_right[MAX_WALK_FRAMES];
  SDL_Surface *walk_frames_left[MAX_WALK_FRAMES];

  // Animation State
  int currentFrame;
  double animTimer;

  int damage;
  double attack_timer;

  int type;
  int ai_state;
  double ai_timer;
  double chargeDirX;
  double chargeDirY;
  double stun_timer;
} EnemyType;

#endif
