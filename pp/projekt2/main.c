#include "headers/main.h"
#include "headers/camera.h"
#include "headers/drawing.h"
#include "headers/enemy.h"
#include "headers/player.h"
#include "headers/utils.h"

// 1. HELPER: Resets game state (called on start and restart)
void InitNewGame(GameState *state, PlayerType *player, CameraType *camera,
                 EnemyType *enemies) {
  state->worldTime = 0;
  ResetPlayer(player); // Uses the efficient reset we made earlier

  for (int i = 0; i < NUMBER_OF_ENEMIES; i++)
    enemies[i].alive = 0;

  camera->X = 0;
  camera->Y = 0;
}

// 2. THE GAME LOOP
// Contains the Input -> Update -> Render cycle
void GameLoop(SDL_Renderer *renderer, SDL_Surface *screen, SDL_Texture *scrtex,
              SDL_Surface *charset, GameState *state, PlayerType *player,
              CameraType *camera, EnemyType *enemies, EnemyAssets *assets) {

  int t1 = SDL_GetTicks();
  int frames = 0;
  double fpsTimer = 0;
  double fps = 0;
  int quit = 0;
  SDL_Event event;

  //

  while (!quit) {
    int t2 = SDL_GetTicks();
    double delta = (t2 - t1) * 0.001;
    t1 = t2;
    state->worldTime += delta;

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
        if (state->currentState == STATE_MENU) {
          if (event.key.keysym.sym == SDLK_RETURN) {
            InitNewGame(state, player, camera, enemies);
            state->currentState = STATE_GAME;
          }
        }
        // 2. GAME INPUTS
        else if (state->currentState == STATE_GAME) {
          if (event.key.keysym.sym == SDLK_m) {
            state->debugMode = !state->debugMode;
          }
          if (event.key.keysym.sym == SDLK_p) {
            state->currentState = STATE_MENU;
          }
          if (event.key.keysym.sym == SDLK_n) {
            InitNewGame(state, player, camera, enemies);
          }
        }
        // 3. GAME OVER INPUTS
        else if (state->currentState == STATE_GAMEOVER) {
          if (event.key.keysym.sym == SDLK_RETURN) {
            state->currentState = STATE_MENU;
          }
        }
      }
    }

    // --- B. UPDATE LOGIC ---
    if (state->currentState == STATE_GAME) {

      SpawnEnemies(enemies, assets); // Note: assets is already a pointer here
      MoveEnemy(enemies, player, delta);
      UpdatePlayer(player, enemies, delta, state->worldTime);
      UpdateCamera(camera, player);

      // Death Check
      if (player->state == DEATH_ANIM) {
        if (player->currentFrame == MAX_DEATH_FRAMES - 1) {
          state->currentState = STATE_GAMEOVER;
        }
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
    if (state->currentState == STATE_MENU) {
      DrawMenu(renderer, screen, scrtex, charset, "MY COOL GAME",
               "PRESS ENTER TO START");
    } else if (state->currentState == STATE_GAMEOVER) {
      DrawMenu(renderer, screen, scrtex, charset, "GAME OVER",
               "PRESS ENTER TO RESET");
    } else {
      DrawGame(renderer, screen, scrtex, charset, player, enemies, camera,
               state, fps);
    }

    frames++;
  }
}

// 3. MAIN (Initialization & Cleanup only)
int main() {
  srand(0);

  // -- Variables --
  GameState state;
  PlayerType player;
  CameraType camera;
  SDL_Window *window;
  SDL_Renderer *renderer;
  EnemyAssets assets;
  EnemyType *enemies = NULL;

  // -- Initialization --
  if (InitializeSDL(&window, &renderer) != 0)
    return 1;

  // Initialize Resources
  SDL_Surface *screen =
      SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32, 0x00FF0000,
                           0x0000FF00, 0x000000FF, 0xFF000000);
  SDL_Texture *scrtex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                                          SDL_TEXTUREACCESS_STREAMING,
                                          SCREEN_WIDTH, SCREEN_HEIGHT);
  SDL_Surface *charset = SDL_LoadBMP("textures/cs8x8.bmp");
  SDL_SetColorKey(charset, SDL_TRUE, 0x000000);

  // Load Heavy Assets (Once)
  LoadPlayerAssets(&player);
  LoadEnemyAssets(&assets);
  enemies = InitEnemies();

  // Setup Defaults
  state.debugMode = 1;
  state.currentState = STATE_MENU;
  InitNewGame(&state, &player, &camera, enemies);

  // -- Run Game Loop --
  GameLoop(renderer, screen, scrtex, charset, &state, &player, &camera, enemies,
           &assets);

  // -- Cleanup --
  Cleanup(window, renderer, screen, scrtex, charset, &player, enemies);
  return 0;
}
