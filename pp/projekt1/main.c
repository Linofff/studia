#include "./depedencies/bird.h"
#include "./depedencies/collisions.h"
#include "./depedencies/configmenaging.h"
#include "./depedencies/game_defs.h"
#include "./depedencies/hunters.h"
#include "./depedencies/ranking.h"
#include "./depedencies/stars.h"
#include "./depedencies/windowmenaging.h"

// --- FILE LOADING ---

void UpdateGameWorld(WIN *playwin, STAR *stars, HUNTER *hunters, BIRD *bird,
                     CONFIG *cfg, int startTime) {
  // Handle Stars
  SpawnStar(playwin, stars, *cfg);
  UpdateStars(playwin, stars, cfg->star_max);
  CheckCollisionsStar(bird, stars, *cfg);

  // Handle Hunters
  SpawnHunter(playwin, hunters, bird, *cfg);
  UpdateHunters(playwin, hunters, cfg->hunter_max);
  CheckCollisionsHunter(bird, hunters, *cfg);
  UpdateConfig(cfg, startTime);
}

void MainLoop(WIN *playwin, WIN *statwin, BIRD *bird, CONFIG *cfg) {
  // calloc allocates and zeroes memory in one step
  time_t startTime = time(NULL);
  int ch, timeLeft;

  UpdateConfig(cfg, startTime);

  STAR *stars = (STAR *)calloc(cfg->star_max, sizeof(STAR));
  HUNTER *hunters = (HUNTER *)calloc(cfg->hunter_max, sizeof(HUNTER));

  while (1) {
    ch = wgetch(statwin->window);
    timeLeft = cfg->game_time - (int)(time(NULL) - startTime);
    if (timeLeft < 0)
      timeLeft = 0;

    // Exit conditions
    if (ch == QUIT)
      break;

    // Bird Movement
    if (ch == UP || ch == LEFT || ch == DOWN || ch == RIGHT)
      ManualMoveBird(bird, ch);
    else
      MoveBird(bird);

    // Process Stars and Hunters
    UpdateGameWorld(playwin, stars, hunters, bird, cfg, startTime);

    // Rendering
    DrawBird(bird);
    ShowStatus(statwin, bird, timeLeft);
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
  ShowStatus(statwin, bird, cfg.game_time);
  wrefresh(playwin->window);

  // UpdateConfig(&cfg, 2);

  // 4. Run Game
  MainLoop(playwin, statwin, bird, &cfg);

  EndGame(statwin, bird->points, bird->health > 0);

  delwin(playwin->window);
  delwin(statwin->window);
  delwin(mainwin);
  endwin();

  free(bird);
  free(playwin);
  free(statwin);

  return EXIT_SUCCESS;
}
