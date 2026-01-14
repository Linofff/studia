#include "../headers/enemy.h"
#include "../headers/drawing.h"

EnemyType *InitEnemies() {
  EnemyType *newEnemies =
      (EnemyType *)calloc(NUMBER_OF_ENEMIES, sizeof(EnemyType));

  if (newEnemies == NULL) {
    printf("Failed to allocate memory for enemies.\n");
    return NULL;
  }

  for (int i = 0; i < NUMBER_OF_ENEMIES; i++) {
    newEnemies[i].alive = 0;
  }

  return newEnemies;
}

void SpawnEnemies(EnemyType *enemies) {
  if ((rand() % 100) >= 1)
    return;

  for (int i = 0; i < NUMBER_OF_ENEMIES; i++) {
    if (enemies[i].alive)
      continue;
    enemies[i].alive = 1;
    enemies[i].type = rand() % 2;
    enemies[i].health = 50;

    if (rand() % 2 == 0) {
      enemies[i].X = (rand() % 2) ? 0 : LEVEL_WIDTH;
      enemies[i].Y = rand() % LEVEL_HEIGHT + LEVEL_HEIGHT / 4;
    } else {
      enemies[i].X = rand() % LEVEL_WIDTH;
      enemies[i].Y = (rand() % 2) ? LEVEL_HEIGHT / 4 : LEVEL_HEIGHT;
    }

    if (enemies[i].type == 0) {
      enemies[i].speed = 100;
      enemies[i].surface_right = SDL_LoadBMP("textures/enemy_1.bmp");
    } else {
      enemies[i].speed = 150;
      enemies[i].ai_state = 0;
      enemies[i].ai_timer = 0;

      enemies[i].surface_right = SDL_LoadBMP("textures/enemy_2.bmp");
    }
    enemies[i].surface_left = FlipSurfaceHorizontal(enemies[i].surface_right);
    break;
  }
}

void MoveEnemy(EnemyType *enemies, PlayerType *player, double delta) {
  for (int i = 0; i < NUMBER_OF_ENEMIES; i++) {
    if (!enemies[i].alive)
      continue;

    double dx = player->X - enemies[i].X;
    double dy = player->Y - enemies[i].Y;
    double distance = sqrt(dx * dx + dy * dy);
    enemies[i].direction = (dx < 0) ? 1 : 0;

    if (enemies[i].type == 0) {
      if (distance > 0) {
        enemies[i].X += (dx / distance) * enemies[i].speed * delta;
        enemies[i].Y += (dy / distance) * enemies[i].speed * delta;
      }
    }

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
          enemies[i].ai_state = 1;   // Switch to Charge
          enemies[i].ai_timer = 1.0; // Charge for 1 second

          double chargeMag = sqrt(dx * dx + dy * dy);
          enemies[i].chargeDirX = dx / chargeMag;
          enemies[i].chargeDirY = dy / chargeMag;
        }

        if (enemies[i].ai_timer > 0)
          enemies[i].ai_timer -= delta;
      }

      else if (enemies[i].ai_state == 1) {
        double chargeSpeed = enemies[i].speed * 2;
        enemies[i].X += enemies[i].chargeDirX * chargeSpeed * delta;
        enemies[i].Y += enemies[i].chargeDirY * chargeSpeed * delta;

        enemies[i].ai_timer -= delta;
        if (enemies[i].ai_timer <= 0) {
          enemies[i].ai_state = 2;   // Switch to Cooldown
          enemies[i].ai_timer = 1.5; // Rest for 1.5 seconds
        }
      }

      else if (enemies[i].ai_state == 2) {
        enemies[i].ai_timer -= delta;
        if (enemies[i].ai_timer <= 0) {
          enemies[i].ai_state = 0; // Go back to Aligning
        }
      }
    }
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
