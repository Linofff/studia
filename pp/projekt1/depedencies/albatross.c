#include "../headers/albatross.h"

void AlbatrossTaxi(HUNTER *hunters, STAR *stars, BIRD *bird, CONFIG *cfg) {
  for (int i = 0; i < cfg->hunter_max; i++) {
    hunters[i].alive = 0;
    for (int r = 0; r < hunters[i].height; r++)
      for (int c = 0; c < hunters[i].width; c++)
        mvwprintw(bird->win->window, hunters[i].y + r, hunters[i].x + c, " ");
  }

  for (int i = 0; i < cfg->star_max; i++) {
    stars[i].alive = 0;
    if (stars[i].y != 0)
      mvwprintw(bird->win->window, stars[i].y, stars[i].x, " ");
  }

  bird->is_in_albatros_taxi = 1;
  mvwprintw(bird->win->window, bird->y, bird->x, " ");
  bird->x = COLS / 2;
  bird->y = ROWS / 2 - 1;
}

void OutOfAlbatrossTaxi(HUNTER *hunters, STAR *stars, BIRD *bird, CONFIG *cfg) {
  bird->is_in_albatros_taxi = 0;
}
