#ifndef BIRD_H
#define BIRD_H

#include "albatross.h"
#include "game_defs.h"

void ChangeColorBird(BIRD *bird, CONFIG cfg);
void ChangeShape(BIRD *bird, const CONFIG cfg);
void BirdBorderCheck(const int at_x_boundary, const int at_y_boundary, BIRD *b,
                     const int rows, const int cols,
                     char occupancyMap[rows][cols]);
void MoveToCenter(BIRD *bird, int rows, int cols);
BIRD *InitBird(WIN *w, int x, int y, int startHealth, int rows, int cols,
               char occupancyMap[rows][cols]);
void DrawBird(const BIRD *b, const int rows, const int cols,
              char occupancyMap[rows][cols]);
void ClearBird(BIRD *b, int rows, int cols, char occupancyMap[rows][cols]);
void MoveBird(BIRD *b, const int rows, const int cols,
              char occupancyMap[rows][cols], STAR *stars, HUNTER *hunters,
              const CONFIG *cfg, WIN *playwin);
void ChangeDirectionBird(BIRD *b, int ch, int rows, int cols,
                         char occupancyMap[rows][cols], STAR *stars,
                         HUNTER *hunters, CONFIG *cfg, WIN *playwin);
void FindWhichStar(const BIRD *b, STAR *stars, const CONFIG *cfg);
void FindWhichHunter(const BIRD *b, HUNTER *hunters, const CONFIG *cfg,
                     const int rows, const int cols,
                     char occupancyMap[rows][cols], WIN *playwin);

#endif
