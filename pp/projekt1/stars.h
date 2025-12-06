#ifndef STARS_H
#define STARS_H

#include "game_defs.h"

void SpawnStar(BIRD *bird, WIN *w, STAR *stars, CONFIG cfg, const int rows,
               const int cols, char occupancyMap[rows][cols]);
void RedrawHunter(STAR *star, HUNTER *hunters, CONFIG *cfg, const int rows,
                  const int cols, char occupancyMap[rows][cols], WIN *playwin);
void EraseStar(WIN *w, const int rows, const int cols,
               char occupancyMap[rows][cols], STAR *star);
void UpdateStars(WIN *w, STAR *stars, const int rows, const int cols,
                 char occupancyMap[rows][cols], BIRD *bird, CONFIG *cfg,
                 HUNTER *hunters);
void DrawStar(WIN *w, const int rows, const int cols,
              char occupancyMap[rows][cols], STAR *star);

#endif
