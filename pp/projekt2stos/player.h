#ifndef PLAYER_H
#define PLAYER_H

#include "main.h"

void LoadPlayerAssets(PlayerType *player);

void ResetPlayer(PlayerType *player);

void PushInput(InputBuffer *buf, InputType input, double time);
int check(InputBuffer *buf, const InputType *seq, int len, double currentTime);
int CheckCombos(InputBuffer *buf, double currentTime);
InputType ResolveInput(const Uint8 *keyState, int *prevKeys);

void UpdateAnimation(int *frame, double *timer, int maxFrames, double speed,
                     double delta);
void HandleAnimationState(PlayerType *player, const Uint8 *keyState,
                          double delta);

int CheckCollision(double x1, double y1, int w1, int h1, double x2, double y2,
                   int w2, int h2);
void GetAttackHitbox(PlayerType *player, double *hitX, double *hitY, int *hitW,
                     int *hitH);
void ApplyAttackDamage(PlayerType *player, EnemyType *enemies, double hX,
                       double hY, int hW, int hH);
void ProcessAttack(PlayerType *player, EnemyType *enemies);
void HandleCombatState(PlayerType *player, EnemyType *enemies, InputType input,
                       double worldTime);
int HandleDeathLogic(PlayerType *player, double delta);
void HandlePhysics(PlayerType *player, const Uint8 *keyState, double delta);

void HandleTimersAndBounds(PlayerType *player, double delta);

void UpdatePlayer(PlayerType *player, EnemyType *enemies, double delta,
                  double worldTime);

#endif
