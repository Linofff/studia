#include <math.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

// --- CONSTANTS (Non-Gameplay) ---
#define QUIT 'q'
#define REVERSE ' '
#define UP 'w'
#define DOWN 's'
#define LEFT 'a'
#define RIGHT 'd'

#define FRAME_TIME 100
#define MAIN_COLOR 1
#define STAT_COLOR 2
#define PLAY_COLOR 3
#define BIRD_COLOR 4
#define HUNTER_COLOR 5

#define BORDER 1
#define ROWS 20
#define COLS 80
#define OFFY 2
#define OFFX 5

// --- CONFIGURATION STRUCT ---
typedef struct {
  int star_quota;
  int game_time;
  int star_spawn_chance;
  int points_per_star;

  int hunter_quota;
  int hunter_spawn_chance;
  int hunter_damage;
  float hunter_speed;
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
} BIRD;

typedef struct {
  int x, y;
  int alive;
} STAR;

typedef struct {
  float fx, fy;
  float vx, vy;
  int x, y;
  int width, height;
  int alive;
  int damage;
  int bounces;
} HUNTER;

// --- FILE LOADING ---

void LoadConfig(CONFIG *c) {
  FILE *f = fopen("settings.txt", "r");
  char k[64];
  while (fscanf(f, "%s", k) > 0) {
    !strcmp(k, "STAR_QUOTA") && fscanf(f, "%d", &c->star_quota);
    !strcmp(k, "GAME_TIME") && fscanf(f, "%d", &c->game_time);
    !strcmp(k, "STAR_SPAWN_CHANCE") && fscanf(f, "%d", &c->star_spawn_chance);
    !strcmp(k, "POINTS_PER_STAR") && fscanf(f, "%d", &c->points_per_star);
    !strcmp(k, "HUNTER_QUOTA") && fscanf(f, "%d", &c->hunter_quota);
    !strcmp(k, "HUNTER_SPAWN_CHANCE") &&
        fscanf(f, "%d", &c->hunter_spawn_chance);
    !strcmp(k, "HUNTER_DAMAGE") && fscanf(f, "%d", &c->hunter_damage);
    !strcmp(k, "HUNTER_SPEED") && fscanf(f, "%f", &c->hunter_speed);
    !strcmp(k, "HUNTER_BOUNCES") && fscanf(f, "%d", &c->hunter_bounces);
    !strcmp(k, "START_HEALTH") && fscanf(f, "%d", &c->start_health);
  }
  fclose(f);
}

// --- INITIALIZATION FUNCTIONS ---

WINDOW *Start() {
  WINDOW *win;
  if ((win = initscr()) == NULL) {
    fprintf(stderr, "Error initialising ncurses.\n");
    exit(EXIT_FAILURE);
  }
  start_color();
  init_pair(MAIN_COLOR, COLOR_WHITE, COLOR_BLACK);
  init_pair(PLAY_COLOR, COLOR_CYAN, COLOR_BLACK);
  init_pair(STAT_COLOR, COLOR_YELLOW, COLOR_BLUE);
  init_pair(BIRD_COLOR, COLOR_RED, COLOR_BLACK);
  init_pair(HUNTER_COLOR, COLOR_MAGENTA, COLOR_BLACK);
  noecho();
  curs_set(0);
  return win;
}

void CleanWin(WIN *W, int bo) {
  wattron(W->window, COLOR_PAIR(W->color));
  if (bo)
    box(W->window, 0, 0);
  for (int i = bo; i < W->rows - bo; i++)
    for (int j = bo; j < W->cols - bo; j++)
      mvwprintw(W->window, i, j, " ");
  wrefresh(W->window);
}

WIN *InitWin(WINDOW *parent, int rows, int cols, int y, int x, int color,
             int bo, int delay) {
  WIN *W = (WIN *)malloc(sizeof(WIN));
  W->rows = rows;
  W->cols = cols;
  W->color = color;
  W->window = subwin(parent, rows, cols, y, x);
  CleanWin(W, bo);
  if (delay == 0)
    nodelay(W->window, TRUE);
  wrefresh(W->window);
  return W;
}

BIRD *InitBird(WIN *w, int x, int y, int startHealth) {
  BIRD *b = (BIRD *)malloc(sizeof(BIRD));
  b->win = w;
  b->x = x;
  b->y = y;
  b->dx = 1;
  b->dy = 0;
  b->speed = 1;
  b->symbol = '8';
  b->color = BIRD_COLOR;
  b->points = 0;
  b->health = startHealth;
  return b;
}

// --- MOVEMENT & DRAWING ---

