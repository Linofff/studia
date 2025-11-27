#include "./../headers/configmanaging.h"

void LoadConfig(CONFIG *c) {
  FILE *file = fopen("settings.txt", "r");
  if (!file) {
    printf("BLAD: Nie mozna otworzyc pliku");
    return;
  }

  char line[256];
  char current_section[50] = "";

  while (fgets(line, sizeof(line), file)) {
    for (int i = 0; line[i]; i++) {
      if (line[i] == '=' || line[i] == '{') {
        line[i] = ' ';
      }
    }

    if (strchr(line, '}')) {
      current_section[0] = '\0';
      continue;
    }

    char key[50];
    float value;

    if (sscanf(line, "%s %f", key, &value) == 2) {

      // Sekcja STARS
      if (strcmp(current_section, "stars") == 0) {
        if (strcmp(key, "max") == 0)
          c->star_max = (int)value;
        else if (strcmp(key, "spawn_chance") == 0)
          c->star_spawn_chance = (int)value;
        else if (strcmp(key, "speed") == 0)
          c->star_speed = value;
      }
      // Sekcja BIRD
      else if (strcmp(current_section, "bird") == 0) {
        if (strcmp(key, "health") == 0)
          c->start_health = (int)value;
      }
      // Sekcja HUNTER
      else if (strcmp(current_section, "hunter") == 0) {
        if (strcmp(key, "max_count") == 0)
          c->initial_hunter_max = (int)value;
        else if (strcmp(key, "spawn_chance") == 0)
          c->hunter_spawn_chance = (int)value;
        else if (strcmp(key, "damage") == 0)
          c->hunter_damage = (int)value;
        else if (strcmp(key, "bounces") == 0)
          c->hunter_bounces = (int)value;
        else if (strcmp(key, "speed") == 0)
          c->hunter_speed = (float)value;
      }
      // Sekcja GAME
      else if (strcmp(current_section, "game") == 0) {
        if (strcmp(key, "star_quota") == 0)
          c->star_quota = (int)value;
        else if (strcmp(key, "game_time") == 0)
          c->game_time_start = (int)value;
      }
    } else if (sscanf(line, "%s", key) == 1) {
      strcpy(current_section, key);
    }
  }

  fclose(file);
}

void UpdateConfig(CONFIG *cfg, int startTime) {
  cfg->hunter_bounces =
      cfg->initial_hunter_bounces + ((time(NULL) - startTime) / 8);
  cfg->hunter_max = cfg->initial_hunter_max + ((time(NULL) - startTime) / 8);
}

void UpdateTimeState(BIRD *bird, time_t *start_timestamp, CONFIG *cfg) {
  time_t current = time(NULL);

  if (bird->is_in_albatross_taxi) {
    *start_timestamp = current;
    return;
  }

  if (current > *start_timestamp) {
    cfg->game_time_elapsed++;
    cfg->game_time_left--;
    if (bird->albatross_in_cooldown > 0)
      bird->albatross_in_cooldown--;

    *start_timestamp = current;
  }
}
