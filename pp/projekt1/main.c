#include "./headers/albatross.h"
#include "./headers/bird.h"
#include "./headers/collisions.h"
#include "./headers/configmanaging.h"
#include "./headers/game_defs.h"
#include "./headers/hunters.h"
#include "./headers/ranking.h"
#include "./headers/stars.h"
#include "./headers/windowmanaging.h"

// --- FILE LOADING ---

void UpdateGameWorld(WIN *playwin, STAR *stars, HUNTER *hunters, BIRD *bird,
                     CONFIG *cfg, int startTime) {
  // Handle Stars
  SpawnStar(bird, playwin, stars, *cfg);
  UpdateStars(playwin, stars, cfg->star_max);
  CheckCollisionsStar(bird, stars, *cfg);

  // Handle Hunters
  SpawnHunter(playwin, hunters, bird, *cfg);
  UpdateHunters(playwin, hunters, cfg->hunter_max, bird, *cfg);
  CheckCollisionsHunter(bird, hunters, *cfg);
  UpdateConfig(cfg, startTime);
}

// Calculates elapsed and remaining time based on the start time

void MainLoop(WIN *playwin, WIN *statwin, BIRD *bird, CONFIG *cfg) {
  time_t startTime = time(NULL);
  int ch;

  // Initialize time variables
  cfg->game_time_elapsed = 0;
  int timeLeft = cfg->game_time;

  // Initial config update
  UpdateConfig(cfg, (int)startTime);

  STAR *stars = (STAR *)calloc(cfg->star_max, sizeof(STAR));
  HUNTER *hunters = (HUNTER *)calloc(cfg->hunter_max, sizeof(HUNTER));

  while (1) {

    box(playwin->window, 0, 0);

    ch = wgetch(statwin->window);

    // --- TIME CALCULATION ---
    // We pass the ADDRESS (&) of startTime so UpdateTimeState can modify it
    UpdateTimeState(bird, &startTime, cfg->game_time, &cfg->game_time_elapsed,
                    &timeLeft);

    // Exit conditions
    if (ch == QUIT)
      break;

    // Bird Movement
    if (ch == UP || ch == LEFT || ch == DOWN || ch == RIGHT)
      ManualMoveBird(bird, ch);
    else
      MoveBird(bird);

    if (ch == TAXI_IN)
      AlbatrossTaxi(hunters, stars, bird, cfg);

    if (ch == TAXI_OUT)
      OutOfAlbatrossTaxi(hunters, stars, bird, cfg);

    if (bird->points >= cfg->star_quota)
      break;

    // Process Stars and Hunters
    // Note: We cast startTime to (int) to match your function prototype
    UpdateGameWorld(playwin, stars, hunters, bird, cfg, (int)startTime);

    // Rendering
    DrawBird(bird);
    ShowStatus(statwin, bird, *cfg);
    wrefresh(playwin->window);

    // Game Over Check
    if (bird->health <= 0 || timeLeft == 0)
      break;

    flushinp();
    usleep(FRAME_TIME * 1000);
  }

  free(stars);
  free(hunters);
}

int main() {
  srand(time(NULL));

  // 1. Load Settings
  CONFIG cfg;
  LoadConfig(&cfg);

  // 2. Start Ncurses
  WINDOW *mainwin = Start();

  WIN *playwin =
      InitWin(mainwin, ROWS, COLS, OFFY, OFFX, PLAY_COLOR, BORDER, 0);
  WIN *statwin =
      InitWin(mainwin, 3, COLS, ROWS + OFFY, OFFX, STAT_COLOR, BORDER, 0);

  // 3. Init Bird
  BIRD *bird = InitBird(playwin, COLS / 2, ROWS / 2, cfg.start_health);

  DrawBird(bird);
  ShowStatus(statwin, bird, cfg);
  wrefresh(playwin->window);

  // UpdateConfig(&cfg, 2);

  // 4. Run Game
  MainLoop(playwin, statwin, bird, &cfg);

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
