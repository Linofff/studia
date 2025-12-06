#include "../headers/hunters.h"

void ChangeColorHunter(HUNTER *hunter, CONFIG cfg) {
  const int low = cfg.level.hunter_bounces / 3;
  const int medium = low * 2;
  if (hunter->bounces <= medium && hunter->bounces > low)
    hunter->color = MEDIUM_HP_HUNTER;
  else if (hunter->bounces <= low)
    hunter->color = LOW_HP_HUNTER;
}

void ChoseShape(HUNTER *hunter, CONFIG cfg) {
  const int variant = rand() % 5;
  hunter->width = cfg.hunter_templates[variant].width;
  hunter->height = cfg.hunter_templates[variant].height;
}

void CalculateDirections(const BIRD *bird, HUNTER *hunter, const CONFIG cfg) {
  const float dx = (float)bird->x - hunter->fx;
  const float dy = (float)bird->y - hunter->fy;
  const float dist = sqrtf(dx * dx + dy * dy);

  if (dist > 0) {
    hunter->vx = (dx / dist) * cfg.level.hunter_speed;
    hunter->vy = (dy / dist) * cfg.level.hunter_speed;
  } else {
    hunter->vx = 0;
    hunter->vy = 0;
  }
}

void InitAndPlaceHunter(WIN *w, HUNTER *h, BIRD *bird, CONFIG cfg,
                        const int rows, const int cols,
                        char occupancyMap[rows][cols]) {
  h->alive = 1;
  h->damage = cfg.level.hunter_damage;
  h->bounces = cfg.level.hunter_bounces;
  h->dashes_left = 1;
  h->color = HIGH_HP_HUNTER;

  ChoseShape(h, cfg);

  int startX = 0, startY = 0;
  const int maxY = w->rows - BORDER - h->height;
  int searching = 1;

  while (searching) {
    const int side = rand() % 2;
    startY = (rand() % (maxY - BORDER) + BORDER);

    if (side == 0)
      startX = BORDER + cfg.fog_currentsize;
    else
      startX = w->cols - BORDER - h->width - cfg.fog_currentsize;

    for (int r = 0; r < h->height; r++) {
      for (int c = 0; c < h->width; c++) {
        if (occupancyMap[startY + r][startX + c] != 'h' &&
            occupancyMap[startY + r][startX + c] != 'b') {
          searching = 0;
        }
      }
    }
    searching = 0;
  }

  h->fx = (float)startX;
  h->fy = (float)startY;
  h->x = startX;
  h->y = startY;
  h->initial_bird_x = bird->x;
  h->initial_bird_y = bird->y;

  CalculateDirections(bird, h, cfg);
  DrawHunter(w, h, rows, cols, occupancyMap);
}

void SpawnHunter(WIN *w, HUNTER *hunters, BIRD *bird, CONFIG cfg,
                 const int rows, const int cols,
                 char occupancyMap[rows][cols]) {
  if (bird->is_in_albatross_taxi || cfg.game_time_elapsed <= 2 ||
      bird->albatross_in_cooldown >= 3)
    return;

  if ((rand() % 100) >= cfg.level.hunter_spawn_chance)
    return;

  for (int i = 0; i < cfg.level.hunter_max; i++) {
    if (!hunters[i].alive) {
      InitAndPlaceHunter(w, &hunters[i], bird, cfg, rows, cols, occupancyMap);
      break;
    }
  }
}

void HunterTriggerDash(HUNTER *hunter, BIRD *bird, CONFIG cfg) {

  CalculateDirections(bird, hunter, cfg);

  hunter->vx *= 2;
  hunter->vy *= 2;

  hunter->boost_timer = cfg.game_time_elapsed + 1;

  hunter->sleep_timer = 0;
}

void UpdateDashingHunters(HUNTER *hunters, BIRD *bird, CONFIG cfg) {
  if (hunters == NULL)
    return;

  for (int i = 0; i < cfg.level.hunter_max; i++) {
    HUNTER *h = &hunters[i];

    if (!h->alive || h == NULL)
      continue;

    if (h->sleep_timer > 0) {
      if (cfg.game_time_elapsed < h->sleep_timer) {
        h->vx = 0;
        h->vy = 0;
        continue;
      }

      HunterTriggerDash(h, bird, cfg);
    }

    if (h->boost_timer > 0 && cfg.game_time_elapsed >= h->boost_timer) {
      h->vx /= 2;
      h->vy /= 2;
      h->boost_timer = 0;
    }

    const int targetX = h->initial_bird_x;
    const int targetY = h->initial_bird_y;

    const bool hitX = (targetX >= h->x) && (targetX <= (h->x + h->width));
    const bool hitY = (targetY >= h->y) && (targetY <= (h->y + h->height));

    if (hitX && hitY && h->dashes_left > 0) {
      if (h->boost_timer > 0) {
        h->vx /= 2;
        h->vy /= 2;
        h->boost_timer = 0;
      }

      h->sleep_timer = cfg.game_time_elapsed + 1;
      h->dashes_left--;

      h->vx = 0;
      h->vy = 0;
    }
  }
}

