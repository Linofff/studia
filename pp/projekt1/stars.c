#include "stars.h"
#include "hunters.h"

void SpawnStar(BIRD *bird, WIN *w, STAR *stars, CONFIG cfg, const int rows,
               const int cols, char occupancyMap[rows][cols]) {
  // spawning conditions
  if (bird->is_in_albatross_taxi || cfg.game_time_elapsed <= 1 ||
      bird->albatross_in_cooldown > 4)
    return;
  if ((rand() % 100) >= cfg.level.star_spawn_chance)
    return;

  for (int i = 0; i < cfg.level.star_max; i++) {
    if (stars[i].alive)
      continue;
    stars[i].symbol = '*';
    stars[i].ticks_lived = 0;

    bool findingspot = 1;
    int attempts = 0;

    // findint a free row withouot a star and fog in it to prevent star to star
    // collision
    while (findingspot && attempts < 50) {
      const int randomizedX = (rand() % (w->cols - 2 * BORDER)) + BORDER;
      bool occupied = 0;
      for (int y = 0; y < rows; y++) {
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

void RedrawHunter(STAR *star, HUNTER *hunters, CONFIG *cfg, const int rows,
                  const int cols, char occupancyMap[rows][cols], WIN *playwin) {
  for (int i = 0; i < cfg->level.hunter_max; i++) {
    for (int r = 0; r < hunters[i].height; r++)
      for (int c = 0; c < hunters[i].width; c++) {
        if ((hunters[i].x + c) == star->x && (hunters[i].y + r) == star->y) {
          DrawHunter(playwin, &hunters[i], rows, cols, occupancyMap);
        }
      }
  }
}

void EraseStar(WIN *w, const int rows, const int cols,
               char occupancyMap[rows][cols], STAR *star) {
  mvwprintw(w->window, star->y, star->x, " ");
  occupancyMap[star->y][star->x] = ' ';
}

void DrawStar(WIN *w, const int rows, const int cols,
              char occupancyMap[rows][cols], STAR *star, CONFIG cfg) {
  wattron(w->window, COLOR_PAIR(STAR_COLOR));

  if (star->ticks_lived % 8 < 4)
    star->symbol = '*';
  else
    star->symbol = '+';

  if (star->y > rows / 2) {
    if (cfg.framecounter % 10 < 9)
      mvwprintw(w->window, star->y, star->x, "%c", star->symbol);

  } else {
    mvwprintw(w->window, star->y, star->x, "%c", star->symbol);
  }
  occupancyMap[star->y][star->x] = 's';

  wattroff(w->window, COLOR_PAIR(STAR_COLOR));
}

void UpdateStars(WIN *w, STAR *stars, const int rows, const int cols,
                 char occupancyMap[rows][cols], BIRD *bird, CONFIG *cfg,
                 HUNTER *hunters) {

  for (int i = 0; i < cfg->level.star_max; i++) {
    if (!stars[i].alive) {
      if (occupancyMap[stars[i].y][stars[i].x] == 's') {
        EraseStar(w, rows, cols, occupancyMap, &stars[i]);
      }
      continue;
    }

    stars[i].ticks_lived++;

    EraseStar(w, rows, cols, occupancyMap, &stars[i]);
    stars[i].fy += stars[i].speed;
    stars[i].y = (int)(stars[i].fy);

    if (stars[i].y >= w->rows - BORDER) {
      stars[i].alive = 0;
      continue;
    }

    // looking for a collision before drawing the star in the new position
    if (occupancyMap[stars[i].y][stars[i].x] == 'b') {
      stars[i].alive = 0;
      bird->points++;
      continue;
    }

    if (occupancyMap[stars[i].y][stars[i].x] == 'h') {
      stars[i].alive = 0;
      EraseStar(w, rows, cols, occupancyMap, &stars[i]);
      RedrawHunter(&stars[i], hunters, cfg, rows, cols, occupancyMap, w);
      continue;
    }

    if (occupancyMap[stars[i].y][stars[i].x] == '#') {
      stars[i].alive = 0;
      continue;
    }

    DrawStar(w, rows, cols, occupancyMap, &stars[i], *cfg);
  }
}
