#include <stdio.h>
#include <string.h>

typedef struct {
  int star_max;
  int star_quota;
  int game_time_start;
  int star_spawn_chance;
  float star_speed;
  int initial_hunter_max;
  int hunter_spawn_chance;
  int hunter_damage;
  int hunter_bounces;
  int start_health;
} GameConfig;

void load_config(const char *filename, GameConfig *c) {
  FILE *file = fopen(filename, "r");
  if (!file) {
    printf("BLAD: Nie mozna otworzyc pliku %s\n", filename);
    return;
  }

  char line[256];
  char current_section[50] = "";

  while (fgets(line, sizeof(line), file)) {
    // 1. Oczyszczanie linii: zamien znaki specjalne na spacje
    // To sprawia, że "max = 100" i "max 100" czyta się tak samo łatwo
    for (int i = 0; line[i]; i++) {
      if (line[i] == '=' || line[i] == '{') {
        line[i] = ' ';
      }
    }

    // 2. Sprawdzanie końca sekcji
    if (strchr(line, '}')) {
      current_section[0] = '\0'; // Wyczysc sekcje
      continue;
    }

    char key[50];
    float value;

    // 3. Próba wczytania pary: KLUCZ WARTOSC
    // Dzieki zamianie '=' na spacje, ten jeden sscanf obsluguje oba formaty
    if (sscanf(line, "%s %f", key, &value) == 2) {

      // --- LOGIKA PRZYPISYWANIA ---

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
        // Obsluga literowki "bouces" oraz poprawnego "bounces"
        else if (strcmp(key, "bouces") == 0 || strcmp(key, "bounces") == 0)
          c->hunter_bounces = (int)value;
      }
      // Sekcja GAME
      else if (strcmp(current_section, "game") == 0) {
        if (strcmp(key, "star_quota") == 0)
          c->star_quota = (int)value;
        else if (strcmp(key, "game_time") == 0)
          c->game_time_start = (int)value;
      }
    }
    // 4. Jeśli nie wczytano pary, sprawdzamy czy to nagłówek sekcji
    else if (sscanf(line, "%s", key) == 1) {
      // Jeśli linia to np. "stars" (bo klamre { zamienilismy na spacje wyzej)
      strcpy(current_section, key);
    }
  }

  fclose(file);
}

// --- TEST ---
int main() {
  GameConfig config = {0}; // Zerowanie struktury na start

  // Tworze plik testowy, zeby upewnic sie ze dane sa
  // FILE *f = fopen("config.txt", "w");
  // fprintf(f, "stars {\n  max = 100\n  spawn_chance = 25\n}\n");
  // fprintf(f, "bird {\n  health = 3\n}\n");
  // fprintf(f, "hunter {\n  max_count = 2\n  spawn_chance = 30\n  damage = 1\n
  // "
  //            "speed = 0.9\n  bouces = 2\n}\n");
  // fprintf(f, "game {\n  star_quota 10\n  game_time 50\n}\n");
  // fclose(f);

  load_config("config.txt", &config);

  printf("--- WYNIKI ---\n");
  printf("Star Max: %d\n", config.star_max);
  printf("Star Spawn Chance: %d\n", config.star_spawn_chance);
  printf("Bird Health: %d\n", config.start_health);
  printf("Hunter Max: %d\n", config.initial_hunter_max);
  printf("Hunter Damage: %d\n", config.hunter_damage);
  printf("Hunter Bounces: %d\n", config.hunter_bounces);
  printf("Game Quota: %d\n", config.star_quota);
  printf("Game Time: %d\n", config.game_time_start);

  return 0;
}
