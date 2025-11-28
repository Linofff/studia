#include "./../headers/configmanaging.h"

// Forward declaration if not in header
void SanitizeLine(char *line) {
  for (int i = 0; line[i]; i++) {
    if (line[i] == '=' || line[i] == '{') {
      line[i] = ' ';
    }
  }
}

void AssignConfigToInput(CONFIG *c, const char *section, const char *key,
                         float value, int *active_template_id) {
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
    HunterConfigLoad(c, key, value, active_template_id);
  }
  // GAME SECTION
  else if (strcmp(section, "game") == 0) {
    GameConfigLoad(c, key, value);
  }
}

void StarsConfigLoad(CONFIG *c, const char *key, float value) {
  if (strcmp(key, "max") == 0)
    c->star_max = (int)value;
  else if (strcmp(key, "spawn_chance") == 0)
    c->star_spawn_chance = (int)value;
}

void HunterConfigLoad(CONFIG *c, const char *key, float value,
                      int *active_template_id) {
  // 1. Handle Template Specific Properties (width/height)
  if (*active_template_id != -1) {
    if (strcmp(key, "width") == 0) {
      c->hunter_templates[*active_template_id].width = (int)value;
    } else if (strcmp(key, "height") == 0) {
      c->hunter_templates[*active_template_id].height = (int)value;
    }
    // Return here so we don't accidentally overwrite global hunter stats
    return;
  }

  // 2. Handle Global Hunter Properties
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
  else if (strcmp(key, "template") == 0)
    LoadTemplates(c, key, value, active_template_id);
}

void LoadTemplates(CONFIG *c, const char *key, float value,
                   int *active_template_id) {
  switch ((int)(value)) {
  case 1:
    *active_template_id = 0;
    break;
  case 2:
    *active_template_id = 1;
    break;
  case 3:
    *active_template_id = 2;
    break;
  case 4:
    *active_template_id = 3;
    break;
  case 5:
    *active_template_id = 4;
    break;
  default:
    break;
  }
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

  // Initialize state: -1 means we are NOT inside a template block
  int active_template_id = -1;

  char line[256];
  char current_section[50] = "";

  while (fgets(line, sizeof(line), file)) {
    // Krok 1: Wyczyść linię
    SanitizeLine(line);

    // Krok 2: Obsługa końca sekcji
    if (strchr(line, '}')) {
      // LOGIC CHANGE: If we are inside a template, '}' closes the template,
      // not the whole 'hunter' section.
      if (active_template_id != -1) {
        active_template_id = -1; // Reset template state
      } else {
        current_section[0] = '\0'; // Reset section state (stars, hunter, etc)
      }
      continue;
    }

    char key[50];
    float value;

    // Krok 3: Próba odczytania pary KLUCZ WARTOŚĆ
    if (sscanf(line, "%s %f", key, &value) == 2) {
      AssignConfigToInput(c, current_section, key, value, &active_template_id);
    }
    // Krok 4: Próba odczytania nowej SEKCJI
    // We only change the main section if we are NOT inside a template
    else if (sscanf(line, "%s", key) == 1 && active_template_id == -1) {
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