void DrawBird(BIRD *b) {
  wattron(b->win->window, COLOR_PAIR(b->color));
  mvwprintw(b->win->window, b->y, b->x, "%c", b->symbol);
  wattron(b->win->window, COLOR_PAIR(b->win->color));
}

void ClearBird(BIRD *b) { mvwprintw(b->win->window, b->y, b->x, " "); }

void MoveBird(BIRD *b) {
  ClearBird(b);

  int at_x_boundary = (b->x <= BORDER - 1) || (b->x >= b->win->cols - BORDER);
  int at_y_boundary = (b->y <= BORDER - 1) || (b->y >= b->win->rows - BORDER);

  if (at_x_boundary) {
    if (b->x <= BORDER) {
      b->dx = 1;
    } else if (b->x >= b->win->cols - BORDER - 1) {
      b->dx = -1;
    }
  } else {
    int new_x = b->x + b->dx * b->speed;

    if (new_x <= BORDER) {
      b->x = BORDER;
      b->dx = 1;
    } else if (new_x >= b->win->cols - BORDER - 1) {
      b->x = b->win->cols - BORDER - 1;
      b->dx = -1;
    } else {
      b->x = new_x;
    }
  }

  if (at_y_boundary) {
    if (b->y <= BORDER) {
      b->dy = 1;
    } else if (b->y >= b->win->rows - BORDER - 1) {
      b->dy = -1;
    }
  } else {
    int new_y = b->y + b->dy * b->speed;

    if (new_y <= BORDER) {
      b->y = BORDER;
      b->dy = 1;
    } else if (new_y >= b->win->rows - BORDER - 1) {
      b->y = b->win->rows - BORDER - 1;
      b->dy = -1;
    } else {
      b->y = new_y;
    }
  }

  DrawBird(b);
}

void ManualMoveBird(BIRD *b, int ch) {
  ClearBird(b);
  if (ch == UP) {
    b->dy = -1;
    b->dx = 0;
    b->y--;
  }
  if (ch == DOWN) {
    b->dy = 1;
    b->dx = 0;
    b->y++;
  }
  if (ch == LEFT) {
    b->dx = -1;
    b->dy = 0;
    b->x--;
  }
  if (ch == RIGHT) {
    b->dx = 1;
    b->dy = 0;
    b->x++;
  }

  if (b->x < BORDER)
    b->x = BORDER;
  if (b->x > b->win->cols - BORDER - 1)
    b->x = b->win->cols - BORDER - 1;
  if (b->y < BORDER)
    b->y = BORDER;
  if (b->y > b->win->rows - BORDER - 1)
    b->y = b->win->rows - BORDER - 1;
  DrawBird(b);
}

void ShowStatus(WIN *W, BIRD *bird, int timeLeft) {
  wattron(W->window, COLOR_PAIR(W->color));
  box(W->window, 0, 0);
  mvwprintw(W->window, 1, 2, "Time: %d | Score: %d | HP: %d | [Q] Quit",
            timeLeft, bird->points, bird->health);
  wrefresh(W->window);
}

// --- STARS LOGIC ---

void SpawnStar(WIN *w, STAR *stars, int maxStars, int chance) {
  if ((rand() % 100) >= chance)
    return;

  for (int i = 0; i < maxStars; i++) {
    if (!stars[i].alive) {
      stars[i].alive = 1;
      stars[i].x = (rand() % (w->cols - 2 * BORDER)) + BORDER;
      stars[i].y = BORDER;
      break;
    }
  }
}

void UpdateStars(WIN *w, STAR *stars, int maxStars) {
  wattron(w->window, COLOR_PAIR(PLAY_COLOR));
  for (int i = 0; i < maxStars; i++) {
    if (!stars[i].alive)
      continue;
    mvwprintw(w->window, stars[i].y, stars[i].x, " "); // Erase
    stars[i].y++;
    if (stars[i].y >= w->rows - BORDER) {
      stars[i].alive = 0;
      continue;
    }
    mvwprintw(w->window, stars[i].y, stars[i].x, "*"); // Draw
  }
  wattroff(w->window, COLOR_PAIR(PLAY_COLOR));
}

// --- HUNTERS LOGIC (RESTORED ORIGINAL SPRITES) ---

