#include "utils.h"

int InitSDLContext() {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    printf("SDL_Init error: %s\n", SDL_GetError());
    return -1;
  }
  return 0;
}

int CreateWinRenderer(SDL_Window **window, SDL_Renderer **renderer) {
  if (SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0, window,
                                  renderer) != 0) {
    SDL_Quit();
    return -1;
  }
  return 0;
}

void ConfigureGraphics(SDL_Window *window, SDL_Renderer *renderer) {
  SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
  SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_SetWindowTitle(window, "Camera and Map Stage");
  SDL_ShowCursor(SDL_DISABLE);
}

int InitializeSDL(SDL_Window **window, SDL_Renderer **renderer) {
  if (InitSDLContext() != 0)
    return -1;
  if (CreateWinRenderer(window, renderer) != 0)
    return -1;
  ConfigureGraphics(*window, *renderer);
  return 0;
}

void CopyPixelsFlipped(SDL_Surface *src, SDL_Surface *dst) {
  int pitch = src->pitch;
  int bpp = src->format->BytesPerPixel;

  SDL_LockSurface(src);
  SDL_LockSurface(dst);

  for (int y = 0; y < src->h; y++) {
    for (int x = 0; x < src->w; x++) {
      Uint8 *srcPixel = (Uint8 *)src->pixels + (y * pitch) + (x * bpp);
      Uint8 *dstPixel =
          (Uint8 *)dst->pixels + (y * pitch) + ((src->w - 1 - x) * bpp);
      memcpy(dstPixel, srcPixel, bpp);
    }
  }

  SDL_UnlockSurface(dst);
  SDL_UnlockSurface(src);
}

SDL_Surface *FlipSurfaceHorizontal(SDL_Surface *source) {
  if (!source)
    return NULL;

  SDL_Surface *flipped = SDL_CreateRGBSurfaceWithFormat(
      0, source->w, source->h, source->format->BitsPerPixel,
      source->format->format);

  if (!flipped)
    return NULL;

  CopyPixelsFlipped(source, flipped);

  if (SDL_HasColorKey(source)) {
    Uint32 colorKey;
    SDL_GetColorKey(source, &colorKey);
    SDL_SetColorKey(flipped, SDL_TRUE, colorKey);
  }
  return flipped;
}

void LoadSingleFrame(const char *prefix, int index, SDL_Surface **destRight,
                     SDL_Surface **destLeft) {
  char filename[128];
  sprintf(filename, "%s_%d.bmp", prefix, index + 1);

  destRight[index] = SDL_LoadBMP(filename);

  if (!destRight[index]) {
    printf("Failed to load %s\n", filename);
    if (destLeft)
      destLeft[index] = NULL;
    return;
  }

  if (destLeft) {
    destLeft[index] = FlipSurfaceHorizontal(destRight[index]);
  }
}

void LoadAnimationSequence(const char *prefix, int count,
                           SDL_Surface **destRight, SDL_Surface **destLeft) {
  for (int i = 0; i < count; i++) {
    LoadSingleFrame(prefix, i, destRight, destLeft);
  }
}

void FreeSurfaceArray(SDL_Surface **arr, int count) {
  for (int i = 0; i < count; i++) {
    if (arr[i]) {
      SDL_FreeSurface(arr[i]);
      arr[i] = NULL;
    }
  }
}

void FreeEnemyAssets(EnemyAssets *assets) {
  if (!assets)
    return;

  // Free Type 0 (Red)
  FreeSurfaceArray(assets->t0_walk_right, MAX_WALK_FRAMES);
  FreeSurfaceArray(assets->t0_walk_left, MAX_WALK_FRAMES);
  FreeSurfaceArray(assets->t0_hit_right, MAX_HIT_FRAMES);
  FreeSurfaceArray(assets->t0_hit_left, MAX_HIT_FRAMES);
  FreeSurfaceArray(assets->t0_attack_right, MAX_ATTACK_FRAMES);
  FreeSurfaceArray(assets->t0_attack_left, MAX_ATTACK_FRAMES);

  // Free Type 1 (Green)
  FreeSurfaceArray(assets->t1_walk_right, MAX_WALK_FRAMES);
  FreeSurfaceArray(assets->t1_walk_left, MAX_WALK_FRAMES);
  FreeSurfaceArray(assets->t1_hit_right, MAX_HIT_FRAMES);
  FreeSurfaceArray(assets->t1_hit_left, MAX_HIT_FRAMES);
  FreeSurfaceArray(assets->t1_attack_right, MAX_ATTACK_FRAMES);
  FreeSurfaceArray(assets->t1_attack_left, MAX_ATTACK_FRAMES);
  FreeSurfaceArray(assets->t1_dash_right, MAX_DASH_FRAMES);
  FreeSurfaceArray(assets->t1_dash_left, MAX_DASH_FRAMES);
}

void FreePlayerMemory(PlayerType *player) {
  if (!player)
    return;
  FreeSurfaceArray(player->walk_frames_right, MAX_WALK_FRAMES);
  FreeSurfaceArray(player->walk_frames_left, MAX_WALK_FRAMES);
  FreeSurfaceArray(player->attack_frames_right, MAX_ATTACK_FRAMES);
  FreeSurfaceArray(player->attack_frames_left, MAX_ATTACK_FRAMES);
  FreeSurfaceArray(player->dash_frames_right, MAX_DASH_FRAMES);
  FreeSurfaceArray(player->dash_frames_left, MAX_DASH_FRAMES);
  FreeSurfaceArray(player->air_frames_right, MAX_AIR_FRAMES);
  FreeSurfaceArray(player->air_frames_left, MAX_AIR_FRAMES);
  FreeSurfaceArray(player->idle_frames_right, MAX_IDLE_FRAMES);
  FreeSurfaceArray(player->idle_frames_left, MAX_IDLE_FRAMES);
  FreeSurfaceArray(player->hit_frames_right, MAX_HIT_FRAMES);
  FreeSurfaceArray(player->hit_frames_left, MAX_HIT_FRAMES);
  FreeSurfaceArray(player->death_frames_right, MAX_DEATH_FRAMES);
  FreeSurfaceArray(player->death_frames_left, MAX_DEATH_FRAMES);
}

void Cleanup(SDL_Window *window, SDL_Renderer *renderer, SDL_Surface *screen,
             SDL_Texture *scrtex, SDL_Surface *charset, PlayerType *player,
             EnemyType *enemies, EnemyAssets *assets) {

  FreePlayerMemory(player);

  FreeEnemyAssets(assets);

  if (enemies)
    free(enemies);

  SDL_FreeSurface(charset);
  SDL_FreeSurface(screen);
  SDL_DestroyTexture(scrtex);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);

  SDL_Quit();
}
