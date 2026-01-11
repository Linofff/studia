#include "../headers/drawing.h"

void DrawOutline(SDL_Surface *screen, int x, int y, int w, int h,
                 Uint32 color) {
  DrawLine(screen, x, y, h, 0, 1, color);         // Left Vertical
  DrawLine(screen, x + w - 1, y, h, 0, 1, color); // Right Vertical
  DrawLine(screen, x, y, w, 1, 0, color);         // Top Horizontal
  DrawLine(screen, x, y + h - 1, w, 1, 0, color); // Bottom Horizontal
}

const char *GetInputName(InputType in) {
  switch (in) {
  case IN_LIGHT:
    return "L";
  case IN_HEAVY:
    return "H";
  case IN_LEFT:
    return "<";
  case IN_RIGHT:
    return ">";
  default:
    return "_";
  }
}

// Helper to get string name of state
const char *GetStateName(int state) {
  switch (state) {
  case IDLE:
    return "IDLE";
  case ATTACK_LIGHT:
    return "ATK_L";
  case ATTACK_HEAVY:
    return "ATK_H";
  case COMBO_TRIPLE:
    return "TRIPLE!";
  case COMBO_MIX:
    return "MIX!!";
  case DASH:
    return "DASH";
  default:
    return "???";
  }
}

void DrawGame(SDL_Renderer *renderer, SDL_Window *window, SDL_Surface *screen,
              SDL_Texture *scrtex, SDL_Surface *charset, PlayerType *player,
              EnemyType *enemies, CameraType *camera, GameState *state,
              double fps) {

  // Define colors relative to the screen format
  int black = SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);
  int white = SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF);
  int skyBlue = SDL_MapRGB(screen->format, 0x87, 0xCE, 0xEB);
  int floorGrey = SDL_MapRGB(screen->format, 0x80, 0x80, 0x80);

  // Debug Colors
  int red = SDL_MapRGB(screen->format, 0xFF, 0x00, 0x00);
  int green = SDL_MapRGB(screen->format, 0x00, 0xFF, 0x00);
  int yellow = SDL_MapRGB(screen->format, 0xFF, 0xFF, 0x00);

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

  // [DEBUG] Draw Player Body Hitbox (Green)
  // Uses actual image size to draw the box around the sprite
  if (player->surface_right) {
    int pw = player->surface_right->w;
    int ph = player->surface_right->h;

    // Calculate top-left based on center (X,Y)
    DrawOutline(screen, (int)(player->X - (pw / 2) - camera->X),
                (int)(player->Y - (ph / 2) - camera->Y), pw, ph, green);
  }

  if (state->debugMode) {
    char bufferText[256] = "Buffer: ";
    char temp[16];

    // 1. Draw Input Buffer contents
    for (int i = 0; i < player->buffer.count; i++) {
      sprintf(temp, "%s ", GetInputName(player->buffer.events[i].input));
      strcat(bufferText, temp);
    }
    DrawString(screen, 10, 60, bufferText, charset);

    // 2. Draw Current Action Name
    char stateText[64];
    sprintf(stateText, "Action: %s", GetStateName(player->state));
    DrawString(screen, 10, 76, stateText, charset);
  }

  // [DEBUG] Draw Attack Hitbox (Red) - Only if attacking
  if (player->attackTimer > 0 && player->surface_right) {
    int hitW;
    if (player->state == ATTACK_LIGHT) {
      hitW = 60;
    } else if (player->state == COMBO_TRIPLE) {
      hitW = 120; // Massive range
    } else if (player->state == DASH) {
      hitW = 200; // Heavy / Mix
    } else {
      hitW = 100;
    }
    int hitH = 50;
    int pHalfW = player->surface_right->w / 2;

    // Calculate X exactly matching ProcessAttack logic
    int hitX;
    if (player->direction == 0) {
      // Right: Start at Center + Half Width
      hitX = player->X + pHalfW;
    } else {
      // Left: Start at Center - Half Width - Attack Width
      hitX = player->X - pHalfW - hitW;
    }

    int hitY = player->Y - (hitH / 2);

    DrawOutline(screen, (int)(hitX - camera->X), (int)(hitY - camera->Y), hitW,
                hitH, red);
  }

  // 4. Draw Enemies
  for (int i = 0; i < NUMBER_OF_ENEMIES; i++) {
    if (enemies[i].alive) {
      // Draw Sprite
      SDL_Surface *eSurf = (!enemies[i].direction) ? enemies[i].surface_right
                                                   : enemies[i].surface_left;
      DrawSurface(screen, eSurf, enemies[i].X - camera->X,
                  enemies[i].Y - camera->Y);

      // [DEBUG] Draw Enemy Hitbox (Yellow)
      // Matches dynamic size logic in player.c
      if (enemies[i].surface_right) {
        int ew = enemies[i].surface_right->w;
        int eh = enemies[i].surface_right->h;

        DrawOutline(screen, (int)(enemies[i].X - (ew / 2) - camera->X),
                    (int)(enemies[i].Y - (eh / 2) - camera->Y), ew, eh, yellow);
      }
    }
  }

  // 5. Draw HUD
  char text[128];
  DrawRectangle(screen, 4, 4, SCREEN_WIDTH - 8, 50, white, black);

  sprintf(text, "Score: %d | Time: %.2f | FPS: %.0f", player->score,
          state->worldTime, fps);
  DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 10, text, charset);

  sprintf(text, "Pos: %.0f,%.0f | Cam: %.0f,%.0f", player->X, player->Y,
          camera->X, camera->Y);
  DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 26, text, charset);

  // Update Instructions for J/K keys
  sprintf(text, "Esc-Quit | WASD-Move | Space-Jump | J-Light | K-Heavy");
  DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 42, text, charset);

  // 6. Push to GPU
  SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
  SDL_RenderCopy(renderer, scrtex, NULL, NULL);
  SDL_RenderPresent(renderer);
}

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

