#ifndef GAME_DEFS_H
#define GAME_DEFS_H

#include <math.h>
#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define RANKING_FILE "ranking.txt"
#define TOP_N 5

#define QUIT 'q'
#define REVERSE ' '
#define UP 'w'
#define DOWN 's'
#define LEFT 'a'
#define RIGHT 'd'
#define TAXI_IN 't'
#define TAXI_OUT 'r'

// #define FRAME_TIME 100
#define SLOWER 'o'
#define FASTER 'p'

#define MAIN_COLOR 1
#define STAT_COLOR 2
#define PLAY_COLOR 3
#define BIRD_COLOR1 4
#define BIRD_COLOR2 5
#define BIRD_COLOR3 6
#define HUNTER_COLOR 7
#define TAXI_COLOR 8

#define BORDER 1
#define ROWS 20
#define COLS 80
#define OFFY 0
#define OFFX 1

#define MAX_TEMPLATES 5

typedef struct {
  int width;
  int height;
} HunterTemplate;

// --- CONFIGURATION STRUCT ---
typedef struct {
  int game_time_start;
  int game_time_left;
  int game_time_elapsed;

  int star_max;
  int star_quota;
  int star_spawn_chance;
  float star_speed;

  int initial_hunter_max;
  int hunter_max;
  int hunter_spawn_chance;
  int hunter_damage;
  float hunter_speed;
  int initial_hunter_bounces;
  int hunter_bounces;

  HunterTemplate hunter_templates[MAX_TEMPLATES];

  int start_health;

  int frame_time;
  int game_speed;
  int seed;
} CONFIG;

// --- GAME STRUCTS ---
typedef struct {
  WINDOW *window;
  int rows, cols;
  int color;
} WIN;

typedef struct {
  WIN *win;
  int x, y;
  float dx, dy;
  float speed;
  char symbol;
  int color;
  int points;
  int health;
  int albatross_taxi_left;
  int albatross_in_cooldown;
  int albatross_out_cooldown;
  bool is_in_albatross_taxi;
} BIRD;

typedef struct {
  float x, y;
  int alive;
  float speed;
} STAR;

typedef struct {
  float fx, fy;
  float vx, vy;
  int x, y;
  int width, height;
  int alive;
  int damage;
  int bounces;
  int initial_bird_x, initial_bird_y;
  int dashleft;
  int sleep_timer;
  int boost_timer;
} HUNTER;

#endif
