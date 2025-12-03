#ifndef FOG_H
#define FOG_H

#include "game_defs.h"

void InitFog(CONFIG *cfg);
void UpdateFog(CONFIG *cfg, WIN *playwin);
void DrawFog(WIN *playwin, CONFIG *cfg);
bool CheckFogCollision(BIRD *bird, CONFIG *cfg);

#endif
