#ifndef FOG_H
#define FOG_H

#include "game_defs.h"

void InitFog(CONFIG *cfg);
void UpdateFog(CONFIG *cfg, WIN *playwin, BIRD *bird);
void DrawFog(WIN *playwin, CONFIG *cfg, char occupancyMap[ROWS][COLS]);

#endif
