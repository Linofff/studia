#ifndef HUNTERS_H
#define HUNTERS_H

#include "game_defs.h"

void ChangeColorHunter(HUNTER *hunter, CONFIG cfg);
void ChoseShape(HUNTER *hunter, CONFIG cfg);
void CalculateDirections(const BIRD *bird, HUNTER *hunter, const CONFIG cfg);
void InitAndPlaceHunter(WIN *w, HUNTER *h, BIRD *bird, CONFIG cfg,
                        const int rows, const int cols,
                        char occupancyMap[rows][cols]);
void SpawnHunter(WIN *w, HUNTER *hunters, BIRD *bird, CONFIG cfg,
                 const int rows, const int cols, char occupancyMap[rows][cols]);
int BorderCheck(WIN *w, HUNTER *hunter);
void EraseHunter(WIN *w, HUNTER *hunter, const int rows, const int cols,
                 char occupancyMap[rows][cols]);
void DrawHunter(WIN *w, HUNTER *hunter, const int rows, const int cols,
                char occupancyMap[rows][cols]);
void UpdateHunters(WIN *w, HUNTER *hunters, int maxHunters, BIRD *bird,
                   const CONFIG cfg, const int rows, const int cols,
                   char occupancyMap[rows][cols], STAR *stars);
void UpdateDashingHunters(WIN *w, HUNTER *hunters, BIRD *bird, CONFIG cfg);
void HunterTriggerDash(HUNTER *hunter, BIRD *bird, CONFIG cfg);
void IncreaseHunterSpeed(HUNTER *h, float multiplier);

#endif
