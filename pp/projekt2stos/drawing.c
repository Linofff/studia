#include "drawing.h"

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
  case RUNNING:
    return "RUNNING";
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

void DrawOutline(SDL_Surface *screen, int x, int y, int w, int h,
                 Uint32 color) {
  DrawLine(screen, x, y, h, 0, 1, color);
  DrawLine(screen, x + w - 1, y, h, 0, 1, color);
  DrawLine(screen, x, y, w, 1, 0, color);
  DrawLine(screen, x, y + h - 1, w, 1, 0, color);
}

void DrawStringScaled(SDL_Surface *screen, int x, int y, const char *text,
                      SDL_Surface *charset, int scale) {
  int px, py, c;
  SDL_Rect s, d;
  s.w = 8;
  s.h = 8;
  d.w = 8 * scale;
  d.h = 8 * scale;

  while (*text) {
    c = *text & 255;
    px = (c % 16) * 8;
    py = (c / 16) * 8;
    s.x = px;
    s.y = py;
    d.x = x;
    d.y = y;
    SDL_BlitScaled(charset, &s, screen, &d);
    x += (8 * scale);
    text++;
  }
}

SDL_Surface *GetPlayerSprite(PlayerType *player) {
  int *idx = &player->currentFrame;
  int left = player->facingLeft;

  if (player->state == DEAD) {
    if (*idx >= MAX_DEATH_FRAMES)
      *idx = 0;
    return left ? player->death_frames_left[*idx]
                : player->death_frames_right[*idx];
  }

  if (player->wasHitTimer > 0) {
    if (player->attackTimer > 0) {
      if (*idx >= MAX_ATTACK_FRAMES)
        *idx = 0;
      return left ? player->attack_frames_left[*idx]
                  : player->attack_frames_right[*idx];
    } else {
      if (*idx >= MAX_HIT_FRAMES)
        *idx = 0;
      return left ? player->hit_frames_left[*idx]
                  : player->hit_frames_right[*idx];
    }
  }

  if (player->attackTimer > 0) {
    if (*idx >= MAX_ATTACK_FRAMES)
      *idx = 0;
    if (player->state == DASH) {
      return left ? player->dash_frames_left[*idx]
                  : player->dash_frames_right[*idx];
    }
    if (*idx >= MAX_DASH_FRAMES)
      *idx = 0;
    return left ? player->attack_frames_left[*idx]
                : player->attack_frames_right[*idx];
  }

  if (!player->onGround) {
    if (*idx >= MAX_AIR_FRAMES)
      *idx = 0;
    return left ? player->air_frames_left[*idx]
                : player->air_frames_right[*idx];
  }

  if (player->state == RUNNING) {
    if (*idx >= MAX_WALK_FRAMES)
      *idx = 0;
    return left ? player->walk_frames_left[*idx]
                : player->walk_frames_right[*idx];
  }

  if (*idx >= MAX_IDLE_FRAMES)
    *idx = 0;
  return left ? player->idle_frames_left[*idx]
              : player->idle_frames_right[*idx];
}

