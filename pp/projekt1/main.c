#include <math.h>    // Added for sqrt calculation
#include <ncurses.h> // Text-based UI library
#include <stdio.h>   // Standard input/output (printf, fprintf)
#include <stdlib.h>  // Standard library (malloc, free, exit)
#include <string.h>  // String operations (memset, strcpy)
#include <time.h>
#include <unistd.h> // Unix standard (usleep for timing)

#define STARTER_TIME 45
#define START_HEALTH 3

#define QUIT 'q'    // Key to quit the game
#define REVERSE ' ' // Key to reverse bird direction (SPACE)
#define NOKEY ERR   // ncurses constant for "no key pressed"

#define FRAME_TIME 100 // Milliseconds per frame (100ms = 0.1 sec)
#define BIRD_SPEED 1   // Bird moves 1 cell per frame

#define MAIN_COLOR 1   // Main window color
#define STAT_COLOR 2   // Status bar color
#define PLAY_COLOR 3   // Play area color
#define BIRD_COLOR 4   // Bird color
#define HUNTER_COLOR 5 // New Hunter color

#define MAX_STARS 10
#define STAR_SPAWN_CHANCE 20 // 10% chance to spawn a star each frame
#define POINTS_PER_STAR 1    // Points awarded for collecting a star

#define MAX_HUNTERS 3
#define HUNTER_SPAWN_CHANCE 10
#define HUNTER_DAMAGE 1
#define HUNTER_SPEED 0.9f
#define HUNTER_BOUNCES 5 // Number of bounces before disappearing

#define UP 'w'
#define DOWN 's'
#define LEFT 'a'
#define RIGHT 'd'

#define BORDER 1 // Border width (in characters)
#define ROWS 20  // Play window height (rows)
#define COLS 80  // Play window width (columns)
#define OFFY 2   // Y offset from top of screen
#define OFFX 5   // X offset from left of screen

typedef struct {
  WINDOW *window; // ncurses window pointer
  int x, y;       // position on screen
  int rows, cols; // size of window
  int color;      // color scheme
} WIN;

typedef struct {
  WIN *win;    // window containing the bird
  int x, y;    // current position
  int dx, dy;  // velocity direction vector
  int speed;   // movement speed
  char symbol; // character to display
  int color;   // color scheme
  int points;
  int health;
} BIRD;

typedef struct {
  int x, y;  // position
  int alive; // 1=active, 0=free slot
} STAR;

typedef struct {
  float fx, fy;      // Floating point position for precise movement
  float vx, vy;      // Velocity vector
  int x, y;          // Integer position for rendering
  int width, height; // Sprite dimensions
  int alive;
  int damage;
  int bounces; // Remaining bounces
} HUNTER;

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
  int i, j;

  // Set window color
  wattron(W->window, COLOR_PAIR(W->color));

  // Draw border if requested
  if (bo)
    box(W->window, 0, 0);

  // Fill window with spaces (clearing it)
  for (i = bo; i < W->rows - bo; i++)
    for (j = bo; j < W->cols - bo; j++)
      mvwprintw(W->window, i, j, " ");

  // Refresh to show changes
  wrefresh(W->window);
}

WIN *InitWin(WINDOW *parent, int rows, int cols, int y, int x, int color,
             int bo, int delay) {
  // Allocate memory for WIN structure
  WIN *W = (WIN *)malloc(sizeof(WIN));

  // Store window properties
  W->x = x;
  W->y = y;
  W->rows = rows;
  W->cols = cols;
  W->color = color;

  // Create ncurses subwindow
  W->window = subwin(parent, rows, cols, y, x);

  // Clear the window
  CleanWin(W, bo);

  // Set input mode: delay==0 means non-blocking (for real-time games)
  if (delay == 0)
    nodelay(W->window, TRUE);

  // Display the window
  wrefresh(W->window);

  return W;
}

BIRD *InitBird(WIN *w, int x, int y, int dx, int dy) {
  // Allocate memory for BIRD structure
  BIRD *b = (BIRD *)malloc(sizeof(BIRD));

  // Set bird properties
  b->win = w;            // window containing bird
  b->x = x;              // initial x position
  b->y = y;              // initial y position
  b->dx = dx;            // direction: -1=left, 1=right
  b->dy = dy;            // direction: -1=up, 1=down
  b->speed = BIRD_SPEED; // movement speed
  b->symbol = '8';       // display character
  b->color = BIRD_COLOR; // color scheme
  b->points = 0;
  b->health = START_HEALTH;
  return b;
}

