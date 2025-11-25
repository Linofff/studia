#ifndef WINDOWMENGAGING_H
#define WINDOWMENGAGING_H

#include "game_defs.h"

WINDOW *Start();
void CleanWin(WIN *W, int bo);
WIN *InitWin(WINDOW *parent, int rows, int cols, int y, int x, int color,
             int bo, int delay);
void ShowStatus(WIN *W, BIRD *bird, int timeLeft);
void EndGame(WIN *W, int score, int survived);

#endif
