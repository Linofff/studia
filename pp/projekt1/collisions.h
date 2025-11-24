#ifndef COLLISIONS_H
#define COLLISIONS_H

#include "game_defs.h"

void CheckCollisionsStar(BIRD *b, STAR *stars, CONFIG cfg);
int IsHit(int bx, int by, HUNTER *h);
void CheckCollisionsHunter(BIRD *b, HUNTER *hunters, CONFIG cfg);

#endif
