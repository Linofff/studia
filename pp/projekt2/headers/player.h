#ifndef PLAYER_H
#define PLAYER_H

#include "main.h"

void InitPlayer(PlayerType *player);
void UpdatePlayer(PlayerType *player, EnemyType *enemies, double delta,
                  double worldTime);
int CheckCollision(double x1, double y1, int w1, int h1, double x2, double y2,
                   int w2, int h2);

#endif
