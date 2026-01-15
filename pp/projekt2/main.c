#include "headers/main.h"
#include "headers/camera.h"
#include "headers/drawing.h"
#include "headers/enemy.h"
#include "headers/player.h"
#include "headers/utils.h"

void InitNewGame(GameState *state, PlayerType *player, CameraType *camera,
                 EnemyType *enemies) {
  state->worldTime = 0;
  InitPlayer(player);

  for (int i = 0; i < NUMBER_OF_ENEMIES; i++)
    enemies[i].alive = 0;

  camera->X = 0;
  camera->Y = 0;
}

int main() {
  srand(0);

  GameState state;
  PlayerType player;
  CameraType camera;
  SDL_Window *window;
  SDL_Renderer *renderer;

  state.debugMode = 0;             // Default debug off
  state.currentState = STATE_MENU; // <--- START AT MENU

  EnemyType *enemies = NULL;

  if (InitializeSDL(&window, &renderer) != 0)
    return 1;

  SDL_Surface *screen =
      SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32, 0x00FF0000,
                           0x0000FF00, 0x000000FF, 0xFF000000);
  SDL_Texture *scrtex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                                          SDL_TEXTUREACCESS_STREAMING,
                                          SCREEN_WIDTH, SCREEN_HEIGHT);
  SDL_Surface *charset = SDL_LoadBMP("textures/cs8x8.bmp");
  SDL_SetColorKey(charset, SDL_TRUE, 0x000000);

  InitPlayer(&player);
  enemies = InitEnemies();

  // Don't start game yet, just setup structs
  InitNewGame(&state, &player, &camera, enemies);

  int t1 = SDL_GetTicks();
  int frames = 0;
  double fpsTimer = 0;
  double fps = 0;
  int quit = 0;
  SDL_Event event;

  while (!quit) {
    int t2 = SDL_GetTicks();
    double delta = (t2 - t1) * 0.001;
    t1 = t2;
    state.worldTime += delta;

    // --- A. HANDLE EVENTS ---
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT)
        quit = 1;

      if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_ESCAPE)
          quit = 1;
        if (event.key.keysym.sym == SDLK_q)
          quit = 1;

        // 1. MENU INPUTS
        if (state.currentState == STATE_MENU) {
          if (event.key.keysym.sym == SDLK_RETURN) {
            InitNewGame(&state, &player, &camera, enemies);
            state.currentState = STATE_GAME; // Switch to Game
          }
        }
        // 2. GAME INPUTS
        else if (state.currentState == STATE_GAME) {
          if (event.key.keysym.sym == SDLK_m) {
            state.debugMode = !state.debugMode;
          }
          if (event.key.keysym.sym == SDLK_p) {
            state.currentState = STATE_MENU; // Pause/Back to menu
          }
        }
        // 3. GAME OVER INPUTS
        else if (state.currentState == STATE_GAMEOVER) {
          if (event.key.keysym.sym == SDLK_RETURN) {
            state.currentState = STATE_MENU; // Go back to title
          }
        }
      }
    }

    // --- B. UPDATE LOGIC ---
    if (state.currentState == STATE_GAME) {

      // Only run game physics if in STATE_GAME
      SpawnEnemies(enemies);
      MoveEnemy(enemies, &player, delta);
      UpdatePlayer(&player, enemies, delta, state.worldTime);
      UpdateCamera(&camera, &player);

      // Check for Death
      if (player.health <= 0) {
        state.currentState = STATE_GAMEOVER;
      }
    }

    // --- C. CALCULATE FPS ---
    fpsTimer += delta;
    if (fpsTimer > 0.5) {
      fps = frames * 2;
      frames = 0;
      fpsTimer -= 0.5;
    }

    // --- D. DRAW ---
    if (state.currentState == STATE_MENU) {
      DrawMenu(renderer, screen, scrtex, charset, "MY COOL GAME",
               "PRESS ENTER TO START");
    } else if (state.currentState == STATE_GAMEOVER) {
      DrawMenu(renderer, screen, scrtex, charset, "GAME OVER",
               "PRESS ENTER TO RESET");
    } else {
      // Only draw the actual game world if playing
      DrawGame(renderer, screen, scrtex, charset, &player, enemies, &camera,
               &state, fps);
    }

    frames++;
  }

  Cleanup(window, renderer, screen, scrtex, charset, &player, enemies);
  return 0;
}
