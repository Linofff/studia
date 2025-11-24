#include "stars.h"

void SpawnStar(WIN *w, STAR *stars, CONFIG cfg) {
  if ((rand() % 100) >= cfg.star_spawn_chance)
    return;

  for (int i = 0; i < cfg.star_max; i++) {
    if (!stars[i].alive) {
      stars[i].speed = cfg.star_speed;
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
    stars[i].y += stars[i].speed;
    if (stars[i].y >= w->rows - BORDER) {
      stars[i].alive = 0;
      continue;
    }
    mvwprintw(w->window, stars[i].y, stars[i].x, "*"); // Draw
  }
  wattroff(w->window, COLOR_PAIR(PLAY_COLOR));
}
