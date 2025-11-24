#include "hunters.h"
void RandomizeShape(HUNTER *hunter) {

  int shape = rand() % 5;
  switch (shape) {
  case 0:
    hunter->width = 1;
    hunter->height = 2;
    break;
  case 1:
    hunter->width = 2;
    hunter->height = 1;
    break;
  case 2:
    hunter->width = 1;
    hunter->height = 3;
    break;
  case 3:
    hunter->width = 3;
    hunter->height = 1;
    break;
  case 4:
    hunter->width = 2;
    hunter->height = 2;
    break;
  }
}

void CalculateDirections(BIRD *bird, HUNTER *hunter, CONFIG cfg) {
  float dx = (float)bird->x - hunter->fx;
  float dy = (float)bird->y - hunter->fy;
  float dist = sqrtf(dx * dx + dy * dy);

  if (dist > 0) {
    hunter->vx = (dx / dist) * cfg.hunter_speed;
    hunter->vy = (dy / dist) * cfg.hunter_speed;
  } else {
    hunter->vx = 0;
    hunter->vy = 0;
  }
}

void SpawnHunter(WIN *w, HUNTER *hunters, BIRD *bird, CONFIG cfg) {
  if ((rand() % 100) >= cfg.hunter_spawn_chance)
    return;

  for (int i = 0; i < cfg.hunter_max; i++) {
    if (!hunters[i].alive) {
      hunters[i].alive = 1;
      hunters[i].damage = cfg.hunter_damage;
      // hunters[i].bounces = cfg.hunter_bounces + ((time(NULL) - startTime) /
      // 20);
      hunters[i].bounces = cfg.hunter_bounces;

      RandomizeShape(&hunters[i]);

      // Pick only LEFT (0) or RIGHT (1)
      int side = rand() % 2;
      int startX, startY;
      int maxY = w->rows - BORDER - hunters[i].height; // Ensure height fits

      if (side == 0) { // Left
        startX = BORDER;
        startY = (rand() % (maxY - BORDER)) + BORDER;
      } else { // Right
        startX =
            w->cols - BORDER - hunters[i].width; // Ensure width fits inside
        startY = (rand() % (maxY - BORDER)) + BORDER;
      }

      // Set positions
      hunters[i].fx = (float)startX;
      hunters[i].fy = (float)startY;
      hunters[i].x = startX;
      hunters[i].y = startY;

      // Calculate vector to bird
      CalculateDirections(bird, &hunters[i], cfg);

      break;
    }
  }
}

int BorderCheck(WIN *w, HUNTER *hunter) {
  int hit = 0;
  if (hunter->fx <= BORDER) {
    hunter->fx = BORDER + 0.1f;
    hunter->vx = -hunter->vx;
    return 1;
  } else if (hunter->fx + hunter->width >= w->cols - BORDER) {
    hunter->fx = w->cols - BORDER - hunter->width - 0.1f;
    hunter->vx = -hunter->vx;
    return 1;
  }

  if (hunter->fy <= BORDER) {
    hunter->fy = BORDER + 0.1f;
    hunter->vy = -hunter->vy;
    return 1;
  } else if (hunter->fy + hunter->height >= w->rows - BORDER) {
    hunter->fy = w->rows - BORDER - hunter->height + 0.1f;
    hunter->vy = -hunter->vy;
    return 1;
  }
  return 0;
}

void ErasePrevHunter(WIN *w, HUNTER *hunter) {

  if (hunter->alive)
    for (int r = 0; r < hunter->height; r++)
      for (int c = 0; c < hunter->width; c++)
        mvwprintw(w->window, hunter->y + r, hunter->x + c, " ");
}
void DrawHunter(WIN *w, HUNTER *hunter) {
  char disp = (hunter->bounces > 9) ? '9' : hunter->bounces + '0';
  for (int r = 0; r < hunter->height; r++)
    for (int c = 0; c < hunter->width; c++)
      if (hunter->y + r < w->rows && hunter->x + c < w->cols)
        mvwprintw(w->window, hunter->y + r, hunter->x + c, "%c", disp);
}

void UpdateHunters(WIN *w, HUNTER *hunters, int maxHunters) {
  wattron(w->window, COLOR_PAIR(HUNTER_COLOR));
  for (int i = 0; i < maxHunters; i++) {
    if (!hunters[i].alive)
      continue;

    ErasePrevHunter(w, &hunters[i]);

    // Move Float
    hunters[i].fx += hunters[i].vx;
    hunters[i].fy += hunters[i].vy;

    // Bounce Check
    int hit = BorderCheck(w, &hunters[i]);

    if (hit) {
      hunters[i].bounces--;
      if (hunters[i].bounces < 0) {
        hunters[i].alive = 0;
        continue;
      }
    }

    // Update Int Pos
    hunters[i].x = (int)hunters[i].fx;
    hunters[i].y = (int)hunters[i].fy;

    // Draw
    DrawHunter(w, &hunters[i]);
  }
  wattroff(w->window, COLOR_PAIR(HUNTER_COLOR));
}
