#include "./../headers/hunters.h"

void ChoseShape(HUNTER *hunter, CONFIG cfg) {

  int variant = rand() % 5;
  switch (variant) {
  case 0:
    hunter->width = cfg.hunter_templates[0].width;
    hunter->height = cfg.hunter_templates[0].height;
    break;
  case 1:
    hunter->width = cfg.hunter_templates[1].width;
    hunter->height = cfg.hunter_templates[1].height;
    break;
  case 2:
    hunter->width = cfg.hunter_templates[2].width;
    hunter->height = cfg.hunter_templates[2].height;
    break;
  case 3:
    hunter->width = cfg.hunter_templates[3].width;
    hunter->height = cfg.hunter_templates[3].height;
    break;
  case 4:
    hunter->width = cfg.hunter_templates[4].width;
    hunter->height = cfg.hunter_templates[4].height;
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

void SpawnHunter(WIN *w, HUNTER *hunters, BIRD *bird, CONFIG cfg,
                 char occupancyMap[ROWS][COLS]) {
  if (!bird->is_in_albatross_taxi) {
    if ((rand() % 100) >= cfg.hunter_spawn_chance)
      return;

    for (int i = 0; i < cfg.hunter_max; i++) {
      if (!hunters[i].alive) {
        hunters[i].alive = 1;
        hunters[i].damage = cfg.hunter_damage;
        hunters[i].bounces = cfg.hunter_bounces;
        hunters[i].dashleft = 1;

        ChoseShape(&hunters[i], cfg);

        int side = rand() % 2;
        int startX, startY;
        int maxY = w->rows - BORDER - hunters[i].height;

        if (side == 0) { // Left
          startX = BORDER;
          startY = (rand() % (maxY - BORDER)) + BORDER;
        } else { // Right
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
  char disp = (hunter->bounces > 9) ? '9' : hunter->bounces + '0';
  for (int r = 0; r < hunter->height; r++)
    for (int c = 0; c < hunter->width; c++)
      if (hunter->y + r < w->rows && hunter->x + c < w->cols) {
        mvwprintw(w->window, hunter->y + r, hunter->x + c, "%c", disp);
        occupancyMap[hunter->y + r][hunter->x + c] = 'h';
      }
}

void UpdateHunters(WIN *w, HUNTER *hunters, int maxHunters, BIRD *bird,
                   const CONFIG cfg, char occupancyMap[ROWS][COLS]) {
  wattron(w->window, COLOR_PAIR(HUNTER_COLOR));
  for (int i = 0; i < maxHunters; i++) {
    if (!hunters[i].alive)
      continue;

    EraseHunter(w, &hunters[i], occupancyMap);

    HunterDash(w, &hunters[i], bird, cfg);

    hunters[i].fx += hunters[i].vx;
    hunters[i].fy += hunters[i].vy;

    int hit = BorderCheck(w, &hunters[i]);

    if (hit) {
      hunters[i].bounces--;
      if (hunters[i].bounces < 0) {
        hunters[i].alive = 0;
        continue;
      }
    }

    hunters[i].x = (int)hunters[i].fx;
    hunters[i].y = (int)hunters[i].fy;

    DrawHunter(w, &hunters[i], occupancyMap);
  }
  wattroff(w->window, COLOR_PAIR(HUNTER_COLOR));
}
