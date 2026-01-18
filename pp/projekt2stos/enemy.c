#include "enemy.h"
#include "main.h"
#include "utils.h"

void TintSequence(SDL_Surface **src, SDL_Surface **dst, int count, Uint8 r,
                  Uint8 g, Uint8 b) {
  for (int i = 0; i < count; i++) {
    if (src[i]) {
      dst[i] = SDL_DuplicateSurface(src[i]);
      SDL_SetSurfaceColorMod(dst[i], r, g, b);
    } else {
      dst[i] = NULL;
    }
  }
}

void GenerateRedAssets(EnemyAssets *assets, SDL_Surface **walk_R,
                       SDL_Surface **walk_L, SDL_Surface **hit_R,
                       SDL_Surface **hit_L, SDL_Surface **atk_R,
                       SDL_Surface **atk_L) {
  TintSequence(walk_R, assets->t0_walk_right, MAX_WALK_FRAMES, 255, 0, 0);
  TintSequence(walk_L, assets->t0_walk_left, MAX_WALK_FRAMES, 255, 0, 0);
  TintSequence(hit_R, assets->t0_hit_right, MAX_HIT_FRAMES, 255, 0, 0);
  TintSequence(hit_L, assets->t0_hit_left, MAX_HIT_FRAMES, 255, 0, 0);
  TintSequence(atk_R, assets->t0_attack_right, MAX_ATTACK_FRAMES, 255, 0, 0);
  TintSequence(atk_L, assets->t0_attack_left, MAX_ATTACK_FRAMES, 255, 0, 0);
}

void GenerateGreenAssets(EnemyAssets *assets, SDL_Surface **walk_R,
                         SDL_Surface **walk_L, SDL_Surface **hit_R,
                         SDL_Surface **hit_L, SDL_Surface **atk_R,
                         SDL_Surface **atk_L, SDL_Surface **dash_R,
                         SDL_Surface **dash_L) {
  TintSequence(walk_R, assets->t1_walk_right, MAX_WALK_FRAMES, 0, 255, 0);
  TintSequence(walk_L, assets->t1_walk_left, MAX_WALK_FRAMES, 0, 255, 0);
  TintSequence(hit_R, assets->t1_hit_right, MAX_HIT_FRAMES, 0, 255, 0);
  TintSequence(hit_L, assets->t1_hit_left, MAX_HIT_FRAMES, 0, 255, 0);
  TintSequence(atk_R, assets->t1_attack_right, MAX_ATTACK_FRAMES, 0, 255, 0);
  TintSequence(atk_L, assets->t1_attack_left, MAX_ATTACK_FRAMES, 0, 255, 0);
  TintSequence(dash_R, assets->t1_dash_right, MAX_DASH_FRAMES, 0, 255, 0);
  TintSequence(dash_L, assets->t1_dash_left, MAX_DASH_FRAMES, 0, 255, 0);
}

void FreeRawAssets(SDL_Surface **walk_R, SDL_Surface **walk_L,
                   SDL_Surface **hit_R, SDL_Surface **hit_L,
                   SDL_Surface **atk_R, SDL_Surface **atk_L,
                   SDL_Surface **dash_R, SDL_Surface **dash_L) {
  for (int i = 0; i < MAX_WALK_FRAMES; i++) {
    SDL_FreeSurface(walk_R[i]);
    SDL_FreeSurface(walk_L[i]);
  }
  for (int i = 0; i < MAX_HIT_FRAMES; i++) {
    SDL_FreeSurface(hit_R[i]);
    SDL_FreeSurface(hit_L[i]);
  }
  for (int i = 0; i < MAX_ATTACK_FRAMES; i++) {
    SDL_FreeSurface(atk_R[i]);
    SDL_FreeSurface(atk_L[i]);
  }
  for (int i = 0; i < MAX_DASH_FRAMES; i++) {
    SDL_FreeSurface(dash_R[i]);
    SDL_FreeSurface(dash_L[i]);
  }
}

void LoadEnemyAssets(EnemyAssets *assets) {
  SDL_Surface *raw_walk_R[MAX_WALK_FRAMES], *raw_walk_L[MAX_WALK_FRAMES];
  SDL_Surface *raw_hit_R[MAX_HIT_FRAMES], *raw_hit_L[MAX_HIT_FRAMES];
  SDL_Surface *raw_atk_R[MAX_ATTACK_FRAMES], *raw_atk_L[MAX_ATTACK_FRAMES];
  SDL_Surface *raw_dash_R[MAX_DASH_FRAMES], *raw_dash_L[MAX_DASH_FRAMES];

  LoadAnimationSequence("textures/fighter_run", MAX_WALK_FRAMES, raw_walk_R,
                        raw_walk_L);
  LoadAnimationSequence("textures/fighter_hit", MAX_HIT_FRAMES, raw_hit_R,
                        raw_hit_L);
  LoadAnimationSequence("textures/fighter_combo", MAX_ATTACK_FRAMES, raw_atk_R,
                        raw_atk_L);
  LoadAnimationSequence("textures/fighter_dash", MAX_DASH_FRAMES, raw_dash_R,
                        raw_dash_L);

  GenerateRedAssets(assets, raw_walk_R, raw_walk_L, raw_hit_R, raw_hit_L,
                    raw_atk_R, raw_atk_L);
  GenerateGreenAssets(assets, raw_walk_R, raw_walk_L, raw_hit_R, raw_hit_L,
                      raw_atk_R, raw_atk_L, raw_dash_R, raw_dash_L);
  FreeRawAssets(raw_walk_R, raw_walk_L, raw_hit_R, raw_hit_L, raw_atk_R,
                raw_atk_L, raw_dash_R, raw_dash_L);
}

