#ifndef BIRD_H
#define BIRD_H

#include "game_defs.h"

BIRD *InitBird(WIN *w, int x, int y, int startHealth);
void DrawBird(BIRD *b);
void ClearBird(BIRD *b);
void MoveBird(BIRD *b);
void ManualMoveBird(BIRD *b, int ch);

#endif
