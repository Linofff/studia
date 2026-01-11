#ifndef ENEMY_H
#define ENEMY_H

#include "main.h"

EnemyType *InitEnemies();
void SpawnEnemies(EnemyType *enemies);
void MoveEnemy(EnemyType *enemies, PlayerType *player, double delta);

#endif
