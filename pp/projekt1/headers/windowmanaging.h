#ifndef WINDOWMENGAGING_H
#define WINDOWMENGAGING_H

#include "game_defs.h"

WINDOW *Start();
void StartScreen(WIN *playwin, const int rows, const int cols, bool *running);
void CleanWin(WIN *W, int bo);
WIN *InitWin(WINDOW *parent, const int rows, const int cols, int y, int x,
             int color, int bo, int delay);
void ShowStatus(WIN *W, BIRD *bird, CONFIG cfg);

#endif
