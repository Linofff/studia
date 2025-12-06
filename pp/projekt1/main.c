#include "albatross.h"
#include "bird.h"
#include "configmanaging.h"
#include "fog.h"
#include "game_defs.h"
#include "hunters.h"
#include "ranking.h"
#include "stars.h"
#include "windowmanaging.h"

void UpdateGameWorld(WIN *playwin, STAR *stars, HUNTER *hunters, BIRD *bird,
                     CONFIG *cfg, int rows, int cols,
                     char occupancyMap[rows][cols]) {
  UpdateConfig(cfg);

  UpdateFog(cfg, bird);

  SpawnStar(bird, playwin, stars, *cfg, rows, cols, occupancyMap);
  UpdateStars(playwin, stars, rows, cols, occupancyMap, bird, cfg, hunters);

  SpawnHunter(playwin, hunters, bird, *cfg, rows, cols, occupancyMap);
  UpdateDashingHunters(hunters, bird, *cfg);
  UpdateHunters(playwin, hunters, cfg->level.hunter_max, bird, *cfg, rows, cols,
                occupancyMap, stars);

  DrawFog(playwin, cfg, rows, cols, occupancyMap);
}

void ResolveChar(WIN *playwin, char ch, int rows, int cols,
                 char occupancyMap[rows][cols], BIRD *bird, bool *running,
                 HUNTER *hunters, STAR *stars, CONFIG *cfg) {
  // Game quit
  if (ch == QUIT)
    *running = 0;

  // Bird Movment
  if (ch == UP || ch == LEFT || ch == DOWN || ch == RIGHT)
    ChangeDirectionBird(bird, ch, rows, cols, occupancyMap, stars, hunters, cfg,
                        playwin);
  else
    MoveBird(bird, rows, cols, occupancyMap, stars, hunters, cfg, playwin);

  // Handling Albatross Taxi
  if (ch == TAXI_IN)
    AlbatrossTaxi(hunters, stars, bird, cfg, rows, cols, occupancyMap, playwin);
  if (ch == TAXI_OUT)
    OutOfAlbatrossTaxi(bird);

  // Changing the speed of the game
  if (ch == FASTER && cfg->game_speed < 4) {
    cfg->frame_time /= 1.5;
    cfg->game_speed++;
  } else if (ch == SLOWER && cfg->game_speed > 0) {
    cfg->frame_time *= 1.5;
    cfg->game_speed--;
  }
}

void AllocateMemory(CONFIG *cfg, STAR **stars, HUNTER **hunters) {
  *stars = (STAR *)calloc(cfg->level.star_max, sizeof(STAR));

  if (*stars == NULL) {
    endwin();
    exit(EXIT_FAILURE);
  }

  // calculating real number of hunter in given game time and rounding it up
  const int maxNumberOfHunters = cfg->level.initial_hunter_max +
                                 (cfg->game_time_start / TIME_ENTITY_MULTI) + 1;

  *hunters = (HUNTER *)calloc(maxNumberOfHunters, sizeof(HUNTER));

  if (*hunters == NULL) {
    free(*stars);
    endwin();
  }
}

void MainLoop(WIN *playwin, WIN *statwin, BIRD *bird, CONFIG *cfg, int rows,
              int cols, char occupancyMap[rows][cols]) {
  time_t startTime = time(NULL);
  char ch;
  bool running = 1;

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
    ResolveChar(playwin, ch, rows, cols, occupancyMap, bird, &running, hunters,
                stars, cfg);

    MainLoopAlbatrossCheck(playwin, bird, rows, cols);

    UpdateGameWorld(playwin, stars, hunters, bird, cfg, rows, cols,
                    occupancyMap);

    ShowStatus(statwin, bird, *cfg);
    wrefresh(playwin->window);

    if (bird->health <= 0 || cfg->game_time_left <= 0)
      running = 0;

    if (bird->points >= cfg->level.star_quota)
      running = 0;

    flushinp();
    usleep(cfg->frame_time * 1000);

    // incrementing framecounter to use it in animations
    cfg->framecounter++;
  }

  free(stars);
  free(hunters);
}

int main() {

  CONFIG cfg;
  LoadConfig(&cfg);

  srand(cfg.seed);

  const int rows = cfg.level.rows;
  const int cols = cfg.level.cols;

  char occupancyMap[rows][cols];

  InitMap(rows, cols, occupancyMap);

  // initializing cfg values to prevent random values
  cfg.frame_time = FRAME_TIME;
  cfg.game_time_left = 0;
  cfg.game_time_elapsed = 0;
  cfg.game_speed = 0;
  cfg.fog_currentsize = 0;

  WINDOW *mainwin = Start();

  WIN *playwin =
      InitWin(mainwin, rows, cols, OFFY, OFFX, PLAY_COLOR, BORDER, 0);
  WIN *statwin =
      InitWin(mainwin, 5, cols, rows + OFFY, OFFX, STAT_COLOR, BORDER, 0);

  playwin->color = MAIN_COLOR;

  BIRD *bird =
      InitBird(playwin, cols / 2, rows / 2, rows, cols, occupancyMap, cfg);

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
