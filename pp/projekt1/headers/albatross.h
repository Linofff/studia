#ifndef ALBATROSS_H
#define ALBATROSS_H

#include "game_defs.h"

void MainLoopAlbatrossCheck(WIN *playwin, BIRD *bird, int rows, int cols);
void AlbatrossTaxi(HUNTER *hunters, STAR *stars, BIRD *bird, CONFIG *cfg,
                   int rows, int cols, char occupancyMap[rows][cols],
                   WIN *playwin);
void OutOfAlbatrossTaxi(HUNTER *hunters, STAR *stars, BIRD *bird, CONFIG *cfg);

#endif