int BorderCheck(WIN *w, HUNTER *hunter) {
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

void EraseHunter(WIN *w, HUNTER *hunter, const int rows, const int cols,
                 char occupancyMap[rows][cols]) {
  if (hunter->alive)
    for (int r = 0; r < hunter->height; r++)
      for (int c = 0; c < hunter->width; c++) {
        mvwprintw(w->window, hunter->y + r, hunter->x + c, " ");
        occupancyMap[hunter->y + r][hunter->x + c] = ' ';
      }
}

void DrawHunter(WIN *w, HUNTER *hunter, const int rows, const int cols,
                char occupancyMap[rows][cols]) {
  wattron(w->window, COLOR_PAIR(hunter->color));
  const char bounces = (hunter->bounces > 9) ? '9' : hunter->bounces + '0';
  for (int r = 0; r < hunter->height; r++)
    for (int c = 0; c < hunter->width; c++)
      if (hunter->y + r < w->rows && hunter->x + c < w->cols) {
        mvwprintw(w->window, hunter->y + r, hunter->x + c, "%c", bounces);
        occupancyMap[hunter->y + r][hunter->x + c] = 'h';
      }
  wattroff(w->window, COLOR_PAIR(hunter->color));
}

void FindWhichStarHunters(WIN *w, int x, int y, STAR *stars, const CONFIG *cfg,
                          const int rows, const int cols,
                          char occupancyMap[rows][cols]) {
  for (int i = 0; i < cfg->level.star_max; i++) {
    if (stars[i].x == x && stars[i].y == y) {
      mvwprintw(w->window, stars[i].y, stars[i].x, " ");
      occupancyMap[stars[i].y][stars[i].x] = ' ';
      stars[i].alive = 0;
    }
  }
}

void CollisionTypeReaction(int hit_type, int tempX, int tempY, STAR *stars,
                           const int rows, const int cols,
                           char occupancyMap[rows][cols], CONFIG *cfg,
                           BIRD *bird, HUNTER *hunter, WIN *w) {
  if (hit_type == HIT_BIRD) {
    bird->health -= cfg->level.hunter_damage;
    hunter->alive = 0;
    // flash();
  } else if (hit_type == HIT_STAR) {
    FindWhichStarHunters(w, tempX, tempY, stars, cfg, rows, cols, occupancyMap);
    DrawHunter(w, hunter, rows, cols, occupancyMap);
  } else if (hit_type == HIT_HUNTER || hit_type == HIT_WALL) {
    hunter->fx -= hunter->vx;
    hunter->fy -= hunter->vy;
    hunter->x = (int)hunter->fx;
    hunter->y = (int)hunter->fy;

    hunter->vx = -hunter->vx;
    hunter->vy = -hunter->vy;

    if (hit_type == HIT_WALL) {
      hunter->bounces--;
      if (hunter->bounces < 1) {
        hunter->alive = 0;
        EraseHunter(w, hunter, rows, cols, occupancyMap);
      }
    }

    if (hit_type != HIT_WALL)
      DrawHunter(w, hunter, rows, cols, occupancyMap);
  } else {
    DrawHunter(w, hunter, rows, cols, occupancyMap);
  }
}

void CollsionCheck(HUNTER *hunter, const int rows, const int cols,
                   char occupancyMap[rows][cols], CONFIG cfg, STAR *stars,
                   BIRD *bird, WIN *w) {

  const int prevx = hunter->x;
  const int prevy = hunter->y;

  int hit_type = 0;

  const int dx = hunter->x - prevx;
  const int dy = hunter->y - prevy;

  int steps = (abs(dx) > abs(dy)) ? abs(dx) : abs(dy);
  int tempX = 0, tempY = 0;
  if (steps == 0)
    steps = 1;

  for (int s = 1; s <= steps; s++) {
    const float t = (float)s / (float)steps;
    const int checkX = prevx + (int)(dx * t);
    const int checkY = prevy + (int)(dy * t);

    for (int r = 0; r < hunter->height; r++) {
      for (int c = 0; c < hunter->width; c++) {

        const int mapY = checkY + r;
        const int mapX = checkX + c;

        if (mapY >= 0 && mapY < rows && mapX >= 0 && mapX < cols) {

          const char cell = occupancyMap[mapY][mapX];

          if (cell == 'b') {
            hit_type = HIT_BIRD;
            break;
          } else if (cell == 's') {
            hit_type = HIT_STAR;
            tempX = mapX;
            tempY = mapY;
            break;
          } else if (cell == 'h') {
            hit_type = HIT_HUNTER;
            break;
          } else if (cell == '#') {
            hit_type = HIT_WALL;
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
  CollisionTypeReaction(hit_type, tempX, tempY, stars, rows, cols, occupancyMap,
                        &cfg, bird, hunter, w);
}

void UpdateHunters(WIN *w, HUNTER *hunters, int maxHunters, BIRD *bird,
                   const CONFIG cfg, const int rows, const int cols,
                   char occupancyMap[rows][cols], STAR *stars) {
  for (int i = 0; i < maxHunters; i++) {
    ChangeColorHunter(&hunters[i], cfg);
    if (!hunters[i].alive)
      continue;

    EraseHunter(w, &hunters[i], rows, cols, occupancyMap);

    UpdateDashingHunters(hunters, bird, cfg);

    hunters[i].fx += hunters[i].vx;
    hunters[i].fy += hunters[i].vy;

    const int hit = BorderCheck(w, &hunters[i]);
    if (hit) {
      hunters[i].bounces--;
      if (hunters[i].bounces < 1) {
        hunters[i].alive = 0;
        continue;
      }
    }

    hunters[i].x = (int)hunters[i].fx;
    hunters[i].y = (int)hunters[i].fy;

    CollsionCheck(&hunters[i], rows, cols, occupancyMap, cfg, stars, bird, w);
  }
}
