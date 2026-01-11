#include "main.h"
#include "drawing.h"

// typedef struct {};
//
SDL_Surface *FlipSurfaceHorizontal(SDL_Surface *source) {
  if (!source)
    return NULL;

  // 1. Create a new surface with the same format and dimensions
  SDL_Surface *flipped = SDL_CreateRGBSurfaceWithFormat(
      0, source->w, source->h, source->format->BitsPerPixel,
      source->format->format);

  if (!flipped)
    return NULL;

  // 2. Lock surfaces to access pixels safely
  SDL_LockSurface(source);
  SDL_LockSurface(flipped);

  int pitch = source->pitch;               // Bytes per row
  int bpp = source->format->BytesPerPixel; // Bytes per pixel (usually 3 or 4)

  // 3. Iterate over every row (y) and column (x)
  for (int y = 0; y < source->h; y++) {
    for (int x = 0; x < source->w; x++) {

      // Get pointer to the pixel at (x, y) in the SOURCE
      Uint8 *srcPixel = (Uint8 *)source->pixels + (y * pitch) + (x * bpp);

      // Get pointer to the pixel at (width - 1 - x, y) in the DESTINATION
      // This effectively mirrors the x-axis
      Uint8 *dstPixel =
          (Uint8 *)flipped->pixels + (y * pitch) + ((source->w - 1 - x) * bpp);

      // Copy the pixel data
      memcpy(dstPixel, srcPixel, bpp);
    }
  }

  // 4. Unlock and return
  SDL_UnlockSurface(flipped);
  SDL_UnlockSurface(source);

  // Optional: Copy the color key (transparency) if the source has one
  if (SDL_HasColorKey(source)) {
    Uint32 colorKey;
    SDL_GetColorKey(source, &colorKey);
    SDL_SetColorKey(flipped, SDL_TRUE, colorKey);
  }

  return flipped;
}

void InitPlayer(PlayerType *player) {
  player->speed = 200;
  player->direction = 0;

  player->surface_right = SDL_LoadBMP("textures/player_right.bmp");
  if (player->surface_right == NULL) {
    printf("Error loading player bmp: %s\n", SDL_GetError());
    exit(1);
  }
  player->surface_left = FlipSurfaceHorizontal(player->surface_right);
  if (player->surface_left == NULL) {
    printf("Error creating flipped surface: %s\n", SDL_GetError());
  }
}

void InitNewGame(GameState *state, PlayerType *player, CameraType *camera) {
  state->worldTime = 0;
  player->X = LEVEL_WIDTH / 2;
  player->Y = LEVEL_HEIGHT / 2;

  camera->X = 0;
  camera->Y = 0;
}

void InitEnemies(EnemyType **enemies) {
  for (int i = 0; i < NUMBER_OF_ENEMIES; i++) {
    if (!enemies[i]->alive)
      enemies[i]->alive = 1;
  }
}

// INITIALIZATION
int InitializeSDL(SDL_Window **window, SDL_Renderer **renderer) {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    printf("SDL_Init error: %s\n", SDL_GetError());
    return -1;
  }

  if (SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0, window,
                                  renderer) != 0) {
    SDL_Quit();
    return -1;
  }

  SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
  SDL_RenderSetLogicalSize(*renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
  SDL_SetRenderDrawColor(*renderer, 0, 0, 0, 255);
  SDL_SetWindowTitle(*window, "Camera and Map Stage");
  SDL_ShowCursor(SDL_DISABLE);
  return 0;
}

//  LOGIC & MOVEMENT
void UpdatePlayer(PlayerType *player, double delta) {
  const Uint8 *keyState = SDL_GetKeyboardState(NULL);

  if (keyState[SDL_SCANCODE_W] || keyState[SDL_SCANCODE_UP])
    player->Y -= player->speed * delta;
  if (keyState[SDL_SCANCODE_S] || keyState[SDL_SCANCODE_DOWN])
    player->Y += player->speed * delta;

  if (keyState[SDL_SCANCODE_A] || keyState[SDL_SCANCODE_LEFT]) {
    player->X -= player->speed * delta;
    player->direction = 1;
  }
  if (keyState[SDL_SCANCODE_D] || keyState[SDL_SCANCODE_RIGHT]) {
    player->X += player->speed * delta;
    player->direction = 0;
  }

  // Bounds checking
  if (player->X < 0)
    player->X = 0;
  if (player->Y < 0)
    player->Y = 0;
  if (player->X > LEVEL_WIDTH)
    player->X = LEVEL_WIDTH;
  if (player->Y > LEVEL_HEIGHT)
    player->Y = LEVEL_HEIGHT;
}

