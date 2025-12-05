#include "./../headers/ranking.h"
#include "./../headers/windowmanaging.h"

void Ranking(WIN *playwin, WIN *statwin, BIRD *bird, CONFIG cfg, int cols) {
  char player_name[16] = "Player";
  echo();
  nodelay(playwin->window, FALSE);

  CleanWin(statwin, 1);
  mvwprintw(statwin->window, 1, (cols / 2) - 8, "Thanks for playing!");
  wrefresh(statwin->window);

  wclear(playwin->window);
  box(playwin->window, 0, 0);
  mvwprintw(playwin->window, 10, (cols / 2) - 20,
            "Enter your name for ranking: ");
  wrefresh(playwin->window);

  mvwgetnstr(playwin->window, 10, (cols / 2) + 9, player_name, 15);
  noecho();

  int final_score = CalculateScore(bird, &cfg);

  int diff_level = cfg.game_speed;

  UpdateAndSaveRanking(player_name, final_score, diff_level);

  ShowRanking(playwin);
}

int CalculateScore(BIRD *bird, CONFIG *cfg) {
  int base_score = 0;
  float difficulty = 1;

  difficulty += ((float)cfg->game_speed / 5);

  base_score += (bird->points * 100);
  base_score += (bird->health * 10);
  base_score -= (cfg->game_time_elapsed * 5);

  if (base_score * difficulty > 0)
    return (int)(base_score * difficulty);
  else
    return 0;
}

int compare_scores(const void *a, const void *b) {
  RankingTemplate *entryA = (RankingTemplate *)a;
  RankingTemplate *entryB = (RankingTemplate *)b;
  return (entryB->score - entryA->score);
}

void UpdateAndSaveRanking(const char *player_name, int score,
                          int difficulty_level) {
  RankingTemplate entries[SAVEABLE_PLAYERS];
  int count = 0;

  FILE *fp = fopen(RANKING_FILE_NAME, "r");
  if (fp != NULL) {
    while (count < SAVEABLE_PLAYERS &&
           fscanf(fp, "%15s %d %d", entries[count].name, &entries[count].score,
                  &entries[count].difficulty) == 3) {
      count++;
    }
    fclose(fp);
  }

  strncpy(entries[count].name, player_name, 15);
  entries[count].name[15] = '\0';
  entries[count].score = score;
  entries[count].difficulty = difficulty_level;
  count++;

  qsort(entries, count, sizeof(RankingTemplate), compare_scores);

  fp = fopen(RANKING_FILE_NAME, "w");
  if (fp == NULL) {
    perror("Error saving ranking");
    return;
  }

  for (int i = 0; i < count; i++) {
    fprintf(fp, "%s %d %d\n", entries[i].name, entries[i].score,
            entries[i].difficulty);
  }
  fclose(fp);
}

void ShowRanking(WIN *w) {
  wclear(w->window);
  box(w->window, 0, 0);

  mvwprintw(w->window, 1, (w->cols - 10) / 2, "HIGH SCORES");
  mvwhline(w->window, 2, 1, ACS_HLINE, w->cols - 2);

  mvwprintw(w->window, 3, 5, "NAME");
  mvwprintw(w->window, 3, 25, "SCORE");
  mvwprintw(w->window, 3, 40, "DIFFICULTY");

  FILE *fp = fopen(RANKING_FILE_NAME, "r");
  if (fp == NULL) {
    mvwprintw(w->window, 5, 5, "No rankings yet.");
  } else {
    int count = 0;
    char name[20];
    int score, diff;
    int row = 5;
    while (fscanf(fp, "%s %d %d", name, &score, &diff) == 3 && count < TOP_N) {
      mvwprintw(w->window, row, 5, "%-15s", name);
      mvwprintw(w->window, row, 25, "%d", score);

      char *diff_str = (diff > 3) ? "HARD" : "NORMAL";
      mvwprintw(w->window, row, 40, "%s", diff_str);

      count++;
      row++;
    }
    fclose(fp);
  }

  mvwprintw(w->window, w->rows - 2, 2, "Press SPACE to continue...");
  wrefresh(w->window);

  nodelay(w->window, FALSE);
  while (wgetch(w->window) != SPACEBAR)
    ;
  usleep(250000);
  nodelay(w->window, TRUE);
}
