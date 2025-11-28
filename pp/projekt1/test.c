#include "./../headers/configmanaging.h"

// Add this struct definition
typedef struct {
  int width;
  int height;
} HunterTemplate;

// Update your main CONFIG struct
typedef struct {
  // ... your existing fields ...

  // NEW: Array to store templates (assuming max 5 like the file)
  HunterTemplate hunter_templates[5];

  // NEW: Helper for parsing state (-1 means not in a template)
  int parsing_template_idx;
} CONFIG;

// No changes needed to SanitizeLine
void SanitizeLine(char *line) {
  for (int i = 0; line[i]; i++) {
    if (line[i] == '=' || line[i] == '{') {
      line[i] = ' ';
    }
  }
}

// ... [AssignConfigToInput and StarsConfigLoad stay the same] ...
// ... [BirdConfigLoad and GameConfigLoad stay the same] ...

// --- UPDATED FUNCTION ---
void HunterConfigLoad(CONFIG *c, const char *key, float value) {
  // 1. Handle switching into a template context
  // Line in config: "template 1 {" -> Key="template", Value=1.0
  if (strcmp(key, "template") == 0) {
    int idx = (int)value - 1; // Convert 1-based to 0-based
    if (idx >= 0 && idx < 5) {
      c->parsing_template_idx = idx;
    }
    return;
  }

  // 2. Handle Template Properties (width/height)
  // We check if we are currently inside a template block
  if (c->parsing_template_idx != -1) {
    int idx = c->parsing_template_idx;
    if (strcmp(key, "width") == 0) {
      c->hunter_templates[idx].width = (int)value;
    } else if (strcmp(key, "height") == 0) {
      c->hunter_templates[idx].height = (int)value;
    }
    return; // Don't check standard hunter keys if we matched a template key
  }

  // 3. Handle Standard Hunter Properties
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

// --- UPDATED FUNCTION ---
void LoadConfig(CONFIG *c) {
  FILE *file = fopen("config.txt", "r");
  if (!file) {
    printf("BLAD: Nie mozna otworzyc pliku\n");
    return;
  }

  // Initialize the parsing state (-1 means we are not inside a template)
  c->parsing_template_idx = -1;

  char line[256];
  char current_section[50] = "";

  while (fgets(line, sizeof(line), file)) {
    // Krok 1: Wyczyść linię
    SanitizeLine(line);

    // Krok 2: Obsługa końca sekcji (UPDATED FOR NESTING)
    if (strchr(line, '}')) {
      // If we are currently inside a template, '}' closes the template only
      if (c->parsing_template_idx != -1) {
        c->parsing_template_idx = -1;
      }
      // Otherwise, '}' closes the main section (e.g., hunter)
      else {
        current_section[0] = '\0';
      }
      continue;
    }

    char key[50];
    float value;

    // Krok 3: Próba odczytania pary KLUCZ WARTOŚĆ
    if (sscanf(line, "%s %f", key, &value) == 2) {
      AssignConfigToInput(c, current_section, key, value);
    }
    // Krok 4: Próba odczytania nowej SEKCJI
    // We only switch main sections if we are NOT inside a template
    else if (sscanf(line, "%s", key) == 1 && c->parsing_template_idx == -1) {
      strcpy(current_section, key);
    }
  }

  fclose(file);
}
