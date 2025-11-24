#include "bird.h"
#include "collisions.h"
#include "game_defs.h"
#include "hunters.h"
#include "stars.h"

// --- FILE LOADING ---

void LoadConfig(CONFIG *c) {
  FILE *f = fopen("settings.txt", "r");
  char k[64];
  while (fscanf(f, "%s", k) > 0) {
    !strcmp(k, "STAR_MAX") && fscanf(f, "%d", &c->star_max);
    !strcmp(k, "STAR_QUOTA") && fscanf(f, "%d", &c->star_quota);
    !strcmp(k, "GAME_TIME") && fscanf(f, "%d", &c->game_time);
    !strcmp(k, "STAR_SPAWN_CHANCE") && fscanf(f, "%d", &c->star_spawn_chance);
    !strcmp(k, "STAR_SPEED") && fscanf(f, "%f", &c->star_speed);
    !strcmp(k, "HUNTER_MAX") && fscanf(f, "%d", &c->initial_hunter_max);
    !strcmp(k, "HUNTER_SPAWN_CHANCE") &&
        fscanf(f, "%d", &c->hunter_spawn_chance);
    !strcmp(k, "HUNTER_DAMAGE") && fscanf(f, "%d", &c->hunter_damage);
    !strcmp(k, "HUNTER_SPEED") && fscanf(f, "%f", &c->hunter_speed);
    !strcmp(k, "HUNTER_BOUNCES") && fscanf(f, "%d", &c->initial_hunter_bounces);
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

void ShowStatus(WIN *W, BIRD *bird, int timeLeft) {
  wattron(W->window, COLOR_PAIR(W->color));
  box(W->window, 0, 0);
  mvwprintw(W->window, 1, 2, "Time: %d | Score: %d | HP: %d | [Q] Quit",
            timeLeft, bird->points, bird->health);
  wrefresh(W->window);
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

void UpdateConfig(CONFIG *cfg, int startTime) {
  cfg->hunter_bounces =
      cfg->initial_hunter_bounces + ((time(NULL) - startTime) / 10);
  cfg->hunter_max = cfg->initial_hunter_max + ((time(NULL) - startTime) / 10);
}

void UpdateGameWorld(WIN *playwin, STAR *stars, HUNTER *hunters, BIRD *bird,
                     CONFIG *cfg, int startTime) {
  // Handle Stars
  SpawnStar(playwin, stars, *cfg);
  UpdateStars(playwin, stars, cfg->star_max);
  CheckCollisionsStar(bird, stars, *cfg);

  // Handle Hunters
  SpawnHunter(playwin, hunters, bird, *cfg);
  UpdateHunters(playwin, hunters, cfg->hunter_max);
  CheckCollisionsHunter(bird, hunters, *cfg);
  UpdateConfig(cfg, startTime);
}

void MainLoop(WIN *playwin, WIN *statwin, BIRD *bird, CONFIG *cfg) {
  // calloc allocates and zeroes memory in one step
  time_t startTime = time(NULL);
  int ch, timeLeft;

  UpdateConfig(cfg, startTime);

  STAR *stars = (STAR *)calloc(cfg->star_max, sizeof(STAR));
  HUNTER *hunters = (HUNTER *)calloc(cfg->hunter_max, sizeof(HUNTER));

  while (1) {
    ch = wgetch(statwin->window);
    timeLeft = cfg->game_time - (int)(time(NULL) - startTime);
    if (timeLeft < 0)
      timeLeft = 0;

    // Exit conditions
    if (ch == QUIT)
      break;

    // Bird Movement
    if (ch == UP || ch == LEFT || ch == DOWN || ch == RIGHT)
      ManualMoveBird(bird, ch);
    else
      MoveBird(bird);

    // Process Stars and Hunters
    UpdateGameWorld(playwin, stars, hunters, bird, cfg, startTime);

    // Rendering
    DrawBird(bird);
    ShowStatus(statwin, bird, timeLeft);
    wrefresh(playwin->window);

    // Game Over Check
    if (bird->health <= 0 || timeLeft == 0)
      break;

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

  // UpdateConfig(&cfg, 2);

  // 4. Run Game
  MainLoop(playwin, statwin, bird, &cfg);

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
