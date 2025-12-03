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
  const char *title[] = {"  ____                         ",
                         " / ___| __ _ _ __ ___   ___    ",
                         "| |  _ / _` | '_ ` _ \\ / _ \\   ",
                         "| |_| | (_| | | | | | |  __/   ",
                         " \\____|\\__,_|_| |_| |_|\\___|   ",
                         "    ___                 _        ",
                         "   / _ \\__   _____ _ __| |       ",
                         "  | | | \\ \\ / / _ \\ '__| |       ",
                         "  | |_| |\\ V /  __/ |  |_|       ",
                         "   \\___/  \\_/ \\___|_|  (_)       "};
  int title_lines = 10;
  int title_width = 31;

  int start_y = (playwin->rows / 2) - (title_lines / 2) - 1;
  int start_x = (playwin->cols / 2) - (title_width / 2) + 2;

  for (int i = 0; i < title_lines; i++) {
    mvwprintw(playwin->window, start_y + i, start_x, "%s", title[i]);
    wrefresh(playwin->window);
    napms(100);
  }
  sleep(1);
}

void StartScreen(WIN *playwin, bool *running) {
  box(playwin->window, 0, 0);
  mvwprintw(playwin->window, (ROWS / 4), (3 * COLS / 8),
            "Press SPACEBAR to START");
  wrefresh(playwin->window);

  int startCh;
  while ((startCh = wgetch(playwin->window)) != SPACEBAR) {
    if (startCh == QUIT) {
      running = 0;
      break;
    }
    usleep(50000);
  }

  mvwprintw(playwin->window, (ROWS / 4), (3 * COLS / 8),
            "                       ");
  wrefresh(playwin->window);
}

void DebugDrawMap(WIN *w, char occupancyMap[ROWS][COLS]) {
  for (int y = 0; y < ROWS; y++) {
    for (int x = 0; x < COLS; x++) {

      char cell = occupancyMap[y][x];

      if (cell == ' ' || cell == 0) {
        wattron(w->window, A_DIM);
        mvwaddch(w->window, y, x, '.');
        wattroff(w->window, A_DIM);
      } else if (isprint(cell)) {
        wattron(w->window, A_BOLD);
        mvwaddch(w->window, y, x, cell);
        wattroff(w->window, A_BOLD);
      } else {
        mvwaddch(w->window, y, x, '?');
      }
    }
  }
  wrefresh(w->window);
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
            "Time left: %d | Albatross cooldown: %d | Game speed: %d",
            cfg.game_time_left, bird->albatross_in_cooldown, cfg.game_speed);
  mvwprintw(W->window, 2, 2,
            "Score: %d | HP: %d | Goal: %d | [Q] Quit | [T/R] Albatros",
            bird->points, bird->health, cfg.star_quota);
  wrefresh(W->window);
}
