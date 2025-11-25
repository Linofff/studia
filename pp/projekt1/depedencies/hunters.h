#ifndef HUNTERS_H
#define HUNTERS_H

#include "game_defs.h"

void RandomizeShape(HUNTER *hunter);
void CalculateDirections(BIRD *bird, HUNTER *hunter, CONFIG cfg);
void SpawnHunter(WIN *w, HUNTER *hunters, BIRD *bird, CONFIG cfg);
int BorderCheck(WIN *w, HUNTER *hunter);
void ErasePrevHunter(WIN *w, HUNTER *hunter);
void DrawHunter(WIN *w, HUNTER *hunter);
void UpdateHunters(WIN *w, HUNTER *hunters, int maxHunters, BIRD *bird,
                   const CONFIG cfg);
void HunterDash(WIN *w, HUNTER *hunters, BIRD *bird, CONFIG cfg);
void HunterSleep(HUNTER *hunter, BIRD *bird, CONFIG cfg);
void IncreaseHunterSpeed(HUNTER *h, float multiplier);

#endif