EnemyType *InitEnemies() {
  EnemyType *newEnemies =
      (EnemyType *)calloc(NUMBER_OF_ENEMIES, sizeof(EnemyType));
  if (newEnemies == NULL) {
    printf("Failed to allocate memory for enemies.\n");
    return NULL;
  }
  return newEnemies;
}

void SetupType0(EnemyType *enemy, EnemyAssets *assets) {
  enemy->speed = ENEMY_T0_SPEED;
  enemy->damage = ENEMY_T0_DAMAGE;
  enemy->health = ENEMY_T0_HEALTH;

  memcpy(enemy->walk_frames_right, assets->t0_walk_right,
         sizeof(assets->t0_walk_right));
  memcpy(enemy->walk_frames_left, assets->t0_walk_left,
         sizeof(assets->t0_walk_left));
  memcpy(enemy->hit_frames_right, assets->t0_hit_right,
         sizeof(assets->t0_hit_right));
  memcpy(enemy->hit_frames_left, assets->t0_hit_left,
         sizeof(assets->t0_hit_left));
  memcpy(enemy->attack_frames_right, assets->t0_attack_right,
         sizeof(assets->t0_attack_right));
  memcpy(enemy->attack_frames_left, assets->t0_attack_left,
         sizeof(assets->t0_attack_left));
}

void SetupType1(EnemyType *enemy, EnemyAssets *assets) {
  enemy->speed = ENEMY_T1_SPEED;
  enemy->damage = ENEMY_T1_DAMAGE;
  enemy->ai_state = 0;
  enemy->health = ENEMY_T1_HEALTH;
  enemy->ai_timer = 0;

  memcpy(enemy->walk_frames_right, assets->t1_walk_right,
         sizeof(assets->t1_walk_right));
  memcpy(enemy->walk_frames_left, assets->t1_walk_left,
         sizeof(assets->t1_walk_left));
  memcpy(enemy->hit_frames_right, assets->t1_hit_right,
         sizeof(assets->t1_hit_right));
  memcpy(enemy->hit_frames_left, assets->t1_hit_left,
         sizeof(assets->t1_hit_left));
  memcpy(enemy->attack_frames_right, assets->t1_attack_right,
         sizeof(assets->t1_attack_right));
  memcpy(enemy->attack_frames_left, assets->t1_attack_left,
         sizeof(assets->t1_attack_left));
  memcpy(enemy->dash_frames_right, assets->t1_dash_right,
         sizeof(assets->t1_dash_right));
  memcpy(enemy->dash_frames_left, assets->t1_dash_left,
         sizeof(assets->t1_dash_left));
}

void SpawnSingleEnemy(EnemyType *enemy, EnemyAssets *assets) {
  enemy->alive = 1;
  enemy->type = rand() % 2;
  enemy->attack_timer = 0;
  enemy->stun_timer = 0;
  enemy->currentFrame = 0;
  enemy->animTimer = 0;

  if (rand() % 2 == 0) {
    enemy->X = (rand() % 2) ? 0 : LEVEL_WIDTH;
    enemy->Y = rand() % LEVEL_HEIGHT + LEVEL_HEIGHT / 4;
  } else {
    enemy->X = rand() % LEVEL_WIDTH;
    enemy->Y = (rand() % 2) ? LEVEL_HEIGHT / 4 : LEVEL_HEIGHT;
  }

  if (enemy->type == 0)
    SetupType0(enemy, assets);
  else
    SetupType1(enemy, assets);
}

void SpawnEnemies(EnemyType *enemies, EnemyAssets *assets) {
  if ((rand() % 100) >= 1)
    return;

  for (int i = 0; i < NUMBER_OF_ENEMIES; i++) {
    if (enemies[i].alive)
      continue;
    SpawnSingleEnemy(&enemies[i], assets);
    break;
  }
}

void UpdateEnemyTimers(EnemyType *enemy, double delta) {
  if (enemy->stun_timer > 0)
    enemy->stun_timer -= delta;
  if (enemy->attack_timer > 0)
    enemy->attack_timer -= delta;
}

