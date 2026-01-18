#ifndef MAIN_H
#define MAIN_H

#include "SDL2-2.0.10/include/SDL.h"
#include "SDL2-2.0.10/include/SDL_main.h"
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
#define LEVEL_HEIGHT 729

// jumping
#define GRAVITY 2000
#define JUMP_FORCE 500
#define PLAYER_MOVE_SPEED 250

// inputs
#define BUFFER_SIZE 10
#define BUFFER_WINDOW 1 // time to execute combo
#define MAX_COMBO_LEN 5

// timings
#define ATTACK_LIGHT_TIME 0.2
#define ATTACK_LIGHT_TIME_DELAY 0.75

#define ATTACK_HEAVY_TIME 0.5
#define ATTACK_HEAVY_TIME_DELAY 1.5

#define ATTACK_LIGHT_COMBO_TIME 0.5
#define ATTACK_LIGHT_COMBO_DELAY 2.0

#define ATTACK_MIX_COMBO_TIME 0.6
#define ATTACK_MIX_COMBO_DELAY 3.0

#define DASH_SPEED_MULTIPLIER 3
#define ATTACK_DASH_COMBO_TIME 0.25
#define ATTACK_DASH_COMBO_DELAY 1.0

// for enemy
#define ATTACK_COOLDOWN 1.0

// damage
#define ATTACK_LIGHT_DAMAGE 10
#define ATTACK_HEAVY_DAMAGE 30
#define ATTACK_LIGHT_COMBO_DAMAGE 20
#define ATTACK_MIX_COMBO_DAMAGE 35
#define ATTACK_DASH_COMBO_DAMAGE 15
#define ENEMY_STUN 1.0

#define PLAYER_HEALTH 500

#define NUMBER_OF_ENEMIES 3

// player attack hitbox height
#define ATTACK_HITBOX_H 50

// enemy attack range
#define ATTACK_RANGE 120.0

// directions
#define RIGHT 0
#define LEFT 1
#define UP 2
#define DOWN 3

// animation frames
#define MAX_WALK_FRAMES 8
#define MAX_ATTACK_FRAMES 19
#define MAX_DASH_FRAMES 6
#define MAX_AIR_FRAMES 5
#define MAX_IDLE_FRAMES 8
#define MAX_HIT_FRAMES 4
#define MAX_DEATH_FRAMES 10

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

enum PlayerState {
  DEAD,
  IDLE,
  RUNNING,
  JUMPING,
  ATTACK_LIGHT,
  ATTACK_HEAVY,
  COMBO_TRIPLE, // same button 3x
  COMBO_MIX,    // Sequence A B
  DASH          // Dash
};

typedef struct {
  double X;
  double Y;
} CameraType;

typedef struct {
  double worldTime;
  int debugMode;
  GameStateEnum currentState;
} GameStateType;

typedef struct {
  InputType input;
  double timestamp;
} InputEvent;

typedef struct {
  InputEvent events[BUFFER_SIZE];
  int count;
} InputBuffer;

typedef struct {
  double X, Y;
  double dy;
  double landingY;
  double speed;
  int direction;
  int facingLeft;
  int onGround;

  int health;
  int score;
  int state;

  double attackTimer;
  double basicCooldownTimer;
  double comboCooldownTimer;
  double wasHitTimer;
  double animTimer;

  int multiplier;
  double multiplierTimer;

  double animSpeed;
  int maxFrames;
  int currentFrame;

  int damage;
  InputBuffer buffer;
  int prev_keys[SDL_NUM_SCANCODES];

  SDL_Surface *walk_frames_right[MAX_WALK_FRAMES];
  SDL_Surface *walk_frames_left[MAX_WALK_FRAMES];
  SDL_Surface *attack_frames_right[MAX_ATTACK_FRAMES];
  SDL_Surface *attack_frames_left[MAX_ATTACK_FRAMES];
  SDL_Surface *dash_frames_right[MAX_DASH_FRAMES];
  SDL_Surface *dash_frames_left[MAX_DASH_FRAMES];
  SDL_Surface *air_frames_right[MAX_AIR_FRAMES]; // Fixed typo: MAX_AIR_FRAMES
  SDL_Surface *air_frames_left[MAX_AIR_FRAMES];  // Fixed typo: MAX_AIR_FRAMES
  SDL_Surface *idle_frames_right[MAX_IDLE_FRAMES];
  SDL_Surface *idle_frames_left[MAX_IDLE_FRAMES];
  SDL_Surface *hit_frames_right[MAX_HIT_FRAMES];
  SDL_Surface *hit_frames_left[MAX_HIT_FRAMES];
  SDL_Surface *death_frames_right[MAX_DEATH_FRAMES];
  SDL_Surface *death_frames_left[MAX_DEATH_FRAMES];

} PlayerType;

typedef struct {
  //  type 0 (red) assets
  SDL_Surface *t0_walk_right[MAX_WALK_FRAMES];
  SDL_Surface *t0_walk_left[MAX_WALK_FRAMES];
  SDL_Surface *t0_hit_right[MAX_HIT_FRAMES];
  SDL_Surface *t0_hit_left[MAX_HIT_FRAMES];
  SDL_Surface *t0_attack_right[MAX_ATTACK_FRAMES];
  SDL_Surface *t0_attack_left[MAX_ATTACK_FRAMES];

  //  type 1 (green) assets
  SDL_Surface *t1_walk_right[MAX_WALK_FRAMES];
  SDL_Surface *t1_walk_left[MAX_WALK_FRAMES];
  SDL_Surface *t1_hit_right[MAX_HIT_FRAMES];
  SDL_Surface *t1_hit_left[MAX_HIT_FRAMES];
  SDL_Surface *t1_attack_right[MAX_ATTACK_FRAMES];
  SDL_Surface *t1_attack_left[MAX_ATTACK_FRAMES];
  SDL_Surface *t1_dash_right[MAX_DASH_FRAMES];
  SDL_Surface *t1_dash_left[MAX_DASH_FRAMES];
} EnemyAssets;

typedef struct {
  int type;
  int alive;
  int health;
  int damage;
  double speed;

  double X;
  double Y;
  int direction;

  int ai_state;
  double ai_timer;
  double chargeDirX;
  double chargeDirY;
  double attack_timer;
  double stun_timer;

  int currentFrame;
  double animTimer;

  SDL_Surface *walk_frames_right[MAX_WALK_FRAMES];
  SDL_Surface *walk_frames_left[MAX_WALK_FRAMES];
  SDL_Surface *hit_frames_right[MAX_HIT_FRAMES];
  SDL_Surface *hit_frames_left[MAX_HIT_FRAMES];
  SDL_Surface *attack_frames_right[MAX_ATTACK_FRAMES];
  SDL_Surface *attack_frames_left[MAX_ATTACK_FRAMES];
  SDL_Surface *dash_frames_right[MAX_DASH_FRAMES];
  SDL_Surface *dash_frames_left[MAX_DASH_FRAMES];
} EnemyType;

#endif
