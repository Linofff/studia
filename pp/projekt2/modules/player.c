#include "../headers/player.h"
#include "../headers/drawing.h"
#include "../headers/utils.h"

// --- COMBO DEFINITIONS ---
InputType COMBO_SEQ_TRIPLE[] = {IN_LIGHT, IN_LIGHT, IN_LIGHT};
InputType COMBO_SEQ_MIX[] = {IN_LIGHT, IN_HEAVY, IN_LIGHT};
InputType COMBO_SEQ_DASH_R[] = {IN_RIGHT, IN_RIGHT};
InputType COMBO_SEQ_DASH_L[] = {IN_LEFT, IN_LEFT};
InputType COMBO_SEQ_DASH_U[] = {IN_UP, IN_UP};
InputType COMBO_SEQ_DASH_D[] = {IN_DOWN, IN_DOWN};

// --- HELPER FUNCTIONS ---

// Helper to load a sequence of frames and their flipped counterparts

// Helper to cycle animation frames based on time
void UpdateAnimation(int *frame, double *timer, int maxFrames, double speed,
                     double delta) {
  *timer += delta;
  if (*timer >= speed) {
    *timer -= speed;
    *frame = (*frame + 1);
    if (*frame >= maxFrames) {
      *frame = 0;
    }
  }
}

int CheckCollision(double x1, double y1, int w1, int h1, double x2, double y2,
                   int w2, int h2) {
  return (x1 < x2 + w2 && x1 + w1 > x2 && y1 < y2 + h2 && y1 + h1 > y2);
}

void PushInput(InputBuffer *buf, InputType input, double time) {
  if (buf->count < BUFFER_SIZE) {
    buf->events[buf->count].input = input;
    buf->events[buf->count].timestamp = time;
    buf->count++;
  } else {
    for (int i = 0; i < BUFFER_SIZE - 1; i++)
      buf->events[i] = buf->events[i + 1];
    buf->events[BUFFER_SIZE - 1].input = input;
    buf->events[BUFFER_SIZE - 1].timestamp = time;
  }
}

void ClearBuffer(InputBuffer *buf) { buf->count = 0; }

// --- INITIALIZATION ---

void LoadPlayerAssets(PlayerType *player) {
  // Clean, readable, one-line-per-animation loading
  LoadAnimationSequence("textures/fighter_run", MAX_WALK_FRAMES,
                        player->walk_frames_right, player->walk_frames_left);
  LoadAnimationSequence("textures/fighter_combo", MAX_ATTACK_FRAMES,
                        player->attack_frames_right,
                        player->attack_frames_left);
  LoadAnimationSequence("textures/fighter_dash", MAX_DASH_FRAMES,
                        player->dash_frames_right, player->dash_frames_left);
  LoadAnimationSequence("textures/fighter_jump", MAX_AIR_FRAMES,
                        player->air_frames_right, player->air_frames_left);
  LoadAnimationSequence("textures/fighter_Idle", MAX_IDLE_FRAMES,
                        player->idle_frames_right, player->idle_frames_left);
  LoadAnimationSequence("textures/fighter_hit", MAX_HIT_FRAMES,
                        player->hit_frames_right, player->hit_frames_left);
  LoadAnimationSequence("textures/fighter_death", MAX_DEATH_FRAMES,
                        player->death_frames_right, player->death_frames_left);

  // Set Defaults
  player->animSpeed = 0.15;
  player->maxFrames = MAX_WALK_FRAMES;
}

void ResetPlayer(PlayerType *player) {
  // 1. Physics & Position
  player->X = LEVEL_WIDTH / 2;
  player->Y = LEVEL_HEIGHT / 2;
  player->landingY = player->Y;

  player->speed = PLAYER_MOVE_SPEED;
  player->dy = 0;
  player->direction = RIGHT;
  player->facingLeft = 0;
  player->onGround = 1;

  // 2. Gameplay Stats
  player->health = PLAYER_HEALTH;
  player->state = IDLE;
  player->score = 0;

  // 3. Reset Timers
  player->animTimer = 0;
  player->currentFrame = 0;
  player->attackTimer = 0;
  player->basicCooldownTimer = 0;
  player->comboCooldownTimer = 0;

  // 4. Reset Multiplier Visuals
  player->multiplier = 1;
  player->multiplierTimer = 0;
  player->multiplierScale = 1.0;

  // 5. Clear Input Buffer
  player->buffer.count = 0;
  memset(player->prev_keys, 0, sizeof(player->prev_keys));
}