void SpawnHunter(WIN *w, HUNTER *hunters, BIRD *bird, CONFIG cfg) {
  if ((rand() % 100) >= cfg.hunter_spawn_chance)
    return;

  for (int i = 0; i < cfg.hunter_quota; i++) {
    if (!hunters[i].alive) {
      hunters[i].alive = 1;
      hunters[i].damage = cfg.hunter_damage;
      hunters[i].bounces = cfg.hunter_bounces;

      int shape = rand() % 5;
      switch (shape) {
      case 0:
        hunters[i].width = 1;
        hunters[i].height = 2;
        break;
      case 1:
        hunters[i].width = 2;
        hunters[i].height = 1;
        break;
      case 2:
        hunters[i].width = 1;
        hunters[i].height = 3;
        break;
      case 3:
        hunters[i].width = 3;
        hunters[i].height = 1;
        break;
      case 4:
        hunters[i].width = 2;
        hunters[i].height = 2;
        break;
      }

      // Pick only LEFT (0) or RIGHT (1)
      int side = rand() % 2;
      int startX, startY;
      int maxY = w->rows - BORDER - hunters[i].height; // Ensure height fits

      if (side == 0) { // Left
        startX = BORDER;
        startY = (rand() % (maxY - BORDER)) + BORDER;
      } else { // Right
        startX =
            w->cols - BORDER - hunters[i].width; // Ensure width fits inside
        startY = (rand() % (maxY - BORDER)) + BORDER;
      }

      // Set positions
      hunters[i].fx = (float)startX;
      hunters[i].fy = (float)startY;
      hunters[i].x = startX;
      hunters[i].y = startY;

      // Calculate vector to bird
      float dx = (float)bird->x - hunters[i].fx;
      float dy = (float)bird->y - hunters[i].fy;
      float dist = sqrtf(dx * dx + dy * dy);

      if (dist > 0) {
        hunters[i].vx = (dx / dist) * cfg.hunter_speed;
        hunters[i].vy = (dy / dist) * cfg.hunter_speed;
      } else {
        hunters[i].vx = 0;
        hunters[i].vy = 0;
      }
      break;
    }
  }
}

void UpdateHunters(WIN *w, HUNTER *hunters, int maxHunters) {
  wattron(w->window, COLOR_PAIR(HUNTER_COLOR));
  for (int i = 0; i < maxHunters; i++) {
    if (!hunters[i].alive)
      continue;

    // Erase
    for (int r = 0; r < hunters[i].height; r++)
      for (int c = 0; c < hunters[i].width; c++)
        mvwprintw(w->window, hunters[i].y + r, hunters[i].x + c, " ");

    // Move Float
    hunters[i].fx += hunters[i].vx;
    hunters[i].fy += hunters[i].vy;

    // Bounce Check
    int hit = 0;
    if (hunters[i].fx <= BORDER) {
      hunters[i].fx = BORDER + 0.1f;
      hunters[i].vx = -hunters[i].vx;
      hit = 1;
    } else if (hunters[i].fx + hunters[i].width >= w->cols - BORDER) {
      hunters[i].fx = w->cols - BORDER - hunters[i].width - 0.1f;
      hunters[i].vx = -hunters[i].vx;
      hit = 1;
    }

    if (hunters[i].fy <= BORDER) {
      hunters[i].fy = BORDER + 0.1f;
      hunters[i].vy = -hunters[i].vy;
      hit = 1;
    } else if (hunters[i].fy + hunters[i].height >= w->rows - BORDER) {
      hunters[i].fy = w->rows - BORDER - hunters[i].height + 0.1f;
      hunters[i].vy = -hunters[i].vy;
      hit = 1;
    }

    if (hit) {
      hunters[i].bounces--;
      if (hunters[i].bounces < 0) {
        hunters[i].alive = 0;
        continue;
      }
    }

    // Update Int Pos
    hunters[i].x = (int)hunters[i].fx;
    hunters[i].y = (int)hunters[i].fy;

    // Draw
    char disp = (hunters[i].bounces > 9) ? '9' : hunters[i].bounces + '0';
    for (int r = 0; r < hunters[i].height; r++)
      for (int c = 0; c < hunters[i].width; c++)
        if (hunters[i].y + r < w->rows && hunters[i].x + c < w->cols)
          mvwprintw(w->window, hunters[i].y + r, hunters[i].x + c, "%c", disp);
  }
  wattroff(w->window, COLOR_PAIR(HUNTER_COLOR));
}

// --- COLLISIONS ---
int IsHit(int bx, int by, HUNTER *h) {
  return (bx >= h->x && bx < h->x + h->width && by >= h->y &&
          by < h->y + h->height);
}

