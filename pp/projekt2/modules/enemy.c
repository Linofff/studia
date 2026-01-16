#include "../headers/enemy.h"
#include "../headers/drawing.h"
#include "../headers/utils.h"

void TintSequence(SDL_Surface **src, SDL_Surface **dst, int count, Uint8 r,
                  Uint8 g, Uint8 b) {
  for (int i = 0; i < count; i++) {
    if (src[i]) {
      dst[i] = SDL_DuplicateSurface(src[i]);   // 1. Copy
      SDL_SetSurfaceColorMod(dst[i], r, g, b); // 2. Tint
    } else {
      dst[i] = NULL;
    }
  }
}

void LoadEnemyAssets(EnemyAssets *assets) {
  // A. Temporary storage for raw images (read from disk)
  SDL_Surface *raw_walk_R[MAX_WALK_FRAMES], *raw_walk_L[MAX_WALK_FRAMES];
  SDL_Surface *raw_hit_R[MAX_HIT_FRAMES], *raw_hit_L[MAX_HIT_FRAMES];
  SDL_Surface *raw_atk_R[MAX_ATTACK_FRAMES], *raw_atk_L[MAX_ATTACK_FRAMES];

  // B. Load Raw Images from Disk (using your helper)
  LoadAnimationSequence("textures/fighter_run", MAX_WALK_FRAMES, raw_walk_R,
                        raw_walk_L);
  LoadAnimationSequence("textures/fighter_hit", MAX_HIT_FRAMES, raw_hit_R,
                        raw_hit_L);
  LoadAnimationSequence("textures/fighter_combo", MAX_ATTACK_FRAMES, raw_atk_R,
                        raw_atk_L);

  // C. Create TYPE 0 (RED)
  TintSequence(raw_walk_R, assets->t0_walk_right, MAX_WALK_FRAMES, 255, 0, 0);
  TintSequence(raw_walk_L, assets->t0_walk_left, MAX_WALK_FRAMES, 255, 0, 0);
  TintSequence(raw_hit_R, assets->t0_hit_right, MAX_HIT_FRAMES, 255, 0, 0);
  TintSequence(raw_hit_L, assets->t0_hit_left, MAX_HIT_FRAMES, 255, 0, 0);
  TintSequence(raw_atk_R, assets->t0_attack_right, MAX_ATTACK_FRAMES, 255, 0,
               0);
  TintSequence(raw_atk_L, assets->t0_attack_left, MAX_ATTACK_FRAMES, 255, 0, 0);

  // D. Create TYPE 1 (GREEN)
  TintSequence(raw_walk_R, assets->t1_walk_right, MAX_WALK_FRAMES, 0, 255, 0);
  TintSequence(raw_walk_L, assets->t1_walk_left, MAX_WALK_FRAMES, 0, 255, 0);
  TintSequence(raw_hit_R, assets->t1_hit_right, MAX_HIT_FRAMES, 0, 255, 0);
  TintSequence(raw_hit_L, assets->t1_hit_left, MAX_HIT_FRAMES, 0, 255, 0);
  TintSequence(raw_atk_R, assets->t1_attack_right, MAX_ATTACK_FRAMES, 0, 255,
               0);
  TintSequence(raw_atk_L, assets->t1_attack_left, MAX_ATTACK_FRAMES, 0, 255, 0);

  // E. Cleanup Raw Surfaces (We only needed them to make copies)
  for (int i = 0; i < MAX_WALK_FRAMES; i++) {
    SDL_FreeSurface(raw_walk_R[i]);
    SDL_FreeSurface(raw_walk_L[i]);
  }
  for (int i = 0; i < MAX_HIT_FRAMES; i++) {
    SDL_FreeSurface(raw_hit_R[i]);
    SDL_FreeSurface(raw_hit_L[i]);
  }
  for (int i = 0; i < MAX_ATTACK_FRAMES; i++) {
    SDL_FreeSurface(raw_atk_R[i]);
    SDL_FreeSurface(raw_atk_L[i]);
  }
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

void SpawnEnemies(EnemyType *enemies, EnemyAssets *assets) {
  if ((rand() % 100) >= 1)
    return;

  for (int i = 0; i < NUMBER_OF_ENEMIES; i++) {
    if (enemies[i].alive)
      continue;

    // --- Standard Init Logic ---
    enemies[i].alive = 1;
    enemies[i].type = rand() % 2;
    enemies[i].health = 100;
    enemies[i].attack_timer = 0;
    enemies[i].stun_timer = 0.1; // small break before attack
    enemies[i].currentFrame = 0;
    enemies[i].animTimer = 0;

    // --- Position Logic ---
    if (rand() % 2 == 0) {
      enemies[i].X = (rand() % 2) ? 0 : LEVEL_WIDTH;
      enemies[i].Y = rand() % LEVEL_HEIGHT + LEVEL_HEIGHT / 4;
    } else {
      enemies[i].X = rand() % LEVEL_WIDTH;
      enemies[i].Y = (rand() % 2) ? LEVEL_HEIGHT / 4 : LEVEL_HEIGHT;
    }

    // --- Assign Shared Assets ---
    if (enemies[i].type == 0) {
      enemies[i].speed = 100;
      enemies[i].damage = 10;

      // Point to the loaded assets in the struct
      memcpy(enemies[i].walk_frames_right, assets->t0_walk_right,
             sizeof(assets->t0_walk_right));
      memcpy(enemies[i].walk_frames_left, assets->t0_walk_left,
             sizeof(assets->t0_walk_left));
      memcpy(enemies[i].hit_frames_right, assets->t0_hit_right,
             sizeof(assets->t0_hit_right));
      memcpy(enemies[i].hit_frames_left, assets->t0_hit_left,
             sizeof(assets->t0_hit_left));
      memcpy(enemies[i].attack_frames_right, assets->t0_attack_right,
             sizeof(assets->t0_attack_right));
      memcpy(enemies[i].attack_frames_left, assets->t0_attack_left,
             sizeof(assets->t0_attack_left));
    }

    else {
      enemies[i].speed = 150;
      enemies[i].damage = 20;
      enemies[i].ai_state = 0;
      enemies[i].ai_timer = 0;

      memcpy(enemies[i].walk_frames_right, assets->t1_walk_right,
             sizeof(assets->t1_walk_right));
      memcpy(enemies[i].walk_frames_left, assets->t1_walk_left,
             sizeof(assets->t1_walk_left));
      memcpy(enemies[i].hit_frames_right, assets->t1_hit_right,
             sizeof(assets->t1_hit_right));
      memcpy(enemies[i].hit_frames_left, assets->t1_hit_left,
             sizeof(assets->t1_hit_left));
      memcpy(enemies[i].attack_frames_right, assets->t1_attack_right,
             sizeof(assets->t1_attack_right));
      memcpy(enemies[i].attack_frames_left, assets->t1_attack_left,
             sizeof(assets->t1_attack_left));
    }
    break;
  }
}

void MoveEnemy(EnemyType *enemies, PlayerType *player, double delta) {
  for (int i = 0; i < NUMBER_OF_ENEMIES; i++) {
    if (!enemies[i].alive)
      continue;

    // --- 1. LOGIC TIMERS ---
    if (enemies[i].stun_timer > 0)
      enemies[i].stun_timer -= delta;

    if (enemies[i].attack_timer > 0)
      enemies[i].attack_timer -= delta;

    // --- 2. ANIMATION CONTROLLER ---
    // We need to know the MAX frames for the CURRENT state
    int maxFrames = MAX_WALK_FRAMES;

    // Check State Priorities (Must match DrawEnemies logic)
    if (enemies[i].stun_timer > 0) {
      maxFrames = MAX_HIT_FRAMES;
    } else if (enemies[i].attack_timer > (ATTACK_COOLDOWN - 0.5)) {
      maxFrames = MAX_ATTACK_FRAMES;
    } else {
      maxFrames = MAX_WALK_FRAMES;
    }

    // Advance Frame
    enemies[i].animTimer += delta;
    if (enemies[i].animTimer >= 0.15) { // Speed of animation
      enemies[i].animTimer -= 0.15;
      enemies[i].currentFrame++;

      // Loop animation
      if (enemies[i].currentFrame >= maxFrames) {
        enemies[i].currentFrame = 0;
      }
    }

    // Safety Fallback: If state changed abruptly (e.g. Walk [frame 7] -> Hit
    // [max 4]) we must clamp the frame immediately to prevent crashing in
    // DrawEnemies.
    if (enemies[i].currentFrame >= maxFrames) {
      enemies[i].currentFrame = 0;
    }
    // ----------------------------

    // If stunned, skip movement logic
    if (enemies[i].stun_timer > 0)
      continue;

    // --- 3. PHYSICS & AI ---
    double dx = player->X - enemies[i].X;
    double dy = player->Y - enemies[i].Y;
    double distance = sqrt(dx * dx + dy * dy);

    // Update Direction
    enemies[i].direction = (dx < 0) ? 1 : 0;

    // Attack Logic
    if (distance < ATTACK_RANGE && enemies[i].attack_timer <= 0) {
      int current_damage = enemies[i].damage;

      // Type 1 enraged damage
      if (enemies[i].type == 1 && enemies[i].ai_state == 1) {
        current_damage *= 2;
      }

      player->health -= current_damage;

      // Reset cooldown
      enemies[i].attack_timer = ATTACK_COOLDOWN;

      // FORCE RESET ANIMATION
      // This ensures the attack animation starts at frame 0 right now
      enemies[i].currentFrame = 0;
      enemies[i].animTimer = 0;
    }

    // Movement Logic (Type 0 - Standard Chaser)
    if (enemies[i].type == 0) {
      if (distance > 5) {
        enemies[i].X += (dx / distance) * enemies[i].speed * delta;
        enemies[i].Y += (dy / distance) * enemies[i].speed * delta;
      }
    }

    // Movement Logic (Type 1 - Charger)
    else if (enemies[i].type == 1) {
      if (enemies[i].ai_state == 0) {
        // ... (Your existing AI State 0 Logic) ...
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
        // ... (Your existing AI State 1 Logic) ...
        double chargeSpeed = enemies[i].speed * 2.5;
        enemies[i].X += enemies[i].chargeDirX * chargeSpeed * delta;
        enemies[i].Y += enemies[i].chargeDirY * chargeSpeed * delta;
        enemies[i].ai_timer -= delta;
        if (enemies[i].ai_timer <= 0) {
          enemies[i].ai_state = 2;
          enemies[i].ai_timer = 1.5;
        }
      } else if (enemies[i].ai_state == 2) {
        // ... (Your existing AI State 2 Logic) ...
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
