#include "fog.h"

void InitFog(CONFIG *cfg, const int cols) {
  // initializing config values
  cfg->safe_zone_width = cols / 2;

  cfg->fog_left_limit = (cols - cfg->safe_zone_width);
  cfg->fog_left_limit = (cols - cfg->safe_zone_width) / 2;
  cfg->fog_right_limit = (cols + cfg->safe_zone_width) / 2;

  cfg->fog_min_x = 1;
  cfg->fog_max_x = cols - 2;
  cfg->fog_timer = 0;

  cfg->fog_update_interval = 30;
}

void UpdateFog(CONFIG *cfg, BIRD *bird) {
  // incresing size of the fog based on the time elapsed, fog_update_interval
  // and if the player is in the taxi
  if (bird->is_in_albatross_taxi)
    return;

  cfg->fog_timer++;

  if (cfg->fog_timer >= cfg->fog_update_interval) {
    cfg->fog_timer = 0;

    if (cfg->fog_min_x < cfg->fog_left_limit) {
      cfg->fog_min_x++;
      cfg->fog_currentsize++;
    }
    if (cfg->fog_max_x > cfg->fog_right_limit) {
      cfg->fog_max_x--;
    }
  }
}

void DrawFog(WIN *playwin, CONFIG *cfg, const int rows, const int cols,
             char occupancyMap[rows][cols]) {
  wattron(playwin->window, COLOR_PAIR(FOG_COLOR_PAIR));

  // redrawing and adding new lines based on theupdad fog
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
