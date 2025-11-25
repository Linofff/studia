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

#define FRAME_TIME 100
#define MAIN_COLOR 1
#define STAT_COLOR 2
#define PLAY_COLOR 3
#define BIRD_COLOR 4
#define HUNTER_COLOR 5

#define BORDER 1
#define ROWS 20
#define COLS 80
#define OFFY 0
#define OFFX 1

// --- CONFIGURATION STRUCT ---
typedef struct {
  int star_max;
  int star_quota;
  int game_time;
  int game_time_elapsed;
  int star_spawn_chance;
  float star_speed;

  int initial_hunter_max;
  int hunter_max;
  int hunter_spawn_chance;
  int hunter_damage;
  float hunter_speed;
  int initial_hunter_bounces;
  int hunter_bounces;

  int start_health;
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
  int dx, dy;
  int speed;
  char symbol;
  int color;
  int points;
  int health;
  bool is_in_albatros_taxi;
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
