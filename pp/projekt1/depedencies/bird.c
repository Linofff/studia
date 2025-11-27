#include "./../headers/bird.h"
#include <stdio.h>

BIRD *InitBird(WIN *w, int x, int y, int startHealth) {
  BIRD *b = (BIRD *)malloc(sizeof(BIRD));
  b->win = w;
  b->x = x;
  b->y = y;
  b->dx = 1;
  b->dy = 0;
  b->speed = 1;
  b->symbol = '8';
  b->color = BIRD_COLOR1;
  b->points = 0;
  b->health = startHealth;
  b->albatross_taxi_left = 3;
  return b;
}

// --- MOVEMENT & DRAWING ---

void ChangeColor(BIRD *bird, CONFIG cfg) {
  int low = cfg.start_health / 3;
  int medium = low * 2;
  if (bird->health <= medium && bird->health > low)
    bird->color = BIRD_COLOR2;
  else if (bird->health <= low)
    bird->color = BIRD_COLOR3;
  // if (bird->health <= 2)
  //   bird->color = BIRD_COLOR2;
}

void DrawBird(BIRD *b) {
  wattron(b->win->window, COLOR_PAIR(b->color));
  mvwprintw(b->win->window, b->y, b->x, "%c", b->symbol);
  wattron(b->win->window, COLOR_PAIR(b->win->color));
}

void MoveToCenter(BIRD *bird) {
  float target_x = (float)COLS / 2.0f;
  float target_y = (float)ROWS / 2.0f;

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

void ClearBird(BIRD *b) { mvwprintw(b->win->window, b->y, b->x, " "); }

void MoveBird(BIRD *b) {
  ClearBird(b);

  if (b->is_in_albatross_taxi) {
    MoveToCenter(b);

    b->x += (int)round(b->dx);
    b->y += (int)round(b->dy);
  } else {
    int at_x_boundary = (b->x <= BORDER - 1) || (b->x >= b->win->cols - BORDER);
    int at_y_boundary = (b->y <= BORDER - 1) || (b->y >= b->win->rows - BORDER);

    if (at_x_boundary) {
      if (b->x <= BORDER)
        b->dx = 1;
      else if (b->x >= b->win->cols - BORDER - 1)
        b->dx = -1;
    } else {
      int new_x = b->x + (int)(b->dx * b->speed);

      if (new_x <= BORDER) {
        b->x = BORDER;
        b->dx = 1;
      } else if (new_x >= b->win->cols - BORDER - 1) {
        b->x = b->win->cols - BORDER - 1;
        b->dx = -1;
      } else {
        b->x = new_x;
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

  DrawBird(b);
}

void ManualMoveBird(BIRD *b, int ch) {
  if (!b->is_in_albatross_taxi) {
    ClearBird(b);
    if (ch == UP) {
      b->dy = -1;
      b->dx = 0;
      b->y--;
    }
    if (ch == DOWN) {
      b->dy = 1;
      b->dx = 0;
      b->y++;
    }
    if (ch == LEFT) {
      b->dx = -1;
      b->dy = 0;
      b->x--;
    }
    if (ch == RIGHT) {
      b->dx = 1;
      b->dy = 0;
      b->x++;
    }

    if (b->x < BORDER)
      b->x = BORDER;
    if (b->x > b->win->cols - BORDER - 1)
      b->x = b->win->cols - BORDER - 1;
    if (b->y < BORDER)
      b->y = BORDER;
    if (b->y > b->win->rows - BORDER - 1)
      b->y = b->win->rows - BORDER - 1;
    DrawBird(b);
  }
}