void DrawPlayerDebugOverlay(SDL_Surface *screen, SDL_Surface *charset,
                            PlayerType *player) {
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

void DrawPlayerAttackDebug(SDL_Surface *screen, PlayerType *player,
                           CameraType *camera, SDL_Surface *currSurf, int red) {
  if (player->attackTimer <= 0)
    return;

  int range;

  if (player->state == ATTACK_LIGHT)
    range = 60;
  else if (player->state == ATTACK_HEAVY)
    range = 100;
  else if (player->state == COMBO_MIX)
    range = 100;
  else if (player->state == COMBO_TRIPLE)
    range = 120;
  else if (player->state == DASH)
    range = 200;
  else
    range = 100;

  int hitW, hitH;
  int pHalfW = (currSurf) ? currSurf->w / 2 : 20;
  int pHalfH = (currSurf) ? currSurf->h / 2 : 20;
  double hitX, hitY;

  if (player->direction == UP) {
    hitW = ATTACK_HITBOX_H;
    hitH = range;
    hitX = player->X - (hitW / 2);
    hitY = player->Y - pHalfH - hitH;
  } else if (player->direction == DOWN) {
    hitW = ATTACK_HITBOX_H;
    hitH = range;
    hitX = player->X - (hitW / 2);
    hitY = player->Y + pHalfH;
  } else if (player->direction == LEFT) {
    hitW = range;
    hitH = ATTACK_HITBOX_H;
    hitX = player->X - pHalfW - hitW;
    hitY = player->Y - (hitH / 2);
  } else {
    hitW = range;
    hitH = ATTACK_HITBOX_H;
    hitX = player->X + pHalfW;
    hitY = player->Y - (hitH / 2);
  }

  DrawOutline(screen, (int)(hitX - camera->X), (int)(hitY - camera->Y), hitW,
              hitH, red);
}

void DrawPlayer(SDL_Surface *screen, SDL_Surface *charset, PlayerType *player,
                CameraType *camera, GameStateType *gameState, int red,
                int green) {
  SDL_Surface *currentSurface = GetPlayerSprite(player);

  DrawSurface(screen, currentSurface, player->X - camera->X,
              player->Y - camera->Y);

  if (gameState->debugMode) {
    if (currentSurface) {
      int pw = currentSurface->w;
      int ph = currentSurface->h;
      DrawOutline(screen, (int)(player->X - (pw / 2) - camera->X),
                  (int)(player->Y - (ph / 2) - camera->Y), pw, ph, green);
    }
    DrawPlayerDebugOverlay(screen, charset, player);
    DrawPlayerAttackDebug(screen, player, camera, currentSurface, red);
  }
}

// --- ENEMY LOGIC ---

SDL_Surface *GetEnemySprite(EnemyType *enemy) {
  int isLeft = (enemy->direction == 1);
  int idx = enemy->currentFrame;

  if (enemy->stun_timer > 0) {
    return isLeft ? enemy->hit_frames_left[idx] : enemy->hit_frames_right[idx];
  }
  if (enemy->type == 1 && enemy->ai_state == 1) {
    return isLeft ? enemy->dash_frames_left[idx]
                  : enemy->dash_frames_right[idx];
  }
  if (enemy->attack_timer > (ATTACK_COOLDOWN - 0.5)) {
    return isLeft ? enemy->attack_frames_left[idx]
                  : enemy->attack_frames_right[idx];
  }
  return isLeft ? enemy->walk_frames_left[idx] : enemy->walk_frames_right[idx];
}

void DrawSingleEnemy(EnemyType *enemy, SDL_Surface *screen, CameraType *camera,
                     int yellow, int debugMode) {
  if (!enemy->alive)
    return;

  SDL_Surface *eSurf = GetEnemySprite(enemy);

  if (eSurf) {
    DrawSurface(screen, eSurf, enemy->X - camera->X, enemy->Y - camera->Y);
    if (debugMode) {
      DrawOutline(screen, (int)(enemy->X - (eSurf->w / 2) - camera->X),
                  (int)(enemy->Y - (eSurf->h / 2) - camera->Y), eSurf->w,
                  eSurf->h, yellow);
    }
  }
}

void DrawEnemies(EnemyType *enemies, SDL_Surface *screen, CameraType *camera,
                 int yellow, GameStateType *gameState) {
  for (int i = 0; i < NUMBER_OF_ENEMIES; i++) {
    DrawSingleEnemy(&enemies[i], screen, camera, yellow, gameState->debugMode);
  }
}

// --- SCENE & HUD LOGIC ---

void DrawBackground(SDL_Surface *screen, CameraType *camera) {
  int skyBlue = SDL_MapRGB(screen->format, 0x87, 0xCE, 0xEB);
  int floorGrey = SDL_MapRGB(screen->format, 0x80, 0x80, 0x80);
  int black = SDL_MapRGB(screen->format, 0, 0, 0);
  int splitY = LEVEL_HEIGHT / 4;

  DrawRectangle(screen, 0 - camera->X, 0 - camera->Y, LEVEL_WIDTH, splitY,
                skyBlue, skyBlue);
  DrawRectangle(screen, 0 - camera->X, splitY - camera->Y, LEVEL_WIDTH,
                LEVEL_HEIGHT - splitY, floorGrey, floorGrey);
  DrawLine(screen, 0 - camera->X, splitY - camera->Y, LEVEL_WIDTH, 1, 0, black);
}

void DrawHUD(SDL_Surface *screen, SDL_Surface *charset, PlayerType *player,
             GameStateType *gameState, double fps) {
  int black = SDL_MapRGB(screen->format, 0, 0, 0);
  int white = SDL_MapRGB(screen->format, 255, 255, 255);
  char text[128];

  DrawRectangle(screen, 4, 4, SCREEN_WIDTH - 8, 50, white, black);

  if (!gameState->debugMode) {
    sprintf(text, "Health: %d | Time: %.2f",
            player->health > 0 ? player->health : 0, gameState->worldTime);
  } else {
    sprintf(text, "Health: %d | Time: %.2f | FPS: %.0f",
            player->health > 0 ? player->health : 0, gameState->worldTime, fps);
  }
  DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 10, text, charset);

  if (!gameState->debugMode) {
    sprintf(text, "SCORE: %d", player->score);
  } else {
    sprintf(text, "SCORE: %d | Implemented: 1,2,3,4,A,B,C,D,E", player->score);
  }
  DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 26, text, charset);

  sprintf(text, "Esc-Quit | WASD-Move | Space-Jump | J-Light | K-Heavy");
  DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 42, text, charset);
}

