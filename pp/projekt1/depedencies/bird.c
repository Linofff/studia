#include "./../headers/bird.h"
#include "./../headers/hunters.h"

BIRD *InitBird(WIN *w, int x, int y, int startHealth, int rows, int cols,
               char occupancyMap[rows][cols]) {
  BIRD *b = (BIRD *)malloc(sizeof(BIRD));
  b->win = w;
  b->x = x;
  b->y = y;
  b->dx = 0;
  b->dy = 0;
  b->speed = 1;
  b->symbol = '|';
  b->color = HIGH_HP_BIRD;
  b->points = 0;
  b->health = startHealth;
  b->albatross_taxi_left = 3;
  b->was_in_taxi = 0;

  occupancyMap[b->y][b->x] = 'b';
  return b;
}

void ChangeColorBird(BIRD *bird, CONFIG cfg) {
  int low = cfg.start_health / 3;
  int medium = low * 2;
  if (bird->health <= medium && bird->health > low)
    bird->color = MEDIUM_HP_BIRD;
  else if (bird->health <= low)
    bird->color = LOW_HP_BIRD;
}

void ChangeShape(BIRD *bird, CONFIG cfg) {
  if (!bird->is_in_albatross_taxi) {
    if (cfg.framecounter % 10 < 3)
      bird->symbol = '-';
    else if (bird->dy == -1)
      bird->symbol = 'A';
    else if (bird->dy == 1)
      bird->symbol = 'V';
    else if (bird->dx == 1)
      bird->symbol = '>';
    else if (bird->dx == -1)
      bird->symbol = '<';
  } else {
    bird->symbol = 'T';
  }
}

void DrawBird(BIRD *b, int rows, int cols, char occupancyMap[rows][cols]) {
  wattron(b->win->window, COLOR_PAIR(b->color));
  mvwprintw(b->win->window, b->y, b->x, "%c", b->symbol);
  wattron(b->win->window, COLOR_PAIR(b->win->color));
  occupancyMap[b->y][b->x] = 'b';
}

void MoveToCenter(BIRD *bird, int rows, int cols) {
  float target_x = (float)cols / 2.0f;
  float target_y = (float)rows / 2.0f;

  float dx = target_x - (float)bird->x;
  float dy = target_y - (float)bird->y;

  float dist = sqrtf(dx * dx + dy * dy);

  if (dist > 0) {
    float speed = (float)bird->speed;

    bird->dx = (dx / dist) * speed;
    bird->dy = (dy / dist) * speed;
  } else {
    bird->x = (int)target_x;
    bird->y = (int)target_y;
    bird->dx = 0;
    bird->dy = 0;
    bird->albatross_out_cooldown = 0;
  }
}

void ClearBird(BIRD *b, int rows, int cols, char occupancyMap[rows][cols]) {
  occupancyMap[b->y][b->x] = ' ';
  mvwprintw(b->win->window, b->y, b->x, " ");
}

void FindWhichStar(BIRD *b, STAR *stars, CONFIG *cfg) {
  for (int i = 0; i < cfg->level.star_max; i++) {
    if (stars[i].x == b->x && stars[i].y == b->y)
      stars[i].alive = 0;
  }
}

void FindWhichHunter(BIRD *b, HUNTER *hunters, CONFIG *cfg, int rows, int cols,
                     char occupancyMap[rows][cols], WIN *playwin) {
  for (int i = 0; i < cfg->level.hunter_max; i++) {
    for (int r = 0; r < hunters[i].height; r++)
      for (int c = 0; c < hunters[i].width; c++) {
        if ((hunters[i].x + c) == b->x && (hunters[i].y + r) == b->y) {
          EraseHunter(playwin, &hunters[i], rows, cols, occupancyMap);
          hunters[i].alive = 0;
        }
      }
  }
}

