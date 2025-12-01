#ifndef STARS_H
#define STARS_H

#include "game_defs.h"

void SpawnStar(BIRD *bird, WIN *w, STAR *stars, CONFIG cfg,
               char occupancyMap[ROWS][COLS]);
void UpdateStars(WIN *w, STAR *stars, int maxStars,
                 char occupancyMap[ROWS][COLS], BIRD *bird);

#endif