void UpdateCamera(CameraType *camera, PlayerType *player) {
  camera->X = player->X - SCREEN_WIDTH / 2;
  camera->Y = player->Y - SCREEN_HEIGHT / 2;

  if (camera->X < 0)
    camera->X = 0;
  if (camera->Y < 0)
    camera->Y = 0;
  if (camera->X > LEVEL_WIDTH - SCREEN_WIDTH)
    camera->X = LEVEL_WIDTH - SCREEN_WIDTH;
  if (camera->Y > LEVEL_HEIGHT - SCREEN_HEIGHT)
    camera->Y = LEVEL_HEIGHT - SCREEN_HEIGHT;
}

// RENDERING
void DrawGame(SDL_Renderer *renderer, SDL_Window *window, SDL_Surface *screen,
              SDL_Texture *scrtex, SDL_Surface *charset, PlayerType *player,
              CameraType *camera, GameState *state, double fps) {

  // Define colors relative to the screen format
  int black = SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);
  int white = SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF);
  int skyBlue = SDL_MapRGB(screen->format, 0x87, 0xCE, 0xEB);
  int floorGrey = SDL_MapRGB(screen->format, 0x80, 0x80, 0x80);

  // 1. Clear screen
  SDL_FillRect(screen, NULL, black);

  // 2. Draw Background
  int splitY = LEVEL_HEIGHT / 4;
  DrawRectangle(screen, 0 - camera->X, 0 - camera->Y, LEVEL_WIDTH, splitY,
                skyBlue, skyBlue);
  DrawRectangle(screen, 0 - camera->X, splitY - camera->Y, LEVEL_WIDTH,
                LEVEL_HEIGHT - splitY, floorGrey, floorGrey);
  DrawLine(screen, 0 - camera->X, splitY - camera->Y, LEVEL_WIDTH, 1, 0, black);

  // 3. Draw Player
  SDL_Surface *currentSurface =
      (player->direction == 1) ? player->surface_left : player->surface_right;
  DrawSurface(screen, currentSurface, player->X - camera->X,
              player->Y - camera->Y);

  // 4. Draw HUD
  char text[128];
  DrawRectangle(screen, 4, 4, SCREEN_WIDTH - 8, 50, white, black);

  sprintf(text, "Time: %.2f | FPS: %.0f", state->worldTime, fps);
  DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 10, text, charset);

  sprintf(text, "Pos: %.0f,%.0f | Cam: %.0f,%.0f", player->X, player->Y,
          camera->X, camera->Y);
  DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 26, text, charset);

  sprintf(text, "Esc - quit | WASD - movement | n - new game");
  DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 42, text, charset);

  // 5. Push to GPU
  SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
  SDL_RenderCopy(renderer, scrtex, NULL, NULL);
  SDL_RenderPresent(renderer);
}

//  CLEANUP
void Cleanup(SDL_Window *window, SDL_Renderer *renderer, SDL_Surface *screen,
             SDL_Texture *scrtex, SDL_Surface *charset, PlayerType *player) {
  SDL_FreeSurface(player->surface_left);
  SDL_FreeSurface(player->surface_right);
  SDL_FreeSurface(charset);
  SDL_FreeSurface(screen);
  SDL_DestroyTexture(scrtex);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

int main(int argc, char *argv[]) {
  GameState state;
  PlayerType player;
  CameraType camera;
  SDL_Window *window;
  SDL_Renderer *renderer;

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
        if (event.key.keysym.sym == SDLK_n)
          InitNewGame(&state, &player, &camera);
      }
    }

    // B. Update Logic
    UpdatePlayer(&player, delta);
    UpdateCamera(&camera, &player);

    // C. Calculate FPS
    fpsTimer += delta;
    if (fpsTimer > 0.5) {
      fps = frames * 2;
      frames = 0;
      fpsTimer -= 0.5;
    }

    // D. Draw
    DrawGame(renderer, window, screen, scrtex, charset, &player, &camera,
             &state, fps);
    frames++;
  }

  // 4. Cleanup
  Cleanup(window, renderer, screen, scrtex, charset, &player);
  return 0;
}
