#ifndef WINDOWMENGAGING_H
#define WINDOWMENGAGING_H

#include "game_defs.h"

WINDOW *Start();
void StartScreen(WIN *playwin, bool *running);
void CleanWin(WIN *W, int bo);
WIN *InitWin(WINDOW *parent, int rows, int cols, int y, int x, int color,
             int bo, int delay);
void ShowStatus(WIN *W, BIRD *bird, CONFIG cfg);
void DebugDrawMap(WIN *w, char occupancyMap[ROWS][COLS]);

#endif
