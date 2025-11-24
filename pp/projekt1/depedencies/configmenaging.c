#include "configmenaging.h"

void LoadConfig(CONFIG *c) {
  FILE *f = fopen("settings.txt", "r");
  char k[64];
  while (fscanf(f, "%s", k) > 0) {
    !strcmp(k, "STAR_MAX") && fscanf(f, "%d", &c->star_max);
    !strcmp(k, "STAR_QUOTA") && fscanf(f, "%d", &c->star_quota);
    !strcmp(k, "GAME_TIME") && fscanf(f, "%d", &c->game_time);
    !strcmp(k, "STAR_SPAWN_CHANCE") && fscanf(f, "%d", &c->star_spawn_chance);
    !strcmp(k, "STAR_SPEED") && fscanf(f, "%f", &c->star_speed);
    !strcmp(k, "HUNTER_MAX") && fscanf(f, "%d", &c->initial_hunter_max);
    !strcmp(k, "HUNTER_SPAWN_CHANCE") &&
        fscanf(f, "%d", &c->hunter_spawn_chance);
    !strcmp(k, "HUNTER_DAMAGE") && fscanf(f, "%d", &c->hunter_damage);
    !strcmp(k, "HUNTER_SPEED") && fscanf(f, "%f", &c->hunter_speed);
    !strcmp(k, "HUNTER_BOUNCES") && fscanf(f, "%d", &c->initial_hunter_bounces);
    !strcmp(k, "START_HEALTH") && fscanf(f, "%d", &c->start_health);
  }
  fclose(f);
}

void UpdateConfig(CONFIG *cfg, int startTime) {
  cfg->hunter_bounces =
      cfg->initial_hunter_bounces + ((time(NULL) - startTime) / 8);
  cfg->hunter_max = cfg->initial_hunter_max + ((time(NULL) - startTime) / 8);
}
