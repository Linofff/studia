#include "./../headers/windowmanaging.h"

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
  init_pair(BIRD_COLOR1, COLOR_GREEN, COLOR_BLACK);
  init_pair(BIRD_COLOR2, COLOR_YELLOW, COLOR_BLACK);
  init_pair(BIRD_COLOR3, COLOR_RED, COLOR_BLACK);
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

void ShowStatus(WIN *W, BIRD *bird, CONFIG cfg) {
  wbkgd(W->window, COLOR_PAIR(W->color));
  werase(W->window);
  box(W->window, 0, 0);
  mvwprintw(W->window, 1, 2,
            "Time: %d %d %d | Score: %d | HP: %d | Goal: %d | [Q] Quit | [T/R] "
            "Albatros",
            cfg.game_time_left, bird->albatross_in_cooldown, cfg.game_speed,
            bird->points, bird->health, cfg.star_quota);
  wrefresh(W->window);
}

void EndGame(WIN *W, int score, int survived, CONFIG cfg) {
  CleanWin(W, 1);
  if (survived && score >= cfg.star_quota)
    mvwprintw(W->window, 1, 2, "You have won in time of %d seconds!",
              cfg.game_time_elapsed);
  else if (survived)
    mvwprintw(W->window, 1, 2, "TIME UP! Final Score: %d", score);
  else
    mvwprintw(W->window, 1, 2, "DIED! Final Score: %d", score);
  wrefresh(W->window);
  sleep(3);
}