void DrawBird(BIRD *b) {
  // Set bird color
  wattron(b->win->window, COLOR_PAIR(b->color));

  // Draw bird symbol at current position
  mvwprintw(b->win->window, b->y, b->x, "%c", b->symbol);

  // Restore window color
  wattron(b->win->window, COLOR_PAIR(b->win->color));
}

void ClearBird(BIRD *b) {
  // Overwrite bird with a space character
  mvwprintw(b->win->window, b->y, b->x, " ");
}

void MoveBird(BIRD *b) {
  // Step 1: Erase bird from old position
  ClearBird(b);

  // Step 2: Check if bird is already at boundary
  int at_x_boundary = (b->x <= BORDER - 1) || (b->x >= b->win->cols - BORDER);
  int at_y_boundary = (b->y <= BORDER - 1) || (b->y >= b->win->rows - BORDER);

  // Step 3: Handle horizontal movement
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

  // Step 4: Handle vertical movement
  if (at_y_boundary) {
    if (b->y <= BORDER) {
      b->dy = 1;
    } else if (b->y >= b->win->rows - BORDER - 1) {
      b->y = b->win->rows - BORDER - 1;
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

  // Step 5: Draw bird at new position
  DrawBird(b);
}

void ManualMoveBird(BIRD *b, int ch) {
  ClearBird(b);

  if (ch == UP) {
    b->dx = 0;
    b->dy = -1;
    b->y--;
  }
  if (ch == DOWN) {
    b->dx = 0;
    b->dy = 1;
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
  // Set status bar color
  wattron(W->window, COLOR_PAIR(W->color));

  // Draw border around status bar
  box(W->window, 0, 0);

  // Use timeLeft instead of time
  mvwprintw(W->window, 1, 2,
            "Time left: %d | Score: %d | Health: %d | [Q]=Quit", timeLeft,
            bird->points, bird->health);

  // Update display
  wrefresh(W->window);
}

void SpawnStar(WIN *w, STAR stars[]) {
  for (int i = 0; i < MAX_STARS; i++) {
    if (!stars[i].alive) {
      stars[i].alive = 1;
      stars[i].x = (rand() % (w->cols - 2 * BORDER)) + BORDER;
      stars[i].y = BORDER;
      break;
    }
  }
}

void UpdateStars(WIN *w, STAR stars[]) {
  wattron(w->window, COLOR_PAIR(PLAY_COLOR));

  for (int i = 0; i < MAX_STARS; i++) {
    if (!stars[i].alive)
      continue;

    mvwprintw(w->window, stars[i].y, stars[i].x, " ");
    stars[i].y++;

    if (stars[i].y >= w->rows - BORDER) {
      stars[i].alive = 0;
      continue;
    }
    mvwprintw(w->window, stars[i].y, stars[i].x, "*");
  }
  wattroff(w->window, COLOR_PAIR(PLAY_COLOR));
}

// --- HUNTER FUNCTIONS ---

void SpawnHunter(WIN *w, HUNTER hunters[], BIRD *bird) {
  for (int i = 0; i < MAX_HUNTERS; i++) {
    if (!hunters[i].alive) {
      hunters[i].alive = 1;
      hunters[i].damage = HUNTER_DAMAGE;
      hunters[i].bounces = HUNTER_BOUNCES;

      // --- DETERMINE SHAPE ---
      int shape = rand() % 5;
      switch (shape) {
      case 0:
        hunters[i].width = 1;
        hunters[i].height = 2;
        break; // 1x2
      case 1:
        hunters[i].width = 2;
        hunters[i].height = 1;
        break; // 2x1
      case 2:
        hunters[i].width = 1;
        hunters[i].height = 3;
        break; // 1x3
      case 3:
        hunters[i].width = 3;
        hunters[i].height = 1;
        break; // 3x1
      case 4:
        hunters[i].width = 2;
        hunters[i].height = 2;
        break; // 2x2
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

      // Set initial floating point position
      hunters[i].fx = (float)startX;
      hunters[i].fy = (float)startY;
      hunters[i].x = startX;
      hunters[i].y = startY;

      // Calculate vector to bird (aim for bird's center)
      float dx = (float)bird->x - hunters[i].fx;
      float dy = (float)bird->y - hunters[i].fy;

      // Calculate Euclidean distance
      float dist = sqrtf(dx * dx + dy * dy);

      // Normalize and scale by speed
      if (dist > 0) {
        hunters[i].vx = (dx / dist) * HUNTER_SPEED;
        hunters[i].vy = (dy / dist) * HUNTER_SPEED;
      } else {
        hunters[i].vx = 0;
        hunters[i].vy = 0;
      }

      break;
    }
  }
}

void UpdateHunters(WIN *w, HUNTER hunters[]) {
  wattron(w->window, COLOR_PAIR(HUNTER_COLOR));

  for (int i = 0; i < MAX_HUNTERS; i++) {
    if (!hunters[i].alive)
      continue;

    // Erase sprite from current integer position (loop through dimensions)
    for (int r = 0; r < hunters[i].height; r++) {
      for (int c = 0; c < hunters[i].width; c++) {
        mvwprintw(w->window, hunters[i].y + r, hunters[i].x + c, " ");
      }
    }

    // Update float position based on constant vector
    hunters[i].fx += hunters[i].vx;
    hunters[i].fy += hunters[i].vy;

    // --- CHECK BOUNCES / COLLISIONS WITH WALLS (With Dimensions) ---

    // Check Vertical Walls (Left/Right)
    int hit_x = 0;
    // Left wall check
    if (hunters[i].fx <= BORDER) {
      hunters[i].fx = BORDER + 0.1f;
      hunters[i].vx = -hunters[i].vx;
      hit_x = 1;
    }
    // Right wall check (check far edge: x + width)
    else if (hunters[i].fx + hunters[i].width >= w->cols - BORDER) {
      hunters[i].fx = w->cols - BORDER - hunters[i].width - 0.1f;
      hunters[i].vx = -hunters[i].vx;
      hit_x = 1;
    }

    // Check Horizontal Walls (Top/Bottom)
    int hit_y = 0;
    // Top wall check
    if (hunters[i].fy <= BORDER) {
      hunters[i].fy = BORDER + 0.1f;
      hunters[i].vy = -hunters[i].vy;
      hit_y = 1;
    }
    // Bottom wall check (check bottom edge: y + height)
    else if (hunters[i].fy + hunters[i].height >= w->rows - BORDER) {
      hunters[i].fy = w->rows - BORDER - hunters[i].height + 0.1f;
      hunters[i].vy = -hunters[i].vy;
      hit_y = 1;
    }

    // Handle bounce logic
    if (hit_x || hit_y) {
      hunters[i].bounces--;
      if (hunters[i].bounces < 0) {
        hunters[i].alive = 0;
        continue; // Stop processing this hunter
      }
    }

    // Update integer position for rendering
    hunters[i].x = (int)hunters[i].fx;
    hunters[i].y = (int)hunters[i].fy;

    // Render the sprite (loop through dimensions)
    // Display bounce count in every cell of the sprite
    char displayChar =
        (hunters[i].bounces > 9) ? '9' : (hunters[i].bounces + '0');

    for (int r = 0; r < hunters[i].height; r++) {
      for (int c = 0; c < hunters[i].width; c++) {
        // Safety check to ensure we don't draw outside window (rare cases)
        if (hunters[i].y + r < w->rows && hunters[i].x + c < w->cols) {
          mvwprintw(w->window, hunters[i].y + r, hunters[i].x + c, "%c",
                    displayChar);
        }
      }
    }
  }
  wattroff(w->window, COLOR_PAIR(HUNTER_COLOR));
}

void CheckCollisions(BIRD *b, STAR stars[], HUNTER hunters[]) {
  for (int i = 0; i < MAX_STARS; i++) {
    if (stars[i].alive) {
      int hit = 0;

      if ((b->x == stars[i].x && b->y == stars[i].y) ||
          (b->dy == -1 && b->x == stars[i].x && b->y == stars[i].y - 1)) {
        hit = 1;
      }

      if (hit) {
        stars[i].alive = 0;
        b->points += POINTS_PER_STAR;
        mvwprintw(b->win->window, stars[i].y, stars[i].x, " ");
      }
    }
  }

  for (int i = 0; i < MAX_HUNTERS; i++) {
    if (hunters[i].alive) {
      int hit = 0;

      // Rectangle Collision Detection
      // Check if Bird (b->x, b->y) is inside the Hunter rectangle
      if (b->x >= hunters[i].x && b->x < hunters[i].x + hunters[i].width &&
          b->y >= hunters[i].y && b->y < hunters[i].y + hunters[i].height) {
        hit = 1;
      }

      if (hit) {
        hunters[i].alive = 0;
        b->health -= hunters[i].damage;

        // Erase the entire sprite
        for (int r = 0; r < hunters[i].height; r++) {
          for (int c = 0; c < hunters[i].width; c++) {
            mvwprintw(b->win->window, hunters[i].y + r, hunters[i].x + c, " ");
          }
        }

        flash();
      }
    }
  }
}

void EndGame(WIN *W, int score, int survived) {
  CleanWin(W, 1);
  if (survived) {
    mvwprintw(W->window, 1, 2, "TIME UP! Final Score: %d", score);
  } else {
    mvwprintw(W->window, 1, 2, "DIED! Final Score: %d", score);
  }
  wrefresh(W->window);
  sleep(3);
}

void MainLoop(WIN *playwin, WIN *statwin, BIRD *bird) {
  STAR stars[MAX_STARS];
  memset(stars, 0, sizeof(stars));

  // Initialize Hunters array
  HUNTER hunters[MAX_HUNTERS];
  memset(hunters, 0, sizeof(hunters));

  int ch;
  int maxTime = STARTER_TIME;
  time_t startTime = time(NULL);

  while (1) {
    ch = wgetch(statwin->window);

    int elapsed = (int)(time(NULL) - startTime);
    int timeLeft = maxTime - elapsed;

    if (timeLeft < 0)
      timeLeft = 0;

    if (ch == QUIT)
      break;

    // 1. Move Bird
    if (ch == UP || ch == LEFT || ch == DOWN || ch == RIGHT) {
      ManualMoveBird(bird, ch);
    } else {
      MoveBird(bird);
    }

    // 2. Spawn and Move Stars
    if ((rand() % 100) < STAR_SPAWN_CHANCE) {
      SpawnStar(playwin, stars);
    }
    UpdateStars(playwin, stars);

    // 3. Spawn and Move Hunters
    if ((rand() % 100) < HUNTER_SPAWN_CHANCE) {
      SpawnHunter(playwin, hunters, bird);
    }
    UpdateHunters(playwin, hunters);

    // 4. Check Collisions (Stars and Hunters)
    CheckCollisions(bird, stars, hunters);

    // 5. Redraw Bird (Must be last to appear on top)
    DrawBird(bird);

    // 6. Update status bar
    ShowStatus(statwin, bird, timeLeft);

    // 7. Check Game Over Conditions
    if (bird->health <= 0) {
      break; // Died
    }
    if (timeLeft == 0) {
      break; // Time up
    }

    wrefresh(playwin->window);
    flushinp();
    usleep(FRAME_TIME * 1000);
  }
}

int main() {
  srand(time(NULL));
  WINDOW *mainwin = Start();

  WIN *playwin =
      InitWin(mainwin, ROWS, COLS, OFFY, OFFX, PLAY_COLOR, BORDER, 0);

  WIN *statwin =
      InitWin(mainwin, 3, COLS, ROWS + OFFY, OFFX, STAT_COLOR, BORDER, 0);

  BIRD *bird = InitBird(playwin, COLS / 2, ROWS / 2, 1, 0);

  DrawBird(bird);
  ShowStatus(statwin, bird, STARTER_TIME);
  wrefresh(playwin->window);

  MainLoop(playwin, statwin, bird);

  // Pass whether bird survived (health > 0) to EndGame
  EndGame(statwin, bird->points, bird->health > 0);

  CleanWin(statwin, 1);
  wrefresh(statwin->window);
  // sleep(2);

  delwin(playwin->window);
  delwin(statwin->window);
  delwin(mainwin);

  endwin();
  // refresh(); // Not strictly necessary after endwin

  free(bird);
  free(playwin);
  free(statwin);

  return EXIT_SUCCESS;
}