// --- INPUT & COMBOS ---

int check(InputBuffer *buf, InputType *seq, int len, double currentTime) {
  if (buf->count < len)
    return 0;
  for (int i = 0; i < len; i++) {
    int bufIdx = buf->count - 1 - i;
    int seqIdx = len - 1 - i;
    if (buf->events[bufIdx].input != seq[seqIdx])
      return 0;
    if (currentTime - buf->events[bufIdx].timestamp > BUFFER_WINDOW)
      return 0;
  }
  return 1;
}

int CheckCombos(InputBuffer *buf, double currentTime) {
  if (buf->count == 0)
    return -1;

  if (check(buf, COMBO_SEQ_TRIPLE, 3, currentTime))
    return COMBO_TRIPLE;
  if (check(buf, COMBO_SEQ_MIX, 3, currentTime))
    return COMBO_MIX;
  if (check(buf, COMBO_SEQ_DASH_D, 2, currentTime))
    return DASH;
  if (check(buf, COMBO_SEQ_DASH_U, 2, currentTime))
    return DASH;
  if (check(buf, COMBO_SEQ_DASH_R, 2, currentTime))
    return DASH;
  if (check(buf, COMBO_SEQ_DASH_L, 2, currentTime))
    return DASH;

  return -1;
}

// --- ATTACK LOGIC ---

void ProcessAttack(PlayerType *player, EnemyType *enemies) {
  // Safety check: ensure we have at least one frame loaded
  if (!player->attack_frames_right[0])
    return;

  double hitX, hitY;
  int hitW, hitH;

  int pHalfW = player->attack_frames_right[0]->w / 2;
  int pHalfH = player->attack_frames_right[0]->h / 2;

  // 1. Determine Range
  int range;
  if (player->state == ATTACK_LIGHT)
    range = 60;
  else if (player->state == COMBO_TRIPLE)
    range = 120;
  else if (player->state == DASH)
    range = 200;
  else
    range = 100;

  // 2. Calculate Hitbox (Vertical vs Horizontal)
  if (player->direction == UP) {
    hitW = HITBOX_H;
    hitH = range;
    hitX = player->X - (hitW / 2);
    hitY = player->Y - pHalfH - hitH;
  } else if (player->direction == DOWN) {
    hitW = HITBOX_H;
    hitH = range;
    hitX = player->X - (hitW / 2);
    hitY = player->Y + pHalfH;
  } else if (player->direction == LEFT) {
    hitW = range;
    hitH = HITBOX_H;
    hitX = player->X - pHalfW - hitW;
    hitY = player->Y - (hitH / 2);
  } else { // RIGHT
    hitW = range;
    hitH = HITBOX_H;
    hitX = player->X + pHalfW;
    hitY = player->Y - (hitH / 2);
  }

  // 3. Collision with Enemies
  for (int i = 0; i < NUMBER_OF_ENEMIES; i++) {
    if (!enemies[i].alive || !enemies[i].walk_frames_right[0])
      continue;

    int eW = enemies[i].walk_frames_right[0]->w;
    int eH = enemies[i].walk_frames_right[0]->h;
    double eLeft = enemies[i].X - (eW / 2);
    double eTop = enemies[i].Y - (eH / 2);

    if (CheckCollision(hitX, hitY, hitW, hitH, eLeft, eTop, eW, eH)) {
      enemies[i].health -= player->attack.damage;
      if (enemies[i].health <= 0)
        enemies[i].alive = 0;

      enemies[i].stun_timer = ENEMY_STUN;

      // Flashy Multiplier Logic
      player->score += 100 * player->multiplier;
      player->multiplier++;
      player->multiplierTimer = 2.0;
      player->multiplierScale = 2.0; // Trigger "Pulse" effect
    }
  }
}

