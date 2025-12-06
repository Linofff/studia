#ifndef ALBATROSS_H
#define ALBATROSS_H

#include "game_defs.h"

void MainLoopAlbatrossCheck(WIN *playwin, BIRD *bird, const int rows,
                            const int cols);
void AlbatrossTaxi(HUNTER *hunters, STAR *stars, BIRD *bird, CONFIG *cfg,
                   const int rows, const int cols,
                   char occupancyMap[rows][cols], WIN *playwin);
void OutOfAlbatrossTaxi(BIRD *bird);

#endif
