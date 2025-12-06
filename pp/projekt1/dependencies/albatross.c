#include "../headers/albatross.h"
#include "../headers/hunters.h"

void MainLoopAlbatrossCheck(WIN *playwin, BIRD *bird, const int rows,
                            const int cols) {
  if (bird->is_in_albatross_taxi) {
    mvwprintw(playwin->window, (rows / 4), (cols / 2) - 8, "You are in a taxi");

    bird->was_in_taxi = 1;
  } else if (bird->was_in_taxi) {

    mvwprintw(playwin->window, (rows / 4), (cols / 2) - 8,
              "                   ");
    bird->was_in_taxi = 0;
  }
}

void AlbatrossTaxi(HUNTER *hunters, STAR *stars, BIRD *bird, CONFIG *cfg,
                   const int rows, const int cols,
                   char occupancyMap[rows][cols], WIN *playwin) {
  if (!bird->albatross_in_cooldown) {
    bird->albatross_in_cooldown = 5;
    bird->albatross_out_cooldown = 1;
    for (int i = 0; i < cfg->level.hunter_max; i++) {
      EraseHunter(playwin, &hunters[i], rows, cols, occupancyMap);
      hunters[i].alive = 0;
    }

    for (int i = 0; i < cfg->level.star_max; i++) {
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

void OutOfAlbatrossTaxi(BIRD *bird) {
  if (!bird->albatross_out_cooldown) {
    bird->is_in_albatross_taxi = 0;
  }
}
