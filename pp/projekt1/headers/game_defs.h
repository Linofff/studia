#ifndef GAME_DEFS_H
#define GAME_DEFS_H

#include <ctype.h>
#include <math.h>
#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define CONFIG_FILE_NAME "config1.txt"

#define RANKING_FILE_NAME "ranking.txt"
#define TOP_N 5
#define MAX_PLAYER_NAME_LENGTH 15
#define SAVEABLE_PLAYERS 10000

#define HIT_BIRD 1
#define HIT_STAR 2
#define HIT_HUNTER 3
#define HIT_WALL 4

#define QUIT 'q'
#define REVERSE ' '
#define STOP 'b'
#define UP 'w'
#define DOWN 's'
#define LEFT 'a'
#define RIGHT 'd'
#define TAXI_IN 't'
#define TAXI_OUT 'r'
#define SPACEBAR ' '

#define FRAME_TIME 100
#define SLOWER 'o'
#define FASTER 'p'

#define MAIN_COLOR 1
#define STAT_COLOR 2
#define PLAY_COLOR 3
#define HIGH_HP_BIRD 4
#define MEDIUM_HP_BIRD 5
#define LOW_HP_BIRD 6
#define HIGH_HP_HUNTER 7
#define MEDIUM_HP_HUNTER 8
#define LOW_HP_HUNTER 9
#define STAR_COLOR 10
#define FOG_COLOR_PAIR 11

#define BORDER 1
#define OFFY 0
#define OFFX 1

#define MAX_TEMPLATES 5

#define TIME_ENTITY_MULTI 8

typedef struct {
  char name[MAX_PLAYER_NAME_LENGTH + 1];
  int score;
  int difficulty;
} RankingTemplate;

typedef struct {
  int width;
  int height;
} HunterTemplate;

typedef struct {
  int number;
  int star_quota;
  int star_max;
  int star_spawn_chance;

  int initial_hunter_max;
  int hunter_max;
  int hunter_spawn_chance;
  int hunter_damage;
  float hunter_speed;
  int initial_hunter_bounces;
  int hunter_bounces;

  int cols;
  int rows;

} LevelTemplate;

typedef struct {
  char player_name[20];

  int game_time_start;
  int game_time_left;
  int game_time_elapsed;

  LevelTemplate level;

  HunterTemplate hunter_templates[MAX_TEMPLATES];

  int start_health;

  int frame_time;
  int game_speed;
  int seed;
  unsigned int framecounter;

  int fog_min_x;
  int fog_max_x;
  int fog_currentsize;
  int fog_timer;
  int fog_update_interval;
} CONFIG;

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
  bool was_in_taxi;
  bool is_in_albatross_taxi;
} BIRD;

typedef struct {
  float fy;
  int x, y;
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
  int dashes_left;
  int sleep_timer;
  int boost_timer;
  int color;
} HUNTER;

#endif
