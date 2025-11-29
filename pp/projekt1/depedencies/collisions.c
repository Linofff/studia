#include "./../headers/collisions.h"
#include "../headers/bird.h"

void CheckCollisionsStar(BIRD *b, STAR *stars, CONFIG cfg,
                         char occupancyMap[COLS][ROWS]) {
  for (int i = 0; i < cfg.star_max; i++) {
    if (stars[i].alive) {
      int sx = (int)stars[i].x;
      int sy = (int)stars[i].y;

      int hit = 0;

      if (b->x == sx && b->y == sy) {
        hit = 1;
      } else if (b->dy == -1 && b->x == sx && sy == (b->y - 1)) {
        hit = 1;
      }

      if (hit) {
        stars[i].alive = 0;
        b->points += 1;

        mvwprintw(b->win->window, sy, sx, " ");
      }
    }
  }
}

int IsHit(int bx, int by, HUNTER *h, char occupancyMap[COLS][ROWS]) {
  return (bx >= h->x && bx < h->x + h->width && by >= h->y &&
          by < h->y + h->height);
}

void CheckCollisionsHunter(BIRD *b, HUNTER *hunters, CONFIG cfg,
                           char occupancyMap[COLS][ROWS]) {
  for (int i = 0; i < cfg.hunter_max; i++) {
    if (hunters[i].alive) {

      int prev_bird_x = b->x - (b->dx * b->speed);
      int prev_bird_y = b->y - (b->dy * b->speed);

      int hit = IsHit(b->x, b->y, &hunters[i], occupancyMap);

      if (!hit) {
        hit = IsHit(prev_bird_x, prev_bird_y, &hunters[i], occupancyMap);
      }

      if (hit) {
        hunters[i].alive = 0;
        b->health -= cfg.hunter_damage;

        // Erase
        for (int r = 0; r < hunters[i].height; r++)
          for (int c = 0; c < hunters[i].width; c++)
            mvwprintw(b->win->window, hunters[i].y + r, hunters[i].x + c, " ");

        flash();
      }
    }
  }
  ChangeColor(b, cfg);
}