void UpdateEnemyAnimation(EnemyType *enemy, double delta) {
  int maxFrames = MAX_WALK_FRAMES;

  if (enemy->stun_timer > 0) {
    maxFrames = MAX_HIT_FRAMES;
  } else if (enemy->type == 1 && enemy->ai_state == 1) {
    maxFrames = MAX_DASH_FRAMES;
  } else if (enemy->attack_timer > (ATTACK_COOLDOWN - 0.5)) {
    maxFrames = MAX_ATTACK_FRAMES;
  }

  enemy->animTimer += delta;
  if (enemy->animTimer >= 0.15) {
    enemy->animTimer -= 0.15;
    enemy->currentFrame++;
    if (enemy->currentFrame >= maxFrames)
      enemy->currentFrame = 0;
  }

  if (enemy->currentFrame >= maxFrames)
    enemy->currentFrame = 0;
}

void HandleEnemyAttack(EnemyType *enemy, PlayerType *player, double distance) {
  if (distance < ATTACK_RANGE && enemy->attack_timer <= 0) {
    int current_damage = enemy->damage;
    if (enemy->type == 1 && enemy->ai_state == 1)
      current_damage *= 2;

    player->health -= current_damage;
    player->wasHitTimer = 1;
    enemy->attack_timer = ATTACK_COOLDOWN;
    enemy->currentFrame = 0;
    enemy->animTimer = 0;
  }
}

void MoveChaser(EnemyType *enemy, double dx, double dy, double dist,
                double delta) {
  if (dist > 1) {
    enemy->X += (dx / dist) * enemy->speed * delta;
    enemy->Y += (dy / dist) * enemy->speed * delta;
  }
}

void MoveCharger(EnemyType *enemy, PlayerType *player, double dx, double dy,
                 double delta) {
  if (enemy->ai_state == 0) {
    double targetX = (dx > 0) ? player->X - 250 : player->X + 250;
    double diffX = targetX - enemy->X;
    double moveSpeed = enemy->speed * delta;

    if (fabs(diffX) > 5)
      enemy->X += (diffX > 0 ? 1 : -1) * moveSpeed;
    if (fabs(dy) > 5)
      enemy->Y += (dy > 0 ? 1 : -1) * moveSpeed;

    if (fabs(dy) < 20 && enemy->ai_timer <= 0) {
      enemy->ai_state = 1;
      enemy->ai_timer = 1.0;
      enemy->currentFrame = 0;
      enemy->animTimer = 0;

      double chargeMag = sqrt(dx * dx + dy * dy);
      enemy->chargeDirX = dx / chargeMag;
      enemy->chargeDirY = dy / chargeMag;
    }
    if (enemy->ai_timer > 0)
      enemy->ai_timer -= delta;

  }

  else if (enemy->ai_state == 1) {
    double chargeSpeed = enemy->speed * ENEMY_T1_SPEED_BOOST;
    enemy->X += enemy->chargeDirX * chargeSpeed * delta;
    enemy->Y += enemy->chargeDirY * chargeSpeed * delta;
    enemy->ai_timer -= delta;
    if (enemy->ai_timer <= 0) {
      enemy->ai_state = 2;
      enemy->ai_timer = 1.5;
    }

  }

  else if (enemy->ai_state == 2) {
    enemy->ai_timer -= delta;
    if (enemy->ai_timer <= 0)
      enemy->ai_state = 0;
  }
}

void EnforceBounds(EnemyType *enemy) {

  int heightHalf = enemy->walk_frames_left[0]->h / 2;
  int widthHalf = enemy->walk_frames_left[0]->w / 2;
  if (enemy->X < widthHalf)
    enemy->X = widthHalf;
  if (enemy->Y < LEVEL_HEIGHT / 4 - heightHalf)
    enemy->Y = LEVEL_HEIGHT / 4 - heightHalf;
  if (enemy->X > LEVEL_WIDTH - widthHalf)
    enemy->X = LEVEL_WIDTH - widthHalf;
  if (enemy->Y > LEVEL_HEIGHT - heightHalf)
    enemy->Y = LEVEL_HEIGHT - heightHalf;
}

void MoveEnemy(EnemyType *enemies, PlayerType *player, double delta) {
  for (int i = 0; i < NUMBER_OF_ENEMIES; i++) {
    if (!enemies[i].alive)
      continue;

    UpdateEnemyTimers(&enemies[i], delta);
    UpdateEnemyAnimation(&enemies[i], delta);

    if (enemies[i].stun_timer > 0)
      continue;

    double dx = player->X - enemies[i].X;
    double dy = player->Y - enemies[i].Y;
    double distance = sqrt(dx * dx + dy * dy);
    enemies[i].direction = (dx < 0) ? 1 : 0;

    HandleEnemyAttack(&enemies[i], player, distance);

    if (enemies[i].type == 0) {
      MoveChaser(&enemies[i], dx, dy, distance, delta);
    } else {
      MoveCharger(&enemies[i], player, dx, dy, delta);
    }

    EnforceBounds(&enemies[i]);
  }
}
