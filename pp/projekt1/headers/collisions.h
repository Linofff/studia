#ifndef COLLISIONS_H
#define COLLISIONS_H

#include "game_defs.h"

void CheckCollisionsStar(BIRD *b, STAR *stars, CONFIG cfg,
                         char occupancyMap[COLS][ROWS]);
int IsHit(int bx, int by, HUNTER *h, char occupancyMap[COLS][ROWS]);
void CheckCollisionsHunter(BIRD *b, HUNTER *hunters, CONFIG cfg,
                           char occupancyMap[COLS][ROWS]);

#endif
