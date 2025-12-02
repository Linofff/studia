#include "./../headers/hunters.h"

void ChangeColorHunter(HUNTER *hunter, CONFIG cfg) {
  int low = cfg.hunter_bounces / 3;
  int medium = low * 2;
  if (hunter->bounces <= medium && hunter->bounces > low)
    hunter->color = MEDIUM_HP_HUNTER;
  else if (hunter->bounces <= low)
    hunter->color = LOW_HP_HUNTER;
}

void ChoseShape(HUNTER *hunter, CONFIG cfg) {

  int variant = rand() % 5;
  hunter->width = cfg.hunter_templates[variant].width;
  hunter->height = cfg.hunter_templates[variant].height;
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

void SpawnHunter(WIN *w, HUNTER *hunters, BIRD *bird, CONFIG cfg,
                 char occupancyMap[ROWS][COLS]) {
  if (!bird->is_in_albatross_taxi && cfg.game_time_elapsed > 2 &&
      bird->albatross_in_cooldown < 3) {
    if ((rand() % 100) >= cfg.hunter_spawn_chance)
      return;

    for (int i = 0; i < cfg.hunter_max; i++) {
      if (!hunters[i].alive) {
        hunters[i].alive = 1;
        hunters[i].damage = cfg.hunter_damage;
        hunters[i].bounces = cfg.hunter_bounces;
        hunters[i].dashleft = 1;
        hunters[i].color = HIGH_HP_HUNTER;

        ChoseShape(&hunters[i], cfg);

        int side = rand() % 2;
        int startX, startY;
        int maxY = w->rows - BORDER - hunters[i].height;

        if (side == 0) {
          startX = BORDER;
          startY = (rand() % (maxY - BORDER)) + BORDER;
        } else {
          startX = w->cols - BORDER - hunters[i].width;
          startY = (rand() % (maxY - BORDER)) + BORDER;
        }

        hunters[i].fx = (float)startX;
        hunters[i].fy = (float)startY;
        hunters[i].x = startX;
        hunters[i].y = startY;

        hunters[i].initial_bird_x = bird->x;
        hunters[i].initial_bird_y = bird->y;

        CalculateDirections(bird, &hunters[i], cfg);

        for (int r = 0; r < hunters[i].height; r++) {
          for (int c = 0; c < hunters[i].width; c++) {
            occupancyMap[hunters[i].y + r][hunters[i].x + c] = 'h';
          }
        }

        break;
      }
    }
  }
}

void HunterSleep(HUNTER *hunter, BIRD *bird, CONFIG cfg) {
  if (cfg.game_time_elapsed >= hunter->sleep_timer) {

    CalculateDirections(bird, hunter, cfg);

    hunter->vx *= 2;
    hunter->vy *= 2;

    hunter->boost_timer = cfg.game_time_elapsed + 1;

    hunter->sleep_timer = 0;

  } else {
    hunter->vx = 0;
    hunter->vy = 0;
  }
}

void HunterDash(WIN *w, HUNTER *hunters, BIRD *bird, CONFIG cfg) {

  for (int i = 0; i < cfg.hunter_max; i++) {
    HUNTER *h = &hunters[i];

    if (h->sleep_timer > 0) {
      if (cfg.game_time_elapsed < h->sleep_timer) {
        h->vx = 0;
        h->vy = 0;
        continue;
      } else {
        HunterSleep(h, bird, cfg);
      }
    }

    if (h->boost_timer > 0 && cfg.game_time_elapsed >= h->boost_timer) {
      h->vx /= 2;
      h->vy /= 2;
      h->boost_timer = 0;
    }

    int targetX = h->initial_bird_x;
    int targetY = h->initial_bird_y;

    int hitX = (targetX >= h->x) && (targetX <= (h->x + h->width));
    int hitY = (targetY >= h->y) && (targetY <= (h->y + h->height));

    if (hitX && hitY && h->dashleft > 0) {

      if (h->boost_timer > 0) {
        h->vx /= 2;
        h->vy /= 2;
        h->boost_timer = 0;
      }

      h->sleep_timer = cfg.game_time_elapsed + 1;

      h->dashleft--;

      h->vx = 0;
      h->vy = 0;
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

void EraseHunter(WIN *w, HUNTER *hunter, char occupancyMap[ROWS][COLS]) {
  if (hunter->alive)
    for (int r = 0; r < hunter->height; r++)
      for (int c = 0; c < hunter->width; c++) {
        mvwprintw(w->window, hunter->y + r, hunter->x + c, " ");
        occupancyMap[hunter->y + r][hunter->x + c] = ' ';
      }
}

void DrawHunter(WIN *w, HUNTER *hunter, char occupancyMap[ROWS][COLS]) {
  wattron(w->window, COLOR_PAIR(hunter->color));
  char disp = (hunter->bounces > 9) ? '9' : hunter->bounces + '0';
  for (int r = 0; r < hunter->height; r++)
    for (int c = 0; c < hunter->width; c++)
      if (hunter->y + r < w->rows && hunter->x + c < w->cols) {
        mvwprintw(w->window, hunter->y + r, hunter->x + c, "%c", disp);
        occupancyMap[hunter->y + r][hunter->x + c] = 'h';
      }
  wattroff(w->window, COLOR_PAIR(hunter->color));
}

void FindWhichStarHunters(WIN *w, int x, int y, STAR *stars, const CONFIG *cfg,
                          char occupancyMap[ROWS][COLS]) {
  for (int i = 0; i < cfg->star_max; i++) {
    if (stars[i].x == x && stars[i].y == y) {
      mvwprintw(w->window, stars[i].y, stars[i].x, " ");
      occupancyMap[stars[i].y][stars[i].x] = ' ';
      stars[i].alive = 0;
    }
  }
}

void CollisionTypeReaction(int hit_type, int tempX, int tempY, STAR *stars,
                           char occupancyMap[ROWS][COLS], CONFIG *cfg,
                           BIRD *bird, HUNTER *hunter, WIN *w) {
  if (hit_type == 1) {
    // Hit Bird -> Kill Hunter, Damage Bird
    bird->health -= cfg->hunter_damage;
    hunter->alive = 0;
    flash();
  } else if (hit_type == 2) {
    // CASE 2: Hit a STAR
    FindWhichStarHunters(w, tempX, tempY, stars, cfg, occupancyMap);
    DrawHunter(w, hunter, occupancyMap);
  } else if (hit_type == 3) {
    // Hit Hunter -> BOUNCE
    // 1. Revert position to prevent sticking
    hunter->fx -= hunter->vx;
    hunter->fy -= hunter->vy;
    hunter->x = (int)hunter->fx;
    hunter->y = (int)hunter->fy;

    // 2. Reverse Direction
    hunter->vx = -hunter->vx;
    hunter->vy = -hunter->vy;

    // 3. Draw immediately so it doesn't flicker
    DrawHunter(w, hunter, occupancyMap);
  } else {
    // Hit Nothing -> Draw normally
    DrawHunter(w, hunter, occupancyMap);
  }
}

void CollsionCheck(HUNTER *hunter, char occupancyMap[ROWS][COLS], CONFIG cfg,
                   STAR *stars, BIRD *bird, WIN *w) {

  int prevx = hunter->x;
  int prevy = hunter->y;

  int hit_type = 0;

  int dx = hunter->x - prevx;
  int dy = hunter->y - prevy;

  int steps = (abs(dx) > abs(dy)) ? abs(dx) : abs(dy);
  int tempX, tempY;
  if (steps == 0)
    steps = 1;

  for (int s = 1; s <= steps; s++) {
    float t = (float)s / (float)steps;
    int checkX = prevx + (int)(dx * t);
    int checkY = prevy + (int)(dy * t);

    for (int r = 0; r < hunter->height; r++) {
      for (int c = 0; c < hunter->width; c++) {

        int mapY = checkY + r;
        int mapX = checkX + c;

        if (mapY >= 0 && mapY < ROWS && mapX >= 0 && mapX < COLS) {

          char cell = occupancyMap[mapY][mapX];

          if (cell == 'b') {
            hit_type = 1;
            break;
          } else if (cell == 's') {
            hit_type = 2;
            tempX = mapX;
            tempY = mapY;
            break;
          } else if (cell == 'h') {
            hit_type = 3;
            break;
          }
        }
      }
      if (hit_type)
        break;
    }
    if (hit_type)
      break;
  }
  CollisionTypeReaction(hit_type, tempX, tempY, stars, occupancyMap, &cfg, bird,
                        hunter, w);
}

void UpdateHunters(WIN *w, HUNTER *hunters, int maxHunters, BIRD *bird,
                   const CONFIG cfg, char occupancyMap[ROWS][COLS],
                   STAR *stars) {
  for (int i = 0; i < maxHunters; i++) {
    ChangeColorHunter(&hunters[i], cfg);
    if (!hunters[i].alive)
      continue;

    // 1. Erase from old position (Crucial: Now 'h' on map is ONLY other
    // hunters)
    EraseHunter(w, &hunters[i], occupancyMap);

    // 2. Logic
    HunterDash(w, &hunters[i], bird, cfg);

    hunters[i].fx += hunters[i].vx;
    hunters[i].fy += hunters[i].vy;

    // 3. Wall Check
    int hit = BorderCheck(w, &hunters[i]);
    if (hit) {
      hunters[i].bounces--;
      if (hunters[i].bounces < 1) {
        hunters[i].alive = 0;
        continue;
      }
    }

    hunters[i].x = (int)hunters[i].fx;
    hunters[i].y = (int)hunters[i].fy;

    CollsionCheck(&hunters[i], occupancyMap, cfg, stars, bird, w);
  }
}
