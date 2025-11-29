#ifndef BIRD_H
#define BIRD_H

#include "albatross.h"
#include "game_defs.h"

void ChangeColor(BIRD *bird, CONFIG cfg);
void MoveToCenter(BIRD *bird);
BIRD *InitBird(WIN *w, int x, int y, int startHealth,
               char occupancyMap[ROWS][COLS]);
void DrawBird(BIRD *b);
void ClearBird(BIRD *b);
void MoveBird(BIRD *b, char occupancyMap[ROWS][COLS]);
void ManualMoveBird(BIRD *b, int ch, char occupancyMap[ROWS][COLS]);

#endif
