#ifndef BIRD_H
#define BIRD_H

#include "albatross.h"
#include "game_defs.h"

void ChangeColorBird(BIRD *bird, CONFIG cfg);
void BirdBorderCheck(int at_x_boundary, int at_y_boundary, BIRD *b, int rows,
                     int cols, char occupancyMap[rows][cols]);
void MoveToCenter(BIRD *bird, int rows, int cols);
BIRD *InitBird(WIN *w, int x, int y, int startHealth, int rows, int cols,
               char occupancyMap[rows][cols]);
void DrawBird(BIRD *b, int rows, int cols, char occupancyMap[rows][cols]);
void ClearBird(BIRD *b, int rows, int cols, char occupancyMap[rows][cols]);
void MoveBird(BIRD *b, int rows, int cols, char occupancyMap[rows][cols],
              STAR *stars, HUNTER *hunters, CONFIG *cfg, WIN *playwin);
void ChangeDirectionBird(BIRD *b, int ch, int rows, int cols,
                         char occupancyMap[rows][cols], STAR *stars,
                         HUNTER *hunters, CONFIG *cfg, WIN *playwin);
void FindWhichStar(BIRD *b, STAR *stars, CONFIG *cfg);

#endif
