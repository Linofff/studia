#include "./../headers/stars.h"

void SpawnStar(BIRD *bird, WIN *w, STAR *stars, CONFIG cfg,
               char occupancyMap[ROWS][COLS]) {
  if (!bird->is_in_albatross_taxi) {
    if ((rand() % 100) >= cfg.star_spawn_chance)
      return;

    for (int i = 0; i < cfg.star_max; i++) {
      if (!stars[i].alive) {
        stars[i].alive = 1;
        stars[i].speed = (float)rand() / (float)RAND_MAX;

        stars[i].x = (rand() % (w->cols - 2 * BORDER)) + BORDER;
        stars[i].y = BORDER;

        stars[i].fy = (float)BORDER;

        occupancyMap[stars[i].y][stars[i].x] = 's';
        break;
      }
    }
  }
}

void UpdateStars(WIN *w, STAR *stars, int maxStars,
                 char occupancyMap[ROWS][COLS]) {
  wattron(w->window, COLOR_PAIR(PLAY_COLOR));

  for (int i = 0; i < maxStars; i++) {
    if (!stars[i].alive)
      continue;

    mvwprintw(w->window, stars[i].y, stars[i].x, " ");
    occupancyMap[stars[i].y][stars[i].x] = ' ';

    stars[i].fy += stars[i].speed;
    stars[i].y = (int)(stars[i].fy);

    if (stars[i].y >= w->rows - BORDER) {
      stars[i].alive = 0;
      continue;
    }

    mvwprintw(w->window, stars[i].y, stars[i].x, "*");
    occupancyMap[stars[i].y][stars[i].x] = 's';
  }

  wattroff(w->window, COLOR_PAIR(PLAY_COLOR));
}
