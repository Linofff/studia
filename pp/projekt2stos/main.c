#include "main.h"
#include "camera.h"
#include "drawing.h"
#include "enemy.h"
#include "player.h"
#include "utils.h"

void InitNewGame(GameStateType *gameState, PlayerType *player,
                 CameraType *camera, EnemyType *enemies) {
  gameState->worldTime = 0;
  ResetPlayer(player);

  for (int i = 0; i < NUMBER_OF_ENEMIES; i++)
    enemies[i].alive = 0;

  camera->X = 0;
  camera->Y = 0;
}
void GameLoop(SDL_Renderer *renderer, SDL_Surface *screen, SDL_Texture *scrtex,
              SDL_Surface *charset, GameStateType *gameState,
              PlayerType *player, CameraType *camera, EnemyType *enemies,
              EnemyAssets *assets) {

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
    gameState->worldTime += delta;

    // event handling
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT)
        quit = 1;

      if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_ESCAPE)
          quit = 1;
        if (event.key.keysym.sym == SDLK_q)
          quit = 1;

        // menu inputs
        if (gameState->currentState == STATE_MENU) {
          if (event.key.keysym.sym == SDLK_RETURN) {
            InitNewGame(gameState, player, camera, enemies);
            gameState->currentState = STATE_GAME;
          }
        }
        // game inputs
        else if (gameState->currentState == STATE_GAME) {
          // debug key 'm'
          if (event.key.keysym.sym == SDLK_m) {
            gameState->debugMode = !gameState->debugMode;
          }
          if (event.key.keysym.sym == SDLK_p) {
            gameState->currentState = STATE_MENU;
          }
          if (event.key.keysym.sym == SDLK_n) {
            InitNewGame(gameState, player, camera, enemies);
          }
        }
        // game over inputs
        else if (gameState->currentState == STATE_GAMEOVER) {
          if (event.key.keysym.sym == SDLK_RETURN) {
            gameState->currentState = STATE_MENU;
          }
        }
      }
    }

    // logic update
    if (gameState->currentState == STATE_GAME) {

      SpawnEnemies(enemies, assets);
      MoveEnemy(enemies, player, delta);
      UpdatePlayer(player, enemies, delta, gameState->worldTime);
      UpdateCamera(camera, player);

      // death check
      if (player->state == DEAD) {
        if (player->currentFrame == MAX_DEATH_FRAMES - 1) {
          gameState->currentState = STATE_GAMEOVER;
        }
      }
    }

    fpsTimer += delta;
    if (fpsTimer > 0.5) {
      fps = frames * 2;
      frames = 0;
      fpsTimer -= 0.5;
    }

    if (gameState->currentState == STATE_MENU) {
      DrawMenu(renderer, screen, scrtex, charset, "BEAT EM UP",
               "PRESS ENTER TO START");

    } else if (gameState->currentState == STATE_GAMEOVER) {
      DrawGameOver(renderer, screen, scrtex, charset, player->score);

    } else {
      DrawGame(renderer, screen, scrtex, charset, player, enemies, camera,
               gameState, fps);
    }

    frames++;
  }
}

int main() {
  srand(0);

  GameStateType gameState;
  PlayerType player;
  CameraType camera;
  SDL_Window *window;
  SDL_Renderer *renderer;
  EnemyAssets assets;
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

  LoadPlayerAssets(&player);
  LoadEnemyAssets(&assets);
  enemies = InitEnemies();

  gameState.debugMode = 0;
  gameState.currentState = STATE_MENU;
  InitNewGame(&gameState, &player, &camera, enemies);

  GameLoop(renderer, screen, scrtex, charset, &gameState, &player, &camera,
           enemies, &assets);

  Cleanup(window, renderer, screen, scrtex, charset, &player, enemies, &assets);
  return 0;
}