void CheckCollisionsStar(BIRD *b, STAR *stars, CONFIG cfg) {
  // Check Stars
  for (int i = 0; i < cfg.star_quota; i++) {
    if (stars[i].alive) {
      int hit = 0;
      if ((b->x == stars[i].x && b->y == stars[i].y) ||
          (b->dy == -1 && b->x == stars[i].x && b->y == stars[i].y - 1)) {
        hit = 1;
      }
      if (hit) {
        stars[i].alive = 0;
        b->points += cfg.points_per_star;
        mvwprintw(b->win->window, stars[i].y, stars[i].x, " ");
      }
    }
  }
}

void CheckCollisionsHunter(BIRD *b, HUNTER *hunters, CONFIG cfg) {
  for (int i = 0; i < cfg.hunter_quota; i++) {
    if (hunters[i].alive) {

      int prev_bird_x = b->x - (b->dx * b->speed);
      int prev_bird_y = b->y - (b->dy * b->speed);

      int hit = IsHit(b->x, b->y, &hunters[i]);

      if (!hit) {
        hit = IsHit(prev_bird_x, prev_bird_y, &hunters[i]);
      }

      if (hit) {
        hunters[i].alive = 0;
        b->health -= cfg.hunter_damage;

        // Erase
        for (int r = 0; r < hunters[i].height; r++)
          for (int c = 0; c < hunters[i].width; c++)
            mvwprintw(b->win->window, hunters[i].y + r, hunters[i].x + c, " ");

        flash();
      }
    }
  }
}

void EndGame(WIN *W, int score, int survived) {
  CleanWin(W, 1);
  if (survived)
    mvwprintw(W->window, 1, 2, "TIME UP! Final Score: %d", score);
  else
    mvwprintw(W->window, 1, 2, "DIED! Final Score: %d", score);
  wrefresh(W->window);
  sleep(3);
}

// --- MAIN LOOP ---

void MainLoop(WIN *playwin, WIN *statwin, BIRD *bird, CONFIG cfg) {
  // DYNAMIC ALLOCATION
  STAR *stars = (STAR *)malloc(sizeof(STAR) * cfg.star_quota);
  memset(stars, 0, sizeof(STAR) * cfg.star_quota);

  HUNTER *hunters = (HUNTER *)malloc(sizeof(HUNTER) * cfg.hunter_quota);
  memset(hunters, 0, sizeof(HUNTER) * cfg.hunter_quota);

  int ch;
  int maxTime = cfg.game_time;
  time_t startTime = time(NULL);

  while (1) {
    ch = wgetch(statwin->window);
    int elapsed = (int)(time(NULL) - startTime);
    int timeLeft = maxTime - elapsed;
    if (timeLeft < 0)
      timeLeft = 0;

    if (ch == QUIT)
      break;

    if (ch == UP || ch == LEFT || ch == DOWN || ch == RIGHT)
      ManualMoveBird(bird, ch);
    else
      MoveBird(bird);

    SpawnStar(playwin, stars, cfg.star_quota, cfg.star_spawn_chance);
    UpdateStars(playwin, stars, cfg.star_quota);

    SpawnHunter(playwin, hunters, bird, cfg);
    UpdateHunters(playwin, hunters, cfg.hunter_quota);

    CheckCollisionsStar(bird, stars, cfg);
    CheckCollisionsHunter(bird, hunters, cfg);

    DrawBird(bird);
    ShowStatus(statwin, bird, timeLeft);

    if (bird->health <= 0 || timeLeft == 0)
      break;

    wrefresh(playwin->window);
    flushinp();
    usleep(FRAME_TIME * 1000);
  }

  free(stars);
  free(hunters);
}

int main() {
  srand(time(NULL));

  // 1. Load Settings
  CONFIG cfg;
  LoadConfig(&cfg);

  // 2. Start Ncurses
  WINDOW *mainwin = Start();

  WIN *playwin =
      InitWin(mainwin, ROWS, COLS, OFFY, OFFX, PLAY_COLOR, BORDER, 0);
  WIN *statwin =
      InitWin(mainwin, 3, COLS, ROWS + OFFY, OFFX, STAT_COLOR, BORDER, 0);

  // 3. Init Bird
  BIRD *bird = InitBird(playwin, COLS / 2, ROWS / 2, cfg.start_health);

  DrawBird(bird);
  ShowStatus(statwin, bird, cfg.game_time);
  wrefresh(playwin->window);

  // 4. Run Game
  MainLoop(playwin, statwin, bird, cfg);

  EndGame(statwin, bird->points, bird->health > 0);

  delwin(playwin->window);
  delwin(statwin->window);
  delwin(mainwin);
  endwin();

  free(bird);
  free(playwin);
  free(statwin);

  return EXIT_SUCCESS;
}
