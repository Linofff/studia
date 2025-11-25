#include "./../headers/bird.h"

BIRD *InitBird(WIN *w, int x, int y, int startHealth) {
  BIRD *b = (BIRD *)malloc(sizeof(BIRD));
  b->win = w;
  b->x = x;
  b->y = y;
  b->dx = 1;
  b->dy = 0;
  b->speed = 1;
  b->symbol = '8';
  b->color = BIRD_COLOR;
  b->points = 0;
  b->health = startHealth;
  return b;
}

// --- MOVEMENT & DRAWING ---

void DrawBird(BIRD *b) {
  wattron(b->win->window, COLOR_PAIR(b->color));
  mvwprintw(b->win->window, b->y, b->x, "%c", b->symbol);
  wattron(b->win->window, COLOR_PAIR(b->win->color));
}

void ClearBird(BIRD *b) { mvwprintw(b->win->window, b->y, b->x, " "); }

void MoveBird(BIRD *b) {
  if (!b->is_in_albatros_taxi) {
    ClearBird(b);

    int at_x_boundary = (b->x <= BORDER - 1) || (b->x >= b->win->cols - BORDER);
    int at_y_boundary = (b->y <= BORDER - 1) || (b->y >= b->win->rows - BORDER);

    if (at_x_boundary) {
      if (b->x <= BORDER) {
        b->dx = 1;
      } else if (b->x >= b->win->cols - BORDER - 1) {
        b->dx = -1;
      }
    } else {
      int new_x = b->x + b->dx * b->speed;

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
      if (b->y <= BORDER) {
        b->dy = 1;
      } else if (b->y >= b->win->rows - BORDER - 1) {
        b->dy = -1;
      }
    } else {
      int new_y = b->y + b->dy * b->speed;

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

    DrawBird(b);
  }
}

void ManualMoveBird(BIRD *b, int ch) {
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
