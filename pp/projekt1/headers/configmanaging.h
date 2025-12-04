#ifndef CONFIGMENAGING_H
#define CONFIGMENAGING_H

#include "game_defs.h"

void InitMap(char occupancyMap[ROWS][COLS]);
void LoadConfig(CONFIG *c);
void UpdateConfig(CONFIG *cfg);
void UpdateTimeState(BIRD *bird, time_t *start_timestamp, CONFIG *cfg);
void AssignConfigToInput(CONFIG *c, const char *section, const char *key,
                         float value, int *active_template_id);
void SanitizeLine(char *line);
void StarsConfigLoad(CONFIG *c, const char *key, float value);
void HunterConfigLoad(CONFIG *c, const char *key, float value,
                      int *active_template_id);
void GameConfigLoad(CONFIG *c, const char *key, float value);
void BirdConfigLoad(CONFIG *c, const char *key, float value);
void LoadTemplates(CONFIG *c, const char *key, float value,
                   int *active_template_id);

#endif
