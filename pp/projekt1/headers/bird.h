#ifndef BIRD_H
#define BIRD_H

#include "albatross.h"
#include "game_defs.h"

void ChangeColorBird(BIRD *bird, CONFIG cfg);
void BirdBorderCheck(int at_x_boundary, int at_y_boundary, BIRD *b);
void MoveToCenter(BIRD *bird);
BIRD *InitBird(WIN *w, int x, int y, int startHealth,
               char occupancyMap[ROWS][COLS]);
void DrawBird(BIRD *b, char occupancyMap[ROWS][COLS]);
void ClearBird(BIRD *b, char occupancyMap[ROWS][COLS]);
void MoveBird(BIRD *b, char occupancyMap[ROWS][COLS], STAR *stars,
              HUNTER *hunters, CONFIG *cfg, WIN *playwin);
void ChangeDirectionBird(BIRD *b, int ch, char occupancyMap[ROWS][COLS],
                         STAR *stars, HUNTER *hunters, CONFIG *cfg,
                         WIN *playwin);
void FindWhichStar(BIRD *b, STAR *stars, CONFIG *cfg);

#endif
