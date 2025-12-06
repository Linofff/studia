#include "fog.h"

void InitFog(CONFIG *cfg, const int cols) {
  cfg->fog_min_x = 1;
  cfg->fog_max_x = cols - 2;
  cfg->fog_timer = 0;

  cfg->fog_update_interval = 30;
}

void UpdateFog(CONFIG *cfg, BIRD *bird, const int cols) {
  if (bird->is_in_albatross_taxi)
    return;

  cfg->fog_timer++;

  if (cfg->fog_timer >= cfg->fog_update_interval) {
    cfg->fog_timer = 0;

    const int safe_zone_width = cols / 2;
    const int left_limit = (cols - safe_zone_width) / 2;
    const int right_limit = (cols + safe_zone_width) / 2;

    if (cfg->fog_min_x < left_limit) {
      cfg->fog_min_x++;
      cfg->fog_currentsize++;
    }
    if (cfg->fog_max_x > right_limit) {
      cfg->fog_max_x--;
    }
  }
}

void DrawFog(WIN *playwin, CONFIG *cfg, const int rows, const int cols,
             char occupancyMap[rows][cols]) {
  wattron(playwin->window, COLOR_PAIR(FOG_COLOR_PAIR));

  for (int y = 1; y < rows - 1; y++) {
    for (int x = 1; x < cfg->fog_min_x; x++) {
      occupancyMap[y][x] = '#';
      mvwprintw(playwin->window, y, x, "%c", '#');
    }
  }

  for (int y = 1; y < rows - 1; y++) {
    for (int x = cfg->fog_max_x + 1; x < cols - 1; x++) {
      occupancyMap[y][x] = '#';
      mvwprintw(playwin->window, y, x, "%c", '#');
    }
  }

  wattroff(playwin->window, COLOR_PAIR(FOG_COLOR_PAIR));
}
