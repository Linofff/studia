#include "./../headers/stars.h"
#include "./../headers/hunters.h"

void SpawnStar(BIRD *bird, WIN *w, STAR *stars, CONFIG cfg,
               char occupancyMap[ROWS][COLS]) {
  if (!bird->is_in_albatross_taxi && cfg.game_time_elapsed > 1 &&
      bird->albatross_in_cooldown < 4) {
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
            if (occupancyMap[y][randomizedX] == 's' ||
                occupancyMap[y][randomizedX] == '#') {
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

void RedrawHunter(STAR *star, HUNTER *hunters, CONFIG *cfg,
                  char occupancyMap[ROWS][COLS], WIN *playwin) {
  for (int i = 0; i < cfg->hunter_max; i++) {
    for (int r = 0; r < hunters[i].height; r++)
      for (int c = 0; c < hunters[i].width; c++) {
        if ((hunters[i].x + c) == star->x && (hunters[i].y + r) == star->y) {
          DrawHunter(playwin, &hunters[i], occupancyMap);
        }
      }
  }
}

void EraseStar(WIN *w, char occupancyMap[ROWS][COLS], STAR *star) {
  mvwprintw(w->window, star->y, star->x, " ");
  occupancyMap[star->y][star->x] = ' ';
}

void DrawStar(WIN *w, char occupancyMap[ROWS][COLS], STAR *star, CONFIG cfg) {
  wattron(w->window, COLOR_PAIR(STAR_COLOR));

  if (star->y > ROWS / 2) {
    if (cfg.framecounter % 4 < 3) {
      mvwprintw(w->window, star->y, star->x, "*");
    }
    occupancyMap[star->y][star->x] = 's';
  } else {
    mvwprintw(w->window, star->y, star->x, "*");
    occupancyMap[star->y][star->x] = 's';
  }

  wattroff(w->window, COLOR_PAIR(STAR_COLOR));
}

void UpdateStars(WIN *w, STAR *stars, char occupancyMap[ROWS][COLS], BIRD *bird,
                 CONFIG *cfg, HUNTER *hunters) {

  for (int i = 0; i < cfg->star_max; i++) {
    if (!stars[i].alive) {
      if (occupancyMap[stars[i].y][stars[i].x] == 's') {
        EraseStar(w, occupancyMap, &stars[i]);
      }
      continue;
    }

    EraseStar(w, occupancyMap, &stars[i]);
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
      RedrawHunter(&stars[i], hunters, cfg, occupancyMap, w);
      continue;
    }

    if (occupancyMap[stars[i].y][stars[i].x] == '#') {
      stars[i].alive = 0;
      continue;
    }

    DrawStar(w, occupancyMap, &stars[i], *cfg);
  }
}
