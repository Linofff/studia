#ifndef STARS_H
#define STARS_H

#include "game_defs.h"

void SpawnStar(BIRD *bird, WIN *w, STAR *stars, CONFIG cfg, int rows, int cols,
               char occupancyMap[rows][cols]);
void RedrawHunter(STAR *star, HUNTER *hunters, CONFIG *cfg, int rows, int cols,
                  char occupancyMap[rows][cols], WIN *playwin);
void EraseStar(WIN *w, int rows, int cols, char occupancyMap[rows][cols],
               STAR *star);
void UpdateStars(WIN *w, STAR *stars, int rows, int cols,
                 char occupancyMap[rows][cols], BIRD *bird, CONFIG *cfg,
                 HUNTER *hunters);

#endif
