#include "./../headers/configmanaging.h"

void InitMap(char occupancyMap[ROWS][COLS]) {
  for (int y = 0; y < ROWS; y++) {
    for (int x = 0; x < COLS; x++) {
      occupancyMap[y][x] = ' ';
    }
  }
}

void SanitizeLine(char *line) {
  for (int i = 0; line[i]; i++) {
    if (line[i] == '=' || line[i] == '{') {
      line[i] = ' ';
    }
  }
}

void AssignConfigToInput(CONFIG *c, const char *section, const char *key,
                         const char *val_str, int *active_template_id) {

  float val_float = (float)atof(val_str);

  // STARS SECTION
  if (strcmp(section, "stars") == 0) {
    StarsConfigLoad(c, key, val_float);
  }
  // PLAYER SECTION
  else if (strcmp(section, "player") == 0) {
    PlayerConfigLoad(c, key, val_str);
  }
  // BIRD SECTION
  else if (strcmp(section, "bird") == 0) {
    BirdConfigLoad(c, key, val_float);
  }
  // HUNTER SECTION
  else if (strcmp(section, "hunter") == 0) {
    HunterConfigLoad(c, key, val_float, active_template_id);
  }
  // GAME SECTION
  else if (strcmp(section, "game") == 0) {
    GameConfigLoad(c, key, val_float);
  }
}

void StarsConfigLoad(CONFIG *c, const char *key, float value) {
  if (strcmp(key, "max") == 0)
    c->levels[0].star_max = (int)value;
  else if (strcmp(key, "spawn_chance") == 0)
    c->levels[0].star_spawn_chance = (int)value;
}

void HunterConfigLoad(CONFIG *c, const char *key, float value,
                      int *active_template_id) {
  if (*active_template_id != -1) {
    if (strcmp(key, "width") == 0) {
      c->hunter_templates[*active_template_id].width = (int)value;
    } else if (strcmp(key, "height") == 0) {
      c->hunter_templates[*active_template_id].height = (int)value;
    }
    return;
  }

  if (strcmp(key, "max_count") == 0)
    c->levels[0].initial_hunter_max = (int)value;
  else if (strcmp(key, "spawn_chance") == 0)
    c->levels[0].hunter_spawn_chance = (int)value;
  else if (strcmp(key, "damage") == 0)
    c->levels[0].hunter_damage = (int)value;
  else if (strcmp(key, "bounces") == 0)
    c->levels[0].initial_hunter_bounces = (int)value;
  else if (strcmp(key, "speed") == 0)
    c->levels[0].hunter_speed = value;
  else if (strcmp(key, "template") == 0)
    *active_template_id = ((int)(value)-1);
}

void GameConfigLoad(CONFIG *c, const char *key, float value) {
  if (strcmp(key, "level") == 0)
    c->levels[0].number = (int)value;
  else if (strcmp(key, "star_quota") == 0)
    c->levels[0].star_quota = (int)value;
  else if (strcmp(key, "game_time") == 0)
    c->game_time_start = (int)value;
  else if (strcmp(key, "seed") == 0)
    c->seed = (time_t)value;
}

void BirdConfigLoad(CONFIG *c, const char *key, float value) {
  if (strcmp(key, "health") == 0)
    c->start_health = (int)value;
}

void PlayerConfigLoad(CONFIG *c, const char *key, const char *val_str) {
  if (strcmp(key, "name") == 0) {
    strcpy(c->player_name, val_str);
  }
}

void LoadConfig(CONFIG *c) {
  FILE *file = fopen(CONFIG_FILE_NAME, "r");
  if (!file) {
    return;
  }

  int active_template_id = -1;

  char line[256];
  char current_section[50] = "";

  while (fgets(line, sizeof(line), file)) {
    SanitizeLine(line);

    if (strchr(line, '}')) {
      if (active_template_id != -1) {
        active_template_id = -1;
      } else {
        current_section[0] = '\0';
      }
      continue;
    }

    char key[50];
    char val_str[100];

    if (sscanf(line, "%s %s", key, val_str) == 2) {
      AssignConfigToInput(c, current_section, key, val_str,
                          &active_template_id);
    } else if (sscanf(line, "%s", key) == 1 && active_template_id == -1) {
      strcpy(current_section, key);
    }
  }

  fclose(file);
}

void UpdateConfig(CONFIG *cfg) {
  int bounces = cfg->levels[0].initial_hunter_max +
                cfg->game_time_elapsed / TIME_ENTITY_MULTI;
  int maxcount = cfg->levels[0].initial_hunter_max +
                 cfg->game_time_elapsed / TIME_ENTITY_MULTI;
  if (bounces < 9)
    cfg->levels[0].hunter_bounces = bounces;
  cfg->levels[0].hunter_max = maxcount;
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
    if (!bird->is_in_albatross_taxi && bird->albatross_in_cooldown > 0)
      bird->albatross_in_cooldown--;

    *start_timestamp = current;
  }
}
