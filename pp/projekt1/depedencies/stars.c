#include "./../headers/stars.h"

void SpawnStar(BIRD *bird, WIN *w, STAR *stars, CONFIG cfg,
               char occupancyMap[ROWS][COLS]) {
  if (!bird->is_in_albatross_taxi) {
    if ((rand() % 100) >= cfg.star_spawn_chance)
      return;

    for (int i = 0; i < cfg.star_max; i++) {
      if (!stars[i].alive) {
        bool findingspot = 1;
        int attempts = 0;
        while (findingspot && attempts < 50) {
          int randomizedX = (rand() % (w->cols - 2 * BORDER)) + BORDER;
          bool occupied = 0;
          for (int y = 0; y < ROWS; y++) {
            if (occupancyMap[y][randomizedX] == 's') {
              occupied = 1;
              break;
            }
          }
          if (!occupied) {
            stars[i].x = randomizedX;
            findingspot = 0;
          }
          attempts++;
        }
        if (findingspot) {
          break;
        }
        stars[i].y = BORDER;

        stars[i].speed = (float)rand() / (float)RAND_MAX;
        stars[i].fy = (float)BORDER;

        stars[i].alive = 1;
        occupancyMap[stars[i].y][stars[i].x] = 's';
        break;
      }
    }
  }
}

void UpdateStars(WIN *w, STAR *stars, int maxStars,
                 char occupancyMap[ROWS][COLS], BIRD *bird) {
  wattron(w->window, COLOR_PAIR(PLAY_COLOR));

  for (int i = 0; i < maxStars; i++) {
    if (!stars[i].alive) {
      if (occupancyMap[stars[i].y][stars[i].x] == 's') {
        mvwprintw(w->window, stars[i].y, stars[i].x, " ");
        occupancyMap[stars[i].y][stars[i].x] = ' ';
      }
      continue;
    }

    mvwprintw(w->window, stars[i].y, stars[i].x, " ");
    occupancyMap[stars[i].y][stars[i].x] = ' ';

    stars[i].fy += stars[i].speed;
    stars[i].y = (int)(stars[i].fy);

    if (stars[i].y >= w->rows - BORDER) {
      stars[i].alive = 0;
      continue;
    }

    if (occupancyMap[stars[i].y][stars[i].x] == 'b') {
      stars[i].alive = 0;
      bird->points++;
      continue;
    }

    if (occupancyMap[stars[i].y][stars[i].x] == 'h') {
      stars[i].alive = 0;
      mvwprintw(w->window, stars[i].y, stars[i].x, " ");
      occupancyMap[stars[i].y][stars[i].x] = ' ';
      continue;
    }

    mvwprintw(w->window, stars[i].y, stars[i].x, "*");
    occupancyMap[stars[i].y][stars[i].x] = 's';
  }

  wattroff(w->window, COLOR_PAIR(PLAY_COLOR));
}
