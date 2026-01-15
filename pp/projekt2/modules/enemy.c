#include "../headers/enemy.h"
#include "../headers/drawing.h"

#define ATTACK_RANGE 35.0
#define ATTACK_COOLDOWN 1.0

EnemyType *InitEnemies() {
  EnemyType *newEnemies =
      (EnemyType *)calloc(NUMBER_OF_ENEMIES, sizeof(EnemyType));

  if (newEnemies == NULL) {
    printf("Failed to allocate memory for enemies.\n");
    return NULL;
  }

  return newEnemies;
}

void SpawnEnemies(EnemyType *enemies) {
  if ((rand() % 100) >= 1)
    return;

  for (int i = 0; i < NUMBER_OF_ENEMIES; i++) {
    if (enemies[i].alive)
      continue;

    // Basic Init
    enemies[i].alive = 1;
    enemies[i].type = rand() % 2;
    enemies[i].health = 1;
    enemies[i].attack_timer = 0;

    // Animation Init
    enemies[i].currentFrame = 0;
    enemies[i].animTimer = 0;

    // Position Logic
    if (rand() % 2 == 0) {
      enemies[i].X = (rand() % 2) ? 0 : LEVEL_WIDTH;
      enemies[i].Y = rand() % LEVEL_HEIGHT + LEVEL_HEIGHT / 4;
    } else {
      enemies[i].X = rand() % LEVEL_WIDTH;
      enemies[i].Y = (rand() % 2) ? LEVEL_HEIGHT / 4 : LEVEL_HEIGHT;
    }

    // TYPE SPECIFIC SETUP
    char filename[64];

    if (enemies[i].type == 0) {
      enemies[i].speed = 100;
      enemies[i].damage = 10;

      for (int j = 0; j < MAX_WALK_FRAMES; j++) {
        sprintf(filename, "textures/fighter_run_%d.bmp", j + 1);
        enemies[i].walk_frames_right[j] = SDL_LoadBMP(filename);
        if (!enemies[i].walk_frames_right[j])
          printf("Failed to load %s\n", filename);
        enemies[i].walk_frames_left[j] =
            FlipSurfaceHorizontal(enemies[i].walk_frames_right[j]);
      }
    } else {
      enemies[i].speed = 150;
      enemies[i].damage = 20;
      enemies[i].ai_state = 0;
      enemies[i].ai_timer = 0;

      for (int j = 0; j < MAX_WALK_FRAMES; j++) {
        sprintf(filename, "textures/fighter_run_%d.bmp", j + 1);
        enemies[i].walk_frames_right[j] = SDL_LoadBMP(filename);
        if (!enemies[i].walk_frames_right[j])
          printf("Failed to load %s\n", filename);
        enemies[i].walk_frames_left[j] =
            FlipSurfaceHorizontal(enemies[i].walk_frames_right[j]);
      }
    }

    break; // Spawn one at a time
  }
}

void MoveEnemy(EnemyType *enemies, PlayerType *player, double delta) {
  for (int i = 0; i < NUMBER_OF_ENEMIES; i++) {

    if (enemies[i].stun_timer > 0)
      enemies[i].stun_timer -= delta;

    if (enemies[i].attack_timer > 0)
      enemies[i].attack_timer -= delta;

    if (!enemies[i].alive || enemies[i].stun_timer > 0)
      continue;

    // --- ANIMATION LOGIC ---
    // Only animate if not stunned
    enemies[i].animTimer += delta;
    if (enemies[i].animTimer >= 0.15) { // 0.15s per frame
      enemies[i].animTimer -= 0.15;
      enemies[i].currentFrame++;
      if (enemies[i].currentFrame >= MAX_WALK_FRAMES) {
        enemies[i].currentFrame = 0;
      }
    }
    // -----------------------

    double dx = player->X - enemies[i].X;
    double dy = player->Y - enemies[i].Y;
    double distance = sqrt(dx * dx + dy * dy);
    enemies[i].direction = (dx < 0) ? 1 : 0;

    // Attack Logic
    if (distance < ATTACK_RANGE && enemies[i].attack_timer <= 0) {
      int current_damage = enemies[i].damage;
      if (enemies[i].type == 1 && enemies[i].ai_state == 1) {
        current_damage *= 2;
      }
      player->health -= current_damage;
      enemies[i].attack_timer = ATTACK_COOLDOWN;
      // printf("Player Hit! HP: %d\n", player->health);
    }

    // Movement Logic (Type 0)
    if (enemies[i].type == 0) {
      if (distance > 0) {
        enemies[i].X += (dx / distance) * enemies[i].speed * delta;
        enemies[i].Y += (dy / distance) * enemies[i].speed * delta;
      }
    }

    // Movement Logic (Type 1)
    else if (enemies[i].type == 1) {
      if (enemies[i].ai_state == 0) {
        double targetX = (dx > 0) ? player->X - 250 : player->X + 250;
        double diffX = targetX - enemies[i].X;
        double moveSpeed = enemies[i].speed * delta;

        if (fabs(diffX) > 5)
          enemies[i].X += (diffX > 0 ? 1 : -1) * moveSpeed;
        if (fabs(dy) > 5)
          enemies[i].Y += (dy > 0 ? 1 : -1) * moveSpeed;

        if (fabs(dy) < 20 && enemies[i].ai_timer <= 0) {
          enemies[i].ai_state = 1;
          enemies[i].ai_timer = 1.0;
          double chargeMag = sqrt(dx * dx + dy * dy);
          enemies[i].chargeDirX = dx / chargeMag;
          enemies[i].chargeDirY = dy / chargeMag;
        }
        if (enemies[i].ai_timer > 0)
          enemies[i].ai_timer -= delta;
      } else if (enemies[i].ai_state == 1) {
        double chargeSpeed = enemies[i].speed * 2;
        enemies[i].X += enemies[i].chargeDirX * chargeSpeed * delta;
        enemies[i].Y += enemies[i].chargeDirY * chargeSpeed * delta;
        enemies[i].ai_timer -= delta;
        if (enemies[i].ai_timer <= 0) {
          enemies[i].ai_state = 2;
          enemies[i].ai_timer = 1.5;
        }
      } else if (enemies[i].ai_state == 2) {
        enemies[i].ai_timer -= delta;
        if (enemies[i].ai_timer <= 0)
          enemies[i].ai_state = 0;
      }
    }

    // Bounds Checks
    if (enemies[i].X < 32)
      enemies[i].X = 32;
    if (enemies[i].Y < LEVEL_HEIGHT / 4 - 20)
      enemies[i].Y = LEVEL_HEIGHT / 4 - 20;
    if (enemies[i].X > LEVEL_WIDTH - 20)
      enemies[i].X = LEVEL_WIDTH - 20;
    if (enemies[i].Y > LEVEL_HEIGHT - 20)
      enemies[i].Y = LEVEL_HEIGHT - 20;
  }
}
