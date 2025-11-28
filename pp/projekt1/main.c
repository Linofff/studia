#include "./headers/albatross.h"
#include "./headers/bird.h"
#include "./headers/collisions.h"
#include "./headers/configmanaging.h"
#include "./headers/game_defs.h"
#include "./headers/hunters.h"
#include "./headers/ranking.h"
#include "./headers/stars.h"
#include "./headers/windowmanaging.h"
#include "ncurses.h"

// --- FILE LOADING ---

void UpdateGameWorld(WIN *playwin, STAR *stars, HUNTER *hunters, BIRD *bird,
                     CONFIG *cfg, int startTime) {
  UpdateConfig(cfg, startTime);
  // Handle Stars
  CheckCollisionsStar(bird, stars, *cfg);
  SpawnStar(bird, playwin, stars, *cfg);
  UpdateStars(playwin, stars, cfg->star_max);
  CheckCollisionsStar(bird, stars, *cfg);

  // Handle Hunters
  CheckCollisionsHunter(bird, hunters, *cfg);
  SpawnHunter(playwin, hunters, bird, *cfg);
  UpdateHunters(playwin, hunters, cfg->hunter_max, bird, *cfg);
  CheckCollisionsHunter(bird, hunters, *cfg);
}

// Calculates elapsed and remaining time based on the start time

void MainLoop(WIN *playwin, WIN *statwin, BIRD *bird, CONFIG *cfg) {
  time_t startTime = time(NULL);
  int ch, running = 1;

  UpdateConfig(cfg, (int)startTime);

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

    // Exit conditions
    if (ch == QUIT)
      running = 0;

    // Bird Movement
    if (ch == UP || ch == LEFT || ch == DOWN || ch == RIGHT)
      ManualMoveBird(bird, ch);
    else
      MoveBird(bird);

    if (ch == TAXI_IN)
      AlbatrossTaxi(hunters, stars, bird, cfg);

    if (ch == TAXI_OUT)
      OutOfAlbatrossTaxi(hunters, stars, bird, cfg);

    if (ch == FASTER && cfg->game_speed < 4) {
      cfg->frame_time /= 1.5;
      cfg->game_speed++;
    } else if (ch == SLOWER && cfg->game_speed > 0) {
      cfg->frame_time *= 1.5;
      cfg->game_speed--;
    }

    DrawBird(bird);

    // int temp = playwin->color;
    if (bird->is_in_albatross_taxi) {
      // playwin->color = TAXI_COLOR;
      mvwprintw(playwin->window, (ROWS / 4), (2 * COLS / 5),
                "You are in the taxi");
    } else {
      mvwprintw(playwin->window, (ROWS / 4), (2 * COLS / 5),
                "                   ");
      // playwin->color = temp;
    }

    UpdateGameWorld(playwin, stars, hunters, bird, cfg, (int)startTime);

    ShowStatus(statwin, bird, *cfg);
    wrefresh(playwin->window);

    // Game Over Check
    if (bird->health <= 0 || timeLeft == 0)
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

  cfg.game_speed = 0;
  cfg.frame_time = 100;

  // 2. Start Ncurses
  WINDOW *mainwin = Start();

  WIN *playwin =
      InitWin(mainwin, ROWS, COLS, OFFY, OFFX, PLAY_COLOR, BORDER, 0);
  WIN *statwin =
      InitWin(mainwin, 4, COLS, ROWS + OFFY, OFFX, STAT_COLOR, BORDER, 0);

  // 3. Init Bird
  BIRD *bird = InitBird(playwin, COLS / 2, ROWS / 2, cfg.start_health);

  DrawBird(bird);
  ShowStatus(statwin, bird, cfg);
  wrefresh(playwin->window);

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
