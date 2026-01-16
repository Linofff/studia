#ifndef ENEMY_H
#define ENEMY_H

#include "main.h"

EnemyType *InitEnemies();
void SpawnEnemies(EnemyType *enemies, EnemyAssets *assets);
void MoveEnemy(EnemyType *enemies, PlayerType *player, double delta);
void LoadEnemyAssets(EnemyAssets *assets);

#endif
