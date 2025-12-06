#ifndef FOG_H
#define FOG_H

#include "game_defs.h"

void InitFog(CONFIG *cfg, int cols);
void UpdateFog(CONFIG *cfg, BIRD *bird);
void DrawFog(WIN *playwin, CONFIG *cfg, int rows, int cols,
             char occupancyMap[rows][cols]);

#endif
