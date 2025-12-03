#include "./../headers/fog.h"

void InitFog(CONFIG *cfg) {
  cfg->fog_min_x = 1;
  cfg->fog_max_x = COLS - 2;
  cfg->fog_timer = 0;

  cfg->fog_update_interval = 40;
}

void UpdateFog(CONFIG *cfg, WIN *playwin, BIRD *bird) {
  if (bird->is_in_albatross_taxi)
    return;

  cfg->fog_timer++;

  if (cfg->fog_timer >= cfg->fog_update_interval) {
    cfg->fog_timer = 0;

    int safe_zone_width = 40;
    int left_limit = (COLS - safe_zone_width) / 2;
    int right_limit = (COLS + safe_zone_width) / 2;

    if (cfg->fog_min_x < left_limit) {
      cfg->fog_min_x++;
      cfg->fog_currentsize++;
    }
    if (cfg->fog_max_x > right_limit) {
      cfg->fog_max_x--;
    }
  }
}
void DrawFog(WIN *playwin, CONFIG *cfg, char occupancyMap[ROWS][COLS]) {
  wattron(playwin->window, COLOR_PAIR(FOG_COLOR_PAIR));

  for (int y = 1; y < ROWS - 1; y++) {
    for (int x = 1; x < cfg->fog_min_x; x++) {
      occupancyMap[y][x] = '#';
      mvwaddch(playwin->window, y, x, '#');
    }
  }

  for (int y = 1; y < ROWS - 1; y++) {
    for (int x = cfg->fog_max_x + 1; x < COLS - 1; x++) {
      occupancyMap[y][x] = '#';
      mvwaddch(playwin->window, y, x, '#');
    }
  }

  wattroff(playwin->window, COLOR_PAIR(FOG_COLOR_PAIR));
}
