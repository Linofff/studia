#include "windowmanaging.h"

WINDOW *Start() {
  WINDOW *win;
  if ((win = initscr()) == NULL) {
    fprintf(stderr, "Error initialising ncurses.\n");
    exit(EXIT_FAILURE);
  }
  start_color();
  init_pair(MAIN_COLOR, COLOR_WHITE, COLOR_BLACK);
  init_pair(PLAY_COLOR, COLOR_CYAN, COLOR_BLACK);
  init_pair(STAT_COLOR, COLOR_WHITE, COLOR_BLACK);
  init_pair(HIGH_HP_BIRD, COLOR_GREEN, COLOR_BLACK);
  init_pair(MEDIUM_HP_BIRD, COLOR_YELLOW, COLOR_BLACK);
  init_pair(LOW_HP_BIRD, COLOR_RED, COLOR_BLACK);
  init_pair(HIGH_HP_HUNTER, COLOR_CYAN, COLOR_BLACK);
  init_pair(MEDIUM_HP_HUNTER, COLOR_MAGENTA, COLOR_BLACK);
  init_pair(LOW_HP_HUNTER, COLOR_RED, COLOR_BLACK);
  init_pair(STAR_COLOR, COLOR_WHITE, COLOR_BLACK);
  init_pair(FOG_COLOR_PAIR, COLOR_WHITE, COLOR_BLACK);

  noecho();
  curs_set(0);
  return win;
}

void GameOver(WIN *playwin, WIN *statwin) {
  CleanWin(playwin, 1);
  CleanWin(statwin, 1);
  const char *gameover[] = {"  ____                         ",
                            " / ___| __ _ _ __ ___   ___    ",
                            "| |  _ / _` | '_ ` _ \\ / _ \\   ",
                            "| |_| | (_| | | | | | |  __/   ",
                            " \\____|\\__,_|_| |_| |_|\\___|   ",
                            "    ___                 _        ",
                            "   / _ \\__   _____ _ __| |       ",
                            "  | | | \\ \\ / / _ \\ '__| |       ",
                            "  | |_| |\\ V /  __/ |  |_|       ",
                            "   \\___/  \\_/ \\___|_|  (_)       "};
  const int gameover_lines = 10;
  const int gameover_width = 31;

  const int start_y = (playwin->rows / 2) - (gameover_lines / 2) - 1;
  const int start_x = (playwin->cols / 2) - (gameover_width / 2) + 2;

  // desplaying gameover the gameover
  for (int i = 0; i < gameover_lines; i++) {
    mvwprintw(playwin->window, start_y + i, start_x, "%s", gameover[i]);
    wrefresh(playwin->window);
    napms(100);
  }
  sleep(1);
}

void StartScreen(WIN *playwin, const int rows, const int cols, bool *running) {
  box(playwin->window, 0, 0);
  mvwprintw(playwin->window, (rows / 4), (cols / 2) - 10,
            "Press SPACE to START");
  wrefresh(playwin->window);

  int startCh;
  while ((startCh = wgetch(playwin->window)) != SPACEBAR) {
    if (startCh == QUIT) {
      *running = 0;
      break;
    }
    usleep(50000);
  }

  werase(playwin->window);
  wrefresh(playwin->window);
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

WIN *InitWin(WINDOW *parent, const int rows, const int cols, int y, int x,
             int color, int bo, int delay) {
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
  mvwprintw(W->window, 1, 2, "Name: %s | Level: %d | Game speed: %d",
            cfg.player_name, cfg.level.number, cfg.game_speed);
  mvwprintw(W->window, 2, 2, "Time left: %d | HP: %d | Stars: %d | Goal: %d",
            cfg.game_time_left, bird->health, bird->points,
            cfg.level.star_quota);

  mvwprintw(W->window, 3, 2, "TAXI cooldown: %d | [Q] Quit | [T/R] TAXI",
            bird->albatross_in_cooldown);
  wrefresh(W->window);
}
