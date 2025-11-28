#include "./../headers/configmanaging.h"

void SanitizeLine(char *line) {
  for (int i = 0; line[i]; i++) {
    if (line[i] == '=' || line[i] == '{') {
      line[i] = ' ';
    }
  }
}

void AssignConfigToInput(CONFIG *c, const char *section, const char *key,
                         float value) {
  // STARS SECTION
  if (strcmp(section, "stars") == 0) {
    StarsConfigLoad(c, key, value);
  }
  // BIRD SECTION
  else if (strcmp(section, "bird") == 0) {
    BirdConfigLoad(c, key, value);
  }
  // HUNTER SECTION
  else if (strcmp(section, "hunter") == 0) {
    HunterConfigLoad(c, key, value);
  }
  // GAME SECTION
  else if (strcmp(section, "game") == 0) {
    GameConfigLoad(c, key, value);
  }
  if (strcmp(section, "template") == 0) {
    exit(0);
  }
}

void StarsConfigLoad(CONFIG *c, const char *key, float value) {
  if (strcmp(key, "max") == 0)
    c->star_max = (int)value;
  else if (strcmp(key, "spawn_chance") == 0)
    c->star_spawn_chance = (int)value;
}

void HunterConfigLoad(CONFIG *c, const char *key, float value) {
  if (strcmp(key, "max_count") == 0)
    c->initial_hunter_max = (int)value;
  else if (strcmp(key, "spawn_chance") == 0)
    c->hunter_spawn_chance = (int)value;
  else if (strcmp(key, "damage") == 0)
    c->hunter_damage = (int)value;
  else if (strcmp(key, "bounces") == 0)
    c->initial_hunter_bounces = (int)value;
  else if (strcmp(key, "speed") == 0)
    c->hunter_speed = value;
}

void GameConfigLoad(CONFIG *c, const char *key, float value) {
  if (strcmp(key, "star_quota") == 0)
    c->star_quota = (int)value;
  else if (strcmp(key, "game_time") == 0)
    c->game_time_start = (int)value;
  else if (strcmp(key, "seed") == 0)
    c->seed = (time_t)value;
}

void BirdConfigLoad(CONFIG *c, const char *key, float value) {
  if (strcmp(key, "health") == 0)
    c->start_health = (int)value;
}

void LoadConfig(CONFIG *c) {
  FILE *file = fopen("config.txt", "r");
  if (!file) {
    printf("BLAD: Nie mozna otworzyc pliku\n");
    return;
  }

  char line[256];
  char current_section[50] = "";

  while (fgets(line, sizeof(line), file)) {
    // Krok 1: Wyczyść linię
    SanitizeLine(line);

    // Krok 2: Obsługa końca sekcji
    if (strchr(line, '}')) {
      current_section[0] = '\0';
      continue;
    }

    char key[50];
    float value;

    // Krok 3: Próba odczytania pary KLUCZ WARTOŚĆ
    if (sscanf(line, "%s %f", key, &value) == 2) {
      AssignConfigToInput(c, current_section, key, value);
    }
    // Krok 4: Próba odczytania nowej SEKCJI
    else if (sscanf(line, "%s", key) == 1) {
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
    if (!bird->is_in_albatross_taxi && bird->albatross_in_cooldown > 0)
      bird->albatross_in_cooldown--;

    *start_timestamp = current;
  }
}
