#include "../headers/drawing.h"
#include <stdio.h>  // For sprintf
#include <string.h> // For strlen

// --- MENU & HELPER FUNCTIONS ---

void DrawMenu(SDL_Renderer *renderer, SDL_Surface *screen, SDL_Texture *scrtex,
              SDL_Surface *charset, const char *title, const char *subtitle) {
  // 1. Clear screen to black
  SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));

  // 2. Draw Text (Center aligned approximation)
  int centerX = SCREEN_WIDTH / 2;
  int centerY = SCREEN_HEIGHT / 2;

  DrawString(screen, centerX - (strlen(title) * 4), centerY - 20, title,
             charset);
  DrawString(screen, centerX - (strlen(subtitle) * 4), centerY + 10, subtitle,
             charset);

  // 3. Update Texture & Render
  SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
  SDL_RenderCopy(renderer, scrtex, NULL, NULL);
  SDL_RenderPresent(renderer);
}

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
  case IN_UP:
    return "^";
  case IN_DOWN:
    return "V";
  case IN_LEFT:
    return "<";
  case IN_RIGHT:
    return ">";
  default:
    return "_";
  }
}

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

void DrawPlayer(SDL_Surface *screen, SDL_Surface *charset, PlayerType *player,
                CameraType *camera, GameState *state, int red, int green) {
  // 3. DRAW PLAYER (ANIMATION LOGIC)
  SDL_Surface *currentSurface = NULL;

  if (player->attackTimer > 0) {
    int idx = player->currentFrame;
    if (idx >= MAX_ATTACK_FRAMES)
      idx = 0;
    if (player->state == DASH) {
      if (player->facingLeft == 1)
        currentSurface = player->dash_frames_left[idx];
      else
        currentSurface = player->dash_frames_rigth[idx];
    } else if (player->state == IDLE) {
    } else if (player->state == JUMPING) {
      if (player->facingLeft == 1)
        currentSurface = player->attack_frames_left[idx];
      else
        currentSurface = player->attack_frames_rigth[idx];
    } else {
      if (player->facingLeft == 1)
        currentSurface = player->attack_frames_left[idx];
      else
        currentSurface = player->attack_frames_rigth[idx];
    }
  } else {
    int idx = player->currentFrame;
    if (idx >= MAX_WALK_FRAMES)
      idx = 0;
    if (player->facingLeft == 1)
      currentSurface = player->walk_frames_left[idx];
    else
      currentSurface = player->walk_frames_right[idx];
  }

  // Fallback if image failed to load

  DrawSurface(screen, currentSurface, player->X - camera->X,
              player->Y - camera->Y);
  DrawSurface(screen, currentSurface, player->X - camera->X,
              player->Y - camera->Y);

  // [DEBUG] Draw Player Hitbox
  int pw = currentSurface->w;
  int ph = currentSurface->h;
  DrawOutline(screen, (int)(player->X - (pw / 2) - camera->X),
              (int)(player->Y - (ph / 2) - camera->Y), pw, ph, green);

  // [DEBUG] Input Buffer & State Text
  if (state->debugMode) {
    char bufferText[256] = "Buffer: ";
    char temp[16];
    for (int i = 0; i < player->buffer.count; i++) {
      sprintf(temp, "%s ", GetInputName(player->buffer.events[i].input));
      strcat(bufferText, temp);
    }
    DrawString(screen, 10, 60, bufferText, charset);

    char stateText[64];
    sprintf(stateText, "Action: %s", GetStateName(player->state));
    DrawString(screen, 10, 76, stateText, charset);
  }

  // [DEBUG] Draw Attack Hitbox (Red)
  if (player->attackTimer > 0) {
    int hitW, hitH, range;

    if (player->state == ATTACK_LIGHT)
      range = 60;
    else if (player->state == COMBO_TRIPLE)
      range = 120;
    else if (player->state == DASH)
      range = 200;
    else
      range = 100;

    // Use current frame dimensions
    int pHalfW = (currentSurface) ? currentSurface->w / 2 : 20;
    int pHalfH = (currentSurface) ? currentSurface->h / 2 : 20;
    double hitX, hitY;

    if (player->direction == UP) {
      hitW = HITBOX_H;
      hitH = range;
      hitX = player->X - (hitW / 2);
      hitY = player->Y - pHalfH - hitH;
    } else if (player->direction == DOWN) {
      hitW = HITBOX_H;
      hitH = range;
      hitX = player->X - (hitW / 2);
      hitY = player->Y + pHalfH;
    } else if (player->direction == LEFT) {
      hitW = range;
      hitH = HITBOX_H;
      hitX = player->X - pHalfW - hitW;
      hitY = player->Y - (hitH / 2);
    } else {
      hitW = range;
      hitH = HITBOX_H;
      hitX = player->X + pHalfW;
      hitY = player->Y - (hitH / 2);
    }

    DrawOutline(screen, (int)(hitX - camera->X), (int)(hitY - camera->Y), hitW,
                hitH, red);
  }
}

