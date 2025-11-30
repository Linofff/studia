#ifndef ALBATROSS_H
#define ALBATROSS_H

#include "game_defs.h"

void AlbatrossTaxi(HUNTER *hunters, STAR *stars, BIRD *bird, CONFIG *cfg,
                   char occupancyMap[ROWS][COLS]);
void OutOfAlbatrossTaxi(HUNTER *hunters, STAR *stars, BIRD *bird, CONFIG *cfg);

#endif