// --- MAIN UPDATE LOOP ---
void UpdatePlayer(PlayerType *player, EnemyType *enemies, double delta,
                  double worldTime) {
  if (player->health <= 0 && player->state != DEATH_ANIM) {
    player->health = 0;
    player->state = DEATH_ANIM;
    player->currentFrame = 0;
    player->animTimer = 0;
    player->attackTimer = 0;
  }

  // --- 2. HANDLE DEATH STATE ---
  if (player->state == DEATH_ANIM) {
    player->animTimer += delta;
    if (player->animTimer >= 0.2) {
      player->animTimer = 0;
      if (player->currentFrame < MAX_DEATH_FRAMES - 1) {
        player->currentFrame++;
      }
    }

    if (!player->onGround) {
      player->dy += GRAVITY * delta;
      player->Y += player->dy * delta;
      if (player->Y >= player->landingY) {
        player->Y = player->landingY;
        player->onGround = 1;
        player->dy = 0;
      }
    }
    return;
  }

  const Uint8 *keyState = SDL_GetKeyboardState(NULL);

  // 1. INPUT
  InputType input = IN_NONE;
  if (keyState[SDL_SCANCODE_J] && !player->prev_keys[SDL_SCANCODE_J])
    input = IN_LIGHT;
  else if (keyState[SDL_SCANCODE_K] && !player->prev_keys[SDL_SCANCODE_K])
    input = IN_HEAVY;
  else if (keyState[SDL_SCANCODE_A] && !player->prev_keys[SDL_SCANCODE_A])
    input = IN_LEFT;
  else if (keyState[SDL_SCANCODE_D] && !player->prev_keys[SDL_SCANCODE_D])
    input = IN_RIGHT;
  else if (keyState[SDL_SCANCODE_W] && !player->prev_keys[SDL_SCANCODE_W])
    input = IN_UP;
  else if (keyState[SDL_SCANCODE_S] && !player->prev_keys[SDL_SCANCODE_S])
    input = IN_DOWN;

  player->prev_keys[SDL_SCANCODE_J] = keyState[SDL_SCANCODE_J];
  player->prev_keys[SDL_SCANCODE_K] = keyState[SDL_SCANCODE_K];
  player->prev_keys[SDL_SCANCODE_A] = keyState[SDL_SCANCODE_A];
  player->prev_keys[SDL_SCANCODE_D] = keyState[SDL_SCANCODE_D];
  player->prev_keys[SDL_SCANCODE_W] = keyState[SDL_SCANCODE_W];
  player->prev_keys[SDL_SCANCODE_S] = keyState[SDL_SCANCODE_S];

  // 2. COMBO & ATTACK STATE
  if (input != IN_NONE) {
    PushInput(&player->buffer, input, worldTime);
    int combo = CheckCombos(&player->buffer, worldTime);

    if (combo != -1 && player->comboCooldownTimer <= 0) {
      ClearBuffer(&player->buffer);
      player->currentFrame = 0;
      player->animTimer = 0;

      if (combo == COMBO_TRIPLE) {
        player->state = COMBO_TRIPLE;
        player->attackTimer = 0.5;
        player->attack.damage = ATTACK_LIGHT_COMBO_DAMAGE;
        player->comboCooldownTimer = ATTACK_LIGHT_COMBO_DELAY;
        ProcessAttack(player, enemies);
      } else if (combo == COMBO_MIX) {
        player->state = COMBO_MIX;
        player->attackTimer = 0.6;
        player->attack.damage = ATTACK_MIX_COMBO_DAMAGE;
        player->comboCooldownTimer = ATTACK_MIX_COMBO_DELAY;
        ProcessAttack(player, enemies);
      } else if (combo == DASH) {
        player->state = DASH;
        player->attack.damage = ATTACK_DASH_COMBO_DAMAGE;
        player->attackTimer = 0.25;
        player->comboCooldownTimer = ATTACK_DASH_COMBO_DELAY;
        ProcessAttack(player, enemies);
      }
    } else if (player->attackTimer <= 0 && player->basicCooldownTimer <= 0) {
      if (input == IN_LIGHT) {
        player->currentFrame = 0;
        player->animTimer = 0;
        player->state = ATTACK_LIGHT;
        player->attackTimer = ATTACK_LIGHT_TIME;
        player->attack.damage = ATTACK_LIGHT_DAMAGE;
        player->basicCooldownTimer = ATTACK_LIGHT_TIME_DELAY;
        ProcessAttack(player, enemies);
      } else if (input == IN_HEAVY) {
        player->currentFrame = 0;
        player->animTimer = 0;
        player->state = ATTACK_HEAVY;
        player->attack.damage = ATTACK_HEAVY_DAMAGE;
        player->attackTimer = ATTACK_HEAVY_TIME;
        player->basicCooldownTimer = ATTACK_HEAVY_TIME_DELAY;
        ProcessAttack(player, enemies);
      }
    }
  }

  // --- 3. ANIMATION STATE LOGIC (CORRECTED) ---
  // Priority 1: Attacks/Dash (Locked by timer)
  if (player->attackTimer > 0) {
    if (player->state == DASH) {
      player->maxFrames = MAX_DASH_FRAMES;
      player->animSpeed = 0.08;
    } else {
      player->maxFrames = MAX_ATTACK_FRAMES;
      player->animSpeed = 0.1;
    }
  }
  // Priority 2: Running (Locked by Input)
  // Added W/S/UP/DOWN so vertical movement triggers RUNNING too
  else if (keyState[SDL_SCANCODE_A] || keyState[SDL_SCANCODE_D] ||
           keyState[SDL_SCANCODE_W] || keyState[SDL_SCANCODE_S] ||
           keyState[SDL_SCANCODE_LEFT] || keyState[SDL_SCANCODE_RIGHT] ||
           keyState[SDL_SCANCODE_UP] || keyState[SDL_SCANCODE_DOWN]) {

    // Explicitly set the state to RUNNING
    if (player->state != RUNNING) {
      player->state = RUNNING;
      player->currentFrame = 0; // Reset frame to start run cycle cleanly
    }
    player->maxFrames = MAX_WALK_FRAMES;
    player->animSpeed = 0.15;
  }
  // Priority 3: Idle (Fallback)
  else {
    if (player->state != IDLE) {
      player->state = IDLE;
      player->currentFrame = 0;
    }
    player->maxFrames = MAX_IDLE_FRAMES;
    player->animSpeed = 0.2;
  }

  UpdateAnimation(&player->currentFrame, &player->animTimer, player->maxFrames,
                  player->animSpeed, delta);

  // 4. MOVEMENT LOGIC
  if (player->state == DASH) {
    double dashSpeed = player->speed * 3.0;
    if (player->direction == RIGHT)
      player->X += dashSpeed * delta;
    else if (player->direction == LEFT)
      player->X -= dashSpeed * delta;
    else if (player->direction == UP)
      player->Y -= dashSpeed * delta;
    else if (player->direction == DOWN)
      player->Y += dashSpeed * delta;
  } else if (player->attackTimer <= 0) {
    if (keyState[SDL_SCANCODE_A] || keyState[SDL_SCANCODE_LEFT]) {
      player->X -= player->speed * delta;
      player->direction = LEFT;
      player->facingLeft = 1;
    }
    if (keyState[SDL_SCANCODE_D] || keyState[SDL_SCANCODE_RIGHT]) {
      player->X += player->speed * delta;
      player->direction = RIGHT;
      player->facingLeft = 0;
    }
  }

  // 5. VERTICAL PHYSICS
  if (player->onGround) {
    if (player->attackTimer <= 0) {
      if (keyState[SDL_SCANCODE_W] || keyState[SDL_SCANCODE_UP]) {
        player->Y -= player->speed * delta;
        player->direction = UP;
      }
      if (keyState[SDL_SCANCODE_S] || keyState[SDL_SCANCODE_DOWN]) {
        player->Y += player->speed * delta;
        player->direction = DOWN;
      }
    }
    player->landingY = player->Y;
    player->dy = 0;

    if (keyState[SDL_SCANCODE_SPACE]) {
      player->dy = -JUMP_FORCE;
      player->onGround = 0;
    }
  } else {
    player->dy += GRAVITY * delta;
    player->Y += player->dy * delta;

    if (player->Y >= player->landingY) {
      player->Y = player->landingY;
      player->onGround = 1;
      player->dy = 0;
    }
  }

  // 6. TIMERS & CLEANUP
  if (player->attackTimer > 0) {
    player->attackTimer -= delta;
    // FIX: Removed the logic that forced player->state = IDLE here.
    // Section 3 now handles the transition back to IDLE or RUNNING
    // automatically.
  }

  if (player->basicCooldownTimer > 0)
    player->basicCooldownTimer -= delta;
  if (player->comboCooldownTimer > 0)
    player->comboCooldownTimer -= delta;

  // Pulse Decay
  if (player->multiplierScale > 1.0) {
    if (player->multiplierTimer <= 0)
      player->multiplierScale = 1;
    if (player->multiplierScale < 1.0)
      player->multiplierScale = 1.0;
  }

  if (player->multiplierTimer > 0) {
    player->multiplierTimer -= delta;
  } else {
    player->multiplier = 1;
  }

  // Bounds
  if (player->X < 0)
    player->X = 0;
  if (player->X > LEVEL_WIDTH)
    player->X = LEVEL_WIDTH;
  if (player->onGround) {
    if (player->Y < LEVEL_HEIGHT / 4 - 20)
      player->Y = LEVEL_HEIGHT / 4 - 20;
    if (player->Y > LEVEL_HEIGHT)
      player->Y = LEVEL_HEIGHT;
  }
}
