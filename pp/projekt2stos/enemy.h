#ifndef ENEMY_H
#define ENEMY_H

#include "main.h"

void TintSequence(SDL_Surface **src, SDL_Surface **dst, int count, Uint8 r,
                  Uint8 g, Uint8 b);
void GenerateRedAssets(EnemyAssets *assets, SDL_Surface **walk_R,
                       SDL_Surface **walk_L, SDL_Surface **hit_R,
                       SDL_Surface **hit_L, SDL_Surface **atk_R,
                       SDL_Surface **atk_L);
void GenerateGreenAssets(EnemyAssets *assets, SDL_Surface **walk_R,
                         SDL_Surface **walk_L, SDL_Surface **hit_R,
                         SDL_Surface **hit_L, SDL_Surface **atk_R,
                         SDL_Surface **atk_L, SDL_Surface **dash_R,
                         SDL_Surface **dash_L);
void FreeRawAssets(SDL_Surface **walk_R, SDL_Surface **walk_L,
                   SDL_Surface **hit_R, SDL_Surface **hit_L,
                   SDL_Surface **atk_R, SDL_Surface **atk_L,
                   SDL_Surface **dash_R, SDL_Surface **dash_L);
void LoadEnemyAssets(EnemyAssets *assets);
EnemyType *InitEnemies();
void SetupType0(EnemyType *enemy, EnemyAssets *assets);
void SetupType1(EnemyType *enemy, EnemyAssets *assets);
void SpawnSingleEnemy(EnemyType *enemy, EnemyAssets *assets);
void SpawnEnemies(EnemyType *enemies, EnemyAssets *assets);
void UpdateEnemyTimers(EnemyType *enemy, double delta);
void UpdateEnemyAnimation(EnemyType *enemy, double delta);
void HandleEnemyAttack(EnemyType *enemy, PlayerType *player, double distance);
void MoveChaser(EnemyType *enemy, double dx, double dy, double dist,
                double delta);
void MoveCharger(EnemyType *enemy, PlayerType *player, double dx, double dy,
                 double delta);
void EnforceBounds(EnemyType *enemy);
void MoveEnemy(EnemyType *enemies, PlayerType *player, double delta);
#endif
