#ifndef CONFIGMENAGING_H
#define CONFIGMENAGING_H

#include "game_defs.h"

void LoadConfig(CONFIG *c);
void UpdateConfig(CONFIG *cfg, int startTime);
void UpdateTimeState(BIRD *bird, time_t *start_timestamp, CONFIG *cfg);
void AssignConfigToInput(CONFIG *c, const char *section, const char *key,
                         float value);
void SanitizeLine(char *line);
void StarsConfigLoad(CONFIG *c, const char *key, float value);
void HunterConfigLoad(CONFIG *c, const char *key, float value);
void GameConfigLoad(CONFIG *c, const char *key, float value);
void BirdConfigLoad(CONFIG *c, const char *key, float value);

#endif
