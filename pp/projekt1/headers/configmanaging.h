#ifndef CONFIGMENAGING_H
#define CONFIGMENAGING_H

#include "game_defs.h"

void InitMap(int rows, int cols, char occupancyMap[rows][cols]);
void LoadConfig(CONFIG *c);
void UpdateConfig(CONFIG *cfg);
void UpdateTimeState(BIRD *bird, time_t *start_timestamp, CONFIG *cfg);
void AssignConfigToInput(CONFIG *c, const char *section, const char *key,
                         const char *val_str, int *active_template_id);
void SanitizeLine(char *line);
void StarsConfigLoad(CONFIG *c, const char *key, float value);
void HunterConfigLoad(CONFIG *c, const char *key, float value,
                      int *active_template_id);
void GameConfigLoad(CONFIG *c, const char *key, float value);
void BirdConfigLoad(CONFIG *c, const char *key, float value);
void LoadTemplates(CONFIG *c, const char *key, float value,
                   int *active_template_id);
void PlayerConfigLoad(CONFIG *c, const char *key, const char *val_str);

#endif
