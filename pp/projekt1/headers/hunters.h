#ifndef HUNTERS_H
#define HUNTERS_H

#include "game_defs.h"

void ChoseShape(HUNTER *hunter, CONFIG cfg);
void CalculateDirections(BIRD *bird, HUNTER *hunter, CONFIG cfg);
void SpawnHunter(WIN *w, HUNTER *hunters, BIRD *bird, CONFIG cfg,
                 char occupancyMap[ROWS][COLS]);
int BorderCheck(WIN *w, HUNTER *hunter);
void EraseHunter(WIN *w, HUNTER *hunter, char occupancyMap[ROWS][COLS]);
void DrawHunter(WIN *w, HUNTER *hunter, char occupancyMap[ROWS][COLS]);
void UpdateHunters(WIN *w, HUNTER *hunters, int maxHunters, BIRD *bird,
                   const CONFIG cfg, char occupancyMap[ROWS][COLS]);
void HunterDash(WIN *w, HUNTER *hunters, BIRD *bird, CONFIG cfg);
void HunterSleep(HUNTER *hunter, BIRD *bird, CONFIG cfg);
void IncreaseHunterSpeed(HUNTER *h, float multiplier);

#endif
