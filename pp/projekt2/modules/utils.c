#include "../headers/utils.h"
#include <stdlib.h>

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

void Cleanup(SDL_Window *window, SDL_Renderer *renderer, SDL_Surface *screen,
             SDL_Texture *scrtex, SDL_Surface *charset, PlayerType *player,
             EnemyType *enemies) {

  // Check if enemies exist
  if (enemies) {
    for (int i = 0; i < NUMBER_OF_ENEMIES; i++) {
      // Simple access now: enemies[i]
      if (enemies[i].surface_left)
        SDL_FreeSurface(enemies[i].surface_left);
      if (enemies[i].surface_right)
        SDL_FreeSurface(enemies[i].surface_right);
    }
    // Free the array itself
    free(enemies);
  }

  SDL_FreeSurface(player->surface_left);
  SDL_FreeSurface(player->surface_right);
  SDL_FreeSurface(charset);
  SDL_FreeSurface(screen);
  SDL_DestroyTexture(scrtex);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}
