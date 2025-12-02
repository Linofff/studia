#ifndef RANKING_H
#define RANKING_H

#include "game_defs.h"

void GameOver(WIN *playwin, WIN *statwin);
void Ranking(WIN *playwin, WIN *statwin, BIRD *bird, CONFIG cfg);
int CalculateScore(BIRD *bird, CONFIG *cfg);
int compare_scores(const void *a, const void *b);
void UpdateAndSaveRanking(const char *player_name, int score,
                          int difficulty_level);
void ShowRanking(WIN *w);

#endif
