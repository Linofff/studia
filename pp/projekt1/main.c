#include "./headers/albatross.h"
#include "./headers/bird.h"
#include "./headers/configmanaging.h"
#include "./headers/game_defs.h"
#include "./headers/hunters.h"
// #include "./headers/ranking.h"
#include "./headers/stars.h"
#include "./headers/windowmanaging.h"

// --- FILE LOADING ---

void UpdateGameWorld(WIN *playwin, STAR *stars, HUNTER *hunters, BIRD *bird,
                     CONFIG *cfg, int startTime,
                     char occupancyMap[ROWS][COLS]) {
  UpdateConfig(cfg, startTime);
  // Handle Stars
  SpawnStar(bird, playwin, stars, *cfg, occupancyMap);
  UpdateStars(playwin, stars, cfg->star_max, occupancyMap, bird);

  // Handle Hunters
  SpawnHunter(playwin, hunters, bird, *cfg, occupancyMap);
  UpdateHunters(playwin, hunters, cfg->hunter_max, bird, *cfg, occupancyMap,
                stars);
}

void ResolveChar(WIN *playwin, WIN *statwin, char ch,
                 char occupancyMap[ROWS][COLS], BIRD *bird, bool *running,
                 HUNTER *hunters, STAR *stars, CONFIG *cfg) {
  if (ch == 'm' || ch == 'M') {
    DebugDrawMap(playwin, occupancyMap);

    while (wgetch(statwin->window) != 'm')
      ;

    wclear(playwin->window);
    box(playwin->window, 0, 0);
  }

  // Exit conditions
  if (ch == QUIT)
    *running = 0;

  // Bird Movement
  if (ch == UP || ch == LEFT || ch == DOWN || ch == RIGHT || ch == STOP)
    ChangeDirectionBird(bird, ch, occupancyMap, stars, hunters, cfg, playwin);
  else
    MoveBird(bird, occupancyMap, stars, hunters, cfg, playwin);

  if (ch == TAXI_IN)
    AlbatrossTaxi(hunters, stars, bird, cfg, occupancyMap);

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

void MainLoop(WIN *playwin, WIN *statwin, BIRD *bird, CONFIG *cfg,
              char occupancyMap[ROWS][COLS]) {
  time_t startTime = time(NULL);
  int ch;
  bool running = 1;

  UpdateConfig(cfg, (int)startTime);

  StartScreen(playwin, &running);

  // Initialize time variables
  cfg->game_time_elapsed = 0;
  int timeLeft = cfg->game_time_start;
  cfg->game_time_left = cfg->game_time_start;

  STAR *stars = (STAR *)calloc(cfg->star_max, sizeof(STAR));
  HUNTER *hunters = (HUNTER *)calloc(cfg->hunter_max, sizeof(HUNTER));

  while (running) {

    box(playwin->window, 0, 0);

    UpdateTimeState(bird, &startTime, cfg);

    ch = wgetch(statwin->window);
    ResolveChar(playwin, statwin, ch, occupancyMap, bird, &running, hunters,
                stars, cfg);

    DrawBird(bird);

    MainLoopAlbatrossCheck(playwin, bird);

    UpdateGameWorld(playwin, stars, hunters, bird, cfg, (int)startTime,
                    occupancyMap);

    ShowStatus(statwin, bird, *cfg);
    wrefresh(playwin->window);

    // Game Over Check
    if (bird->health <= 0 || cfg->game_time_left <= 0)
      running = 0;

    if (bird->points >= cfg->star_quota)
      running = 0;

    flushinp();
    usleep(cfg->frame_time * 1000);
  }

  free(stars);
  free(hunters);
}

int main() {

  // 1. Load Settings
  CONFIG cfg;
  LoadConfig(&cfg);

  srand(cfg.seed);
  // srand(time(NULL));

  char occupancyMap[ROWS][COLS];

  InitMap(occupancyMap);

  cfg.frame_time = FRAME_TIME;
  cfg.game_time_left = 0;

  // 2. Start Ncurses
  WINDOW *mainwin = Start();

  WIN *playwin =
      InitWin(mainwin, ROWS, COLS, OFFY, OFFX, PLAY_COLOR, BORDER, 0);
  WIN *statwin =
      InitWin(mainwin, 4, COLS, ROWS + OFFY, OFFX, STAT_COLOR, BORDER, 0);

  // 3. Init Bird
  BIRD *bird =
      InitBird(playwin, COLS / 2, ROWS / 2, cfg.start_health, occupancyMap);

  DrawBird(bird);
  ShowStatus(statwin, bird, cfg);
  wrefresh(playwin->window);

  // 4. Run Game

  MainLoop(playwin, statwin, bird, &cfg, occupancyMap);

  EndGame(statwin, bird->points, bird->health > 0, cfg);

  delwin(playwin->window);
  delwin(statwin->window);
  delwin(mainwin);
  endwin();

  free(bird);
  free(playwin);
  free(statwin);

  return EXIT_SUCCESS;
}
