#include "./../headers/albatross.h"
#include "./../headers/hunters.h"

void MainLoopAlbatrossCheck(WIN *playwin, BIRD *bird) {
  if (bird->is_in_albatross_taxi) {
    mvwprintw(playwin->window, (ROWS / 4), (2 * COLS / 5), "You are in a taxi");

    bird->was_in_taxi = 1;
  } else if (bird->was_in_taxi) {

    mvwprintw(playwin->window, (ROWS / 4), (2 * COLS / 5),
              "                   ");
    bird->was_in_taxi = 0;
  }
}

void AlbatrossTaxi(HUNTER *hunters, STAR *stars, BIRD *bird, CONFIG *cfg,
                   char occupancyMap[ROWS][COLS], WIN *playwin) {
  if (!bird->albatross_in_cooldown) {
    bird->albatross_in_cooldown = 5;
    bird->albatross_out_cooldown = 1;
    for (int i = 0; i < cfg->levels[0].hunter_max; i++) {
      EraseHunter(playwin, &hunters[i], occupancyMap);
      hunters[i].alive = 0;
    }

    for (int i = 0; i < cfg->levels[0].star_max; i++) {
      stars[i].alive = 0;
      if (stars[i].y != 0) {
        mvwprintw(bird->win->window, stars[i].y, stars[i].x, " ");
        occupancyMap[stars[i].y][stars[i].x] = ' ';
      }
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
  }
}
