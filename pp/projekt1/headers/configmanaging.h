#ifndef CONFIGMENAGING_H
#define CONFIGMENAGING_H

#include "game_defs.h"

void LoadConfig(CONFIG *c);
void UpdateConfig(CONFIG *cfg, int startTime);
void UpdateTimeState(BIRD *bird, time_t *start_timestamp, CONFIG *cfg);

#endif