void DrawString(SDL_Surface *screen, int x, int y, const char *text,
                SDL_Surface *charset) {
  int px, py, c;
  SDL_Rect s, d;
  s.w = 8;
  s.h = 8;
  d.w = 8;
  d.h = 8;
  while (*text) {
    c = *text & 255;
    px = (c % 16) * 8;
    py = (c / 16) * 8;
    s.x = px;
    s.y = py;
    d.x = x;
    d.y = y;
    SDL_BlitSurface(charset, &s, screen, &d);
    x += 8;
    text++;
  }
}

void DrawSurface(SDL_Surface *screen, SDL_Surface *sprite, int x, int y) {
  SDL_Rect dest;
  dest.x = x - sprite->w / 2;
  dest.y = y - sprite->h / 2;
  dest.w = sprite->w;
  dest.h = sprite->h;
  SDL_BlitSurface(sprite, NULL, screen, &dest);
}

void DrawPixel(SDL_Surface *surface, int x, int y, Uint32 color) {
  int bpp = surface->format->BytesPerPixel;
  if (x < 0 || x >= surface->w || y < 0 || y >= surface->h)
    return;
  Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;
  *(Uint32 *)p = color;
}

void DrawLine(SDL_Surface *screen, int x, int y, int l, int dx, int dy,
              Uint32 color) {
  for (int i = 0; i < l; i++) {
    DrawPixel(screen, x, y, color);
    x += dx;
    y += dy;
  }
}

void DrawRectangle(SDL_Surface *screen, int x, int y, int l, int k,
                   Uint32 outlineColor, Uint32 fillColor) {
  DrawLine(screen, x, y, k, 0, 1, outlineColor);
  DrawLine(screen, x + l - 1, y, k, 0, 1, outlineColor);
  DrawLine(screen, x, y, l, 1, 0, outlineColor);
  DrawLine(screen, x, y + k - 1, l, 1, 0, outlineColor);
  for (int i = y + 1; i < y + k - 1; i++)
    DrawLine(screen, x + 1, i, l - 2, 1, 0, fillColor);
}
