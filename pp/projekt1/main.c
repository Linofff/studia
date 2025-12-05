#include "./headers/albatross.h"
#include "./headers/bird.h"
#include "./headers/configmanaging.h"
#include "./headers/fog.h"
#include "./headers/game_defs.h"
#include "./headers/hunters.h"
#include "./headers/ranking.h"
#include "./headers/stars.h"
#include "./headers/windowmanaging.h"

void UpdateGameWorld(WIN *playwin, STAR *stars, HUNTER *hunters, BIRD *bird,
                     CONFIG *cfg, int startTime, int rows, int cols,
                     char occupancyMap[rows][cols]) {
  UpdateConfig(cfg);

  UpdateFog(cfg, playwin, bird, cols);

  SpawnStar(bird, playwin, stars, *cfg, rows, cols, occupancyMap);
  UpdateStars(playwin, stars, rows, cols, occupancyMap, bird, cfg, hunters);

  SpawnHunter(playwin, hunters, bird, *cfg, rows, cols, occupancyMap);
  UpdateHunters(playwin, hunters, cfg->level.hunter_max, bird, *cfg, rows, cols,
                occupancyMap, stars);
  DrawFog(playwin, cfg, rows, cols, occupancyMap);
}

void ResolveChar(WIN *playwin, WIN *statwin, char ch, int rows, int cols,
                 char occupancyMap[rows][cols], BIRD *bird, bool *running,
                 HUNTER *hunters, STAR *stars, CONFIG *cfg) {
  if (ch == QUIT)
    *running = 0;

  if (ch == UP || ch == LEFT || ch == DOWN || ch == RIGHT)
    ChangeDirectionBird(bird, ch, rows, cols, occupancyMap, stars, hunters, cfg,
                        playwin);
  else
    MoveBird(bird, rows, cols, occupancyMap, stars, hunters, cfg, playwin);

  if (ch == TAXI_IN)
    AlbatrossTaxi(hunters, stars, bird, cfg, rows, cols, occupancyMap, playwin);

  if (ch == TAXI_OUT)
    OutOfAlbatrossTaxi(hunters, stars, bird, cfg);

  if (ch == FASTER && cfg->game_speed < 4) {
    cfg->frame_time /= 1.5;
    cfg->game_speed++;
  } else if (ch == SLOWER && cfg->game_speed > 0) {
    cfg->frame_time *= 1.5;
    cfg->game_speed--;
  }
}

void AllocateMemory(CONFIG *cfg, STAR **stars, HUNTER **hunters) {
  *stars = (STAR *)malloc(cfg->level.star_max * sizeof(STAR));
  int maxNumberOfHunters = cfg->level.initial_hunter_max +
                           (cfg->game_time_start / TIME_ENTITY_MULTI) + 2;
  *hunters = (HUNTER *)malloc(maxNumberOfHunters * sizeof(HUNTER));
}

void MainLoop(WIN *playwin, WIN *statwin, BIRD *bird, CONFIG *cfg, int rows,
              int cols, char occupancyMap[rows][cols]) {
  time_t startTime = time(NULL);
  int ch;
  bool running = 1;

  int timeLeft = cfg->game_time_start;
  cfg->game_time_left = cfg->game_time_start;

  STAR *stars = NULL;
  HUNTER *hunters = NULL;

  UpdateConfig(cfg);
  InitFog(cfg, cols);

  AllocateMemory(cfg, &stars, &hunters);

  StartScreen(playwin, rows, cols, &running);

  while (running) {

    box(playwin->window, 0, 0);

    UpdateTimeState(bird, &startTime, cfg);

    ch = wgetch(statwin->window);
    ResolveChar(playwin, statwin, ch, rows, cols, occupancyMap, bird, &running,
                hunters, stars, cfg);

    MainLoopAlbatrossCheck(playwin, bird, rows, cols);

    UpdateGameWorld(playwin, stars, hunters, bird, cfg, (int)startTime, rows,
                    cols, occupancyMap);

    ShowStatus(statwin, bird, *cfg);
    wrefresh(playwin->window);

    if (bird->health <= 0 || cfg->game_time_left <= 0)
      running = 0;

    if (bird->points >= cfg->level.star_quota)
      running = 0;

    flushinp();
    usleep(cfg->frame_time * 1000);
    cfg->framecounter++;
  }

  free(stars);
  free(hunters);
}

int main() {

  CONFIG cfg;
  LoadConfig(&cfg);

  srand(cfg.seed);

  int rows = cfg.level.rows;
  int cols = cfg.level.cols;

  char occupancyMap[rows][cols];

  InitMap(rows, cols, occupancyMap);

  cfg.frame_time = FRAME_TIME;
  cfg.game_time_left = 0;
  cfg.game_time_elapsed = 0;

  WINDOW *mainwin = Start();

  WIN *playwin =
      InitWin(mainwin, rows, cols, OFFY, OFFX, PLAY_COLOR, BORDER, 0);
  WIN *statwin =
      InitWin(mainwin, 5, cols, rows + OFFY, OFFX, STAT_COLOR, BORDER, 0);

  playwin->color = MAIN_COLOR;

  BIRD *bird = InitBird(playwin, cols / 2, rows / 2, cfg.start_health, rows,
                        cols, occupancyMap);

  DrawBird(bird, rows, cols, occupancyMap);
  ShowStatus(statwin, bird, cfg);
  wrefresh(playwin->window);

  MainLoop(playwin, statwin, bird, &cfg, rows, cols, occupancyMap);

  GameOver(playwin, statwin);

  Ranking(playwin, statwin, bird, cfg, cols);

  delwin(playwin->window);
  delwin(statwin->window);
  delwin(mainwin);
  endwin();

  free(bird);
  free(playwin);
  free(statwin);

  return EXIT_SUCCESS;
}