// --- MAIN DRAWING LOGIC ---

void DrawGame(SDL_Renderer *renderer, SDL_Surface *screen, SDL_Texture *scrtex,
              SDL_Surface *charset, PlayerType *player, EnemyType *enemies,
              CameraType *camera, GameState *state, double fps) {

  // Define colors
  int black = SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);
  int white = SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF);
  int skyBlue = SDL_MapRGB(screen->format, 0x87, 0xCE, 0xEB);
  int floorGrey = SDL_MapRGB(screen->format, 0x80, 0x80, 0x80);
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

  DrawPlayer(screen, charset, player, camera, state, red, green);

  // 4. Draw Enemies
  // 4. Draw Enemies
  for (int i = 0; i < NUMBER_OF_ENEMIES; i++) {
    if (enemies[i].alive) {

      // 1. Pick the correct frame
      int frameIdx = enemies[i].currentFrame;
      SDL_Surface *eSurf = NULL;
      if (enemies[i].direction == RIGHT)
        eSurf = enemies[i].walk_frames_right[frameIdx];
      else
        eSurf = enemies[i].walk_frames_left[frameIdx];

      // 2. Draw it
      if (eSurf) {
        // Handle Direction:
        // Note: SDL_LoadBMP loads one direction.
        // If you need them to flip left, you should either pre-generate left
        // frames or just draw them as is. For now, we draw the raw frame:

        DrawSurface(screen, eSurf, enemies[i].X - camera->X,
                    enemies[i].Y - camera->Y);

        // [DEBUG] Enemy Hitbox
        int ew = eSurf->w;
        int eh = eSurf->h;
        DrawOutline(screen, (int)(enemies[i].X - (ew / 2) - camera->X),
                    (int)(enemies[i].Y - (eh / 2) - camera->Y), ew, eh, yellow);
      }
    }
  }

  // 5. Draw HUD
  char text[128];
  DrawRectangle(screen, 4, 4, SCREEN_WIDTH - 8, 50, white, black);

  sprintf(text, "Health: %d | Time: %.2f | FPS: %.0f", player->health,
          state->worldTime, fps);
  DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 10, text, charset);

  // --- FLASHY MULTIPLIER UI ---
  // If multiplier scale > 1.2, make it Yellow, otherwise White
  if (player->multiplierScale > 1.2) {
    SDL_SetSurfaceColorMod(charset, 255, 255, 0); // Yellow
  } else {
    SDL_SetSurfaceColorMod(charset, 255, 255, 255); // White
  }

  int uiX = screen->w / 2;
  int uiY = 26;

  // Shake Effect
  if (player->multiplierScale > 1.1) {
    uiX += (rand() % 4) - 2;
    uiY += (rand() % 4) - 2;
  }

  sprintf(text, "SCORE: %d (x%d)", player->score, player->multiplier);
  DrawString(screen, uiX - strlen(text) * 8 / 2, uiY, text, charset);

  // Reset Color
  SDL_SetSurfaceColorMod(charset, 255, 255, 255);
  // --- END FLASHY UI ---

  sprintf(text, "Esc-Quit | WASD-Move | Space-Jump | J-Light | K-Heavy");
  DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 42, text, charset);

  // 6. Push to GPU
  SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
  SDL_RenderCopy(renderer, scrtex, NULL, NULL);
  SDL_RenderPresent(renderer);
}

// --- BASIC DRAWING PRIMITIVES ---

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
  if (!sprite)
    return;
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