void BirdBorderCheck(int at_x_boundary, int at_y_boundary, BIRD *b, int rows,
                     int cols, char occupancyMap[rows][cols]) {

  if (at_x_boundary) {
    if (b->x <= BORDER)
      b->dx = 1;
    else if (b->x >= b->win->cols - BORDER - 1)
      b->dx = -1;
  } else {
    int dir_x = (b->dx > 0) ? 1 : -1;

    if (occupancyMap[b->y][b->x + dir_x] == '#') {
      b->dx *= -1;
    } else {
      int new_x = b->x + (int)(b->dx * b->speed);

      if (new_x <= BORDER) {
        b->x = BORDER;
        b->dx = 1;
      } else if (new_x >= b->win->cols - BORDER - 1) {
        b->x = b->win->cols - BORDER - 1;
        b->dx = -1;
      } else if (occupancyMap[b->y][new_x] == '#') {
        b->dx *= -1;
      } else {
        b->x = new_x;
      }
    }
  }

  if (at_y_boundary) {
    if (b->y <= BORDER)
      b->dy = 1;
    else if (b->y >= b->win->rows - BORDER - 1)
      b->dy = -1;
  } else {
    int new_y = b->y + (int)(b->dy * b->speed);

    if (new_y <= BORDER) {
      b->y = BORDER;
      b->dy = 1;
    } else if (new_y >= b->win->rows - BORDER - 1) {
      b->y = b->win->rows - BORDER - 1;
      b->dy = -1;
    } else {
      b->y = new_y;
    }
  }
}

void MoveBird(BIRD *b, int rows, int cols, char occupancyMap[rows][cols],
              STAR *stars, HUNTER *hunters, CONFIG *cfg, WIN *playwin) {
  ClearBird(b, rows, cols, occupancyMap);
  ChangeColorBird(b, *cfg);

  if (b->is_in_albatross_taxi) {
    MoveToCenter(b, rows, cols);
    ChangeShape(b, *cfg);

    b->x += (int)round(b->dx);
    b->y += (int)round(b->dy);
  } else {
    int at_x_boundary = (b->x <= BORDER - 1) ||
                        (b->x >= b->win->cols - BORDER) ||
                        (occupancyMap[b->y][b->x] == '#');
    int at_y_boundary = (b->y <= BORDER - 1) ||
                        (b->y >= b->win->rows - BORDER) ||
                        (occupancyMap[b->y][b->x] == '#');

    BirdBorderCheck(at_x_boundary, at_y_boundary, b, rows, cols, occupancyMap);

    ChangeShape(b, *cfg);
  }

  if (occupancyMap[b->y][b->x] == 's') {
    b->points++;
    FindWhichStar(b, stars, cfg);
  }
  if (occupancyMap[b->y][b->x] == 'h') {
    b->health -= hunters->damage;
    FindWhichHunter(b, hunters, cfg, rows, cols, occupancyMap, playwin);
    // flash();
  }
  if (occupancyMap[b->y][b->x] == '#') {
    if (b->x < COLS / 2)
      b->x++;
    if (b->x > COLS / 2)
      b->x--;
  }

  DrawBird(b, rows, cols, occupancyMap);
}

void ChangeDirectionBird(BIRD *b, int ch, int rows, int cols,
                         char occupancyMap[rows][cols], STAR *stars,
                         HUNTER *hunters, CONFIG *cfg, WIN *playwin) {
  if (!b->is_in_albatross_taxi) {
    if (ch == UP) {
      b->dy = -1;
      b->dx = 0;
    }
    if (ch == DOWN) {
      b->dy = 1;
      b->dx = 0;
    }
    if (ch == LEFT) {
      b->dx = -1;
      b->dy = 0;
    }
    if (ch == RIGHT) {
      b->dx = 1;
      b->dy = 0;
    }
    if (ch == STOP) {
      b->dx = 0;
      b->dy = 0;
    }

    ChangeShape(b, *cfg);
    MoveBird(b, rows, cols, occupancyMap, stars, hunters, cfg, playwin);
  }
}
