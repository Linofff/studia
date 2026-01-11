#include "headers/main.h"
#include "headers/camera.h"
#include "headers/drawing.h"
#include "headers/enemy.h"
#include "headers/player.h"
#include "headers/utils.h"

void InitNewGame(GameState *state, PlayerType *player, CameraType *camera) {
  state->worldTime = 0;
  // player->X = LEVEL_WIDTH / 2;
  // player->Y = LEVEL_HEIGHT / 2;
  InitPlayer(player);

  camera->X = 0;
  camera->Y = 0;
}

int main() {
  srand(4);

  GameState state;
  PlayerType player;
  CameraType camera;
  SDL_Window *window;
  SDL_Renderer *renderer;

  EnemyType *enemies = NULL;
  // 1. Init SDL
  if (InitializeSDL(&window, &renderer) != 0)
    return 1;

  // 2. Init Resources
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

  InitNewGame(&state, &player, &camera);

  // 3. Game Loop Variables
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

    // A. Handle Events
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT)
        quit = 1;
      else if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_ESCAPE)
          quit = 1;
        if (event.key.keysym.sym == SDLK_q)
          quit = 1;
        if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_i)
          state.debugMode = !state.debugMode;
        if (event.key.keysym.sym == SDLK_n)
          InitNewGame(&state, &player, &camera);
      }
    }

    // B. Update Logic
    SpawnEnemies(enemies);
    MoveEnemy(enemies, &player, delta);

    UpdatePlayer(&player, enemies, delta, state.worldTime);
    UpdateCamera(&camera, &player);

    // C. Calculate FPS
    fpsTimer += delta;
    if (fpsTimer > 0.5) {
      fps = frames * 2;
      frames = 0;
      fpsTimer -= 0.5;
    }

    // D. Draw
    DrawGame(renderer, window, screen, scrtex, charset, &player, enemies,
             &camera, &state, fps);
    frames++;
  }

  // 4. Cleanup
  Cleanup(window, renderer, screen, scrtex, charset, &player, enemies);
  return 0;
}