void DrawMultiplier(SDL_Surface *screen, SDL_Surface *charset,
                    PlayerType *player) {
  if (player->multiplier <= 1)
    return;

  SDL_SetSurfaceColorMod(charset, 255, 255, 0);

  int multX = screen->w / 2;
  int multY = 70;

  multX += (rand() % 4) - 2;
  multY += (rand() % 4) - 2;

  char text[32];
  sprintf(text, "x%d", player->multiplier);

  int scale = 2;
  DrawStringScaled(screen, multX - (strlen(text) * 8 * scale) / 2, multY, text,
                   charset, scale);

  SDL_SetSurfaceColorMod(charset, 255, 255, 255);
}

// --- MAIN DRAW FUNCTION ---

void DrawGame(SDL_Renderer *renderer, SDL_Surface *screen, SDL_Texture *scrtex,
              SDL_Surface *charset, PlayerType *player, EnemyType *enemies,
              CameraType *camera, GameStateType *gameState, double fps) {

  int black = SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);
  int red = SDL_MapRGB(screen->format, 0xFF, 0x00, 0x00);
  int green = SDL_MapRGB(screen->format, 0x00, 0xFF, 0x00);
  int yellow = SDL_MapRGB(screen->format, 0xFF, 0xFF, 0x00);

  SDL_FillRect(screen, NULL, black);

  DrawBackground(screen, camera);
  DrawPlayer(screen, charset, player, camera, gameState, red, green);
  DrawEnemies(enemies, screen, camera, yellow, gameState);
  DrawHUD(screen, charset, player, gameState, fps);
  DrawMultiplier(screen, charset, player);

  SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
  SDL_RenderCopy(renderer, scrtex, NULL, NULL);
  SDL_RenderPresent(renderer);
}

// --- MENUS ---

void DrawMenu(SDL_Renderer *renderer, SDL_Surface *screen, SDL_Texture *scrtex,
              SDL_Surface *charset, const char *title, const char *subtitle) {
  SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));

  int centerX = SCREEN_WIDTH / 2;
  int centerY = SCREEN_HEIGHT / 2;

  DrawString(screen, centerX - (strlen(title) * 4), centerY - 20, title,
             charset);
  DrawString(screen, centerX - (strlen(subtitle) * 4), centerY + 10, subtitle,
             charset);

  SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
  SDL_RenderCopy(renderer, scrtex, NULL, NULL);
  SDL_RenderPresent(renderer);
}

void DrawGameOver(SDL_Renderer *renderer, SDL_Surface *screen,
                  SDL_Texture *scrtex, SDL_Surface *charset, int score) {
  SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));

  int centerX = SCREEN_WIDTH / 2;
  int centerY = SCREEN_HEIGHT / 2;
  char textBuffer[64];

  const char *title = "GAME OVER";
  DrawString(screen, centerX - (strlen(title) * 4), centerY - 30, title,
             charset);

  sprintf(textBuffer, "FINAL SCORE: %d", score);
  SDL_SetSurfaceColorMod(charset, 255, 255, 0);
  DrawString(screen, centerX - (strlen(textBuffer) * 4), centerY, textBuffer,
             charset);
  SDL_SetSurfaceColorMod(charset, 255, 255, 255);

  const char *sub = "PRESS ENTER TO RESET";
  DrawString(screen, centerX - (strlen(sub) * 4), centerY + 30, sub, charset);

  SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
  SDL_RenderCopy(renderer, scrtex, NULL, NULL);
  SDL_RenderPresent(renderer);
}

// --- PRIMITIVES ---

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
