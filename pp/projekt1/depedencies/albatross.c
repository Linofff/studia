#include "../headers/albatross.h"

void AlbatrossTaxi(HUNTER *hunters, STAR *stars, BIRD *bird, CONFIG *cfg) {
  if (!bird->albatross_in_cooldown) {
    bird->albatross_out_cooldown = 1;
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

    if (!bird->is_in_albatross_taxi)
      bird->points -= 2;
    bird->is_in_albatross_taxi = 1;
    mvwprintw(bird->win->window, bird->y, bird->x, " ");
  }
}

void OutOfAlbatrossTaxi(HUNTER *hunters, STAR *stars, BIRD *bird, CONFIG *cfg) {
  if (!bird->albatross_out_cooldown) {
    bird->is_in_albatross_taxi = 0;
    bird->albatross_in_cooldown = 5;
  }
}
