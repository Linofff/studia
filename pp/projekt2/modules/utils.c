#include "../headers/utils.h"

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

SDL_Surface *FlipSurfaceHorizontal(SDL_Surface *source) {
  if (!source)
    return NULL;
  SDL_Surface *flipped = SDL_CreateRGBSurfaceWithFormat(
      0, source->w, source->h, source->format->BitsPerPixel,
      source->format->format);
  if (!flipped)
    return NULL;

  SDL_LockSurface(source);
  SDL_LockSurface(flipped);

  int pitch = source->pitch;
  int bpp = source->format->BytesPerPixel;

  for (int y = 0; y < source->h; y++) {
    for (int x = 0; x < source->w; x++) {
      Uint8 *srcPixel = (Uint8 *)source->pixels + (y * pitch) + (x * bpp);
      Uint8 *dstPixel =
          (Uint8 *)flipped->pixels + (y * pitch) + ((source->w - 1 - x) * bpp);
      memcpy(dstPixel, srcPixel, bpp);
    }
  }

  SDL_UnlockSurface(flipped);
  SDL_UnlockSurface(source);

  if (SDL_HasColorKey(source)) {
    Uint32 colorKey;
    SDL_GetColorKey(source, &colorKey);
    SDL_SetColorKey(flipped, SDL_TRUE, colorKey);
  }
  return flipped;
}

void LoadAnimationSequence(const char *prefix, int count,
                           SDL_Surface **destRight, SDL_Surface **destLeft) {
  char filename[128];
  for (int i = 0; i < count; i++) {
    // Construct filename (e.g., "textures/fighter_run_1.bmp")
    sprintf(filename, "%s_%d.bmp", prefix, i + 1);

    // Load Right (Original)
    destRight[i] = SDL_LoadBMP(filename);
    if (!destRight[i]) {
      printf("Failed to load %s\n", filename);
      continue;
    }

    // Create Left (Flipped)
    if (destLeft) {
      destLeft[i] = FlipSurfaceHorizontal(destRight[i]);
    }
  }
}

void Cleanup(SDL_Window *window, SDL_Renderer *renderer, SDL_Surface *screen,
             SDL_Texture *scrtex, SDL_Surface *charset, PlayerType *player,
             EnemyType *enemies) {

  // Check if enemies exist
  // if (enemies) {
  //   for (int i = 0; i < NUMBER_OF_ENEMIES; i++) {
  //     // Simple access now: enemies[i]
  //     for (int j = 0; j < MAX_WALK_FRAMES; j++) {
  //       if (enemies[i].walk_frames_right[j])
  //         SDL_FreeSurface(enemies[i].walk_frames_right[j]);
  //       if (enemies[i].walk_frames_left[j])
  //         SDL_FreeSurface(enemies[i].walk_frames_left[j]);
  //     }
  //   }
  //   // Free the array itself
  //   free(enemies);
  // }

  for (int i = 0; i < MAX_WALK_FRAMES; i++) {
    SDL_FreeSurface(player->walk_frames_left[i]);
    SDL_FreeSurface(player->walk_frames_right[i]);
  }
  SDL_FreeSurface(charset);
  SDL_FreeSurface(screen);
  SDL_DestroyTexture(scrtex);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}
