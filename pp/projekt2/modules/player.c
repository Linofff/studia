#include "../headers/player.h"
#include "../headers/drawing.h"
#include <stdio.h>

// --- COMBO DEFINITIONS ---
InputType COMBO_SEQ_TRIPLE[] = {IN_LIGHT, IN_LIGHT, IN_LIGHT};
InputType COMBO_SEQ_MIX[] = {IN_LIGHT, IN_HEAVY, IN_LIGHT};
InputType COMBO_SEQ_DASH_R[] = {IN_RIGHT, IN_RIGHT};
InputType COMBO_SEQ_DASH_L[] = {IN_LEFT, IN_LEFT};
InputType COMBO_SEQ_DASH_U[] = {IN_UP, IN_UP};
InputType COMBO_SEQ_DASH_D[] = {IN_DOWN, IN_DOWN};

// --- HELPER FUNCTIONS ---

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

void InitPlayer(PlayerType *player) {
  // 1. Physics & Stats
  player->speed = PLAYER_MOVE_SPEED;
  player->direction = RIGHT; // Default to facing right
  player->dy = 0;
  player->onGround = 1;
  player->health = PLAYER_HEALTH;
  player->facingLeft = 0;

  player->Y = LEVEL_HEIGHT / 2;
  player->X = LEVEL_WIDTH / 2;
  player->landingY = player->Y;

  // 2. State & Timers
  player->state = IDLE;
  player->score = 0;
  player->multiplier = 1;
  player->multiplierTimer = 0;
  player->multiplierScale = 1.0; // Reset visual scale

  player->attackTimer = 0;
  player->basicCooldownTimer = 0;
  player->comboCooldownTimer = 0;

  player->buffer.count = 0;
  memset(player->prev_keys, 0, sizeof(player->prev_keys));

  // 3. Load Animation Frames
  char filename[64];

  // Load Walk Frames (0 to MAX_WALK_FRAMES-1)
  for (int i = 0; i < MAX_WALK_FRAMES; i++) {
    sprintf(filename, "textures/fighter_run_%d.bmp", i + 1);
    player->walk_frames_right[i] = SDL_LoadBMP(filename);
    if (!player->walk_frames_right[i]) {
      printf("Failed to load %s\n", filename);
      // Fallback: Use a default if specific frame missing, or exit
    }
    player->walk_frames_left[i] =
        FlipSurfaceHorizontal(player->walk_frames_right[i]);
  }

  // Load Attack Frames (0 to MAX_ATTACK_FRAMES-1)
  for (int i = 0; i < MAX_ATTACK_FRAMES; i++) {
    sprintf(filename, "textures/fighter_combo_%d.bmp", i + 1);
    player->attack_frames_rigth[i] = SDL_LoadBMP(filename);
    if (!player->attack_frames_rigth[i]) {
      printf("Failed to load %s\n", filename);
    }
    player->attack_frames_left[i] =
        FlipSurfaceHorizontal(player->attack_frames_rigth[i]);
  }

  for (int i = 0; i < MAX_DASH_FRAMES; i++) {
    sprintf(filename, "textures/fighter_dash_%d.bmp", i + 1);
    player->dash_frames_rigth[i] = SDL_LoadBMP(filename);
    if (!player->dash_frames_rigth[i]) {
      printf("Failed to load %s\n", filename);
    }
    player->dash_frames_left[i] =
        FlipSurfaceHorizontal(player->dash_frames_rigth[i]);
  }

  for (int i = 0; i < MAX_AIR_FRAMES; i++) {
    sprintf(filename, "textures/fighter_air_%d.bmp", i + 1);
    player->air_frames_rigth[i] = SDL_LoadBMP(filename);
    if (!player->air_frames_rigth[i]) {
      printf("Failed to load %s\n", filename);
    }
    player->air_frames_left[i] =
        FlipSurfaceHorizontal(player->air_frames_rigth[i]);
  }

  // Animation Defaults
  player->currentFrame = 0;
  player->animTimer = 0;
  player->animSpeed = 0.15; // Default speed
  player->maxFrames = MAX_WALK_FRAMES;
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
  if (!player->attack_frames_rigth[0])
    return;

  double hitX, hitY;
  int hitW, hitH;

  int pHalfW = player->attack_frames_rigth[0]->w / 2;
  int pHalfH = player->attack_frames_rigth[0]->h / 2;

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

    // Priority 1: Combos
    if (combo != -1 && player->comboCooldownTimer <= 0) {
      ClearBuffer(&player->buffer);

      // Setup specific combo stats
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
    }
    // Priority 2: Basic Attacks
    else if (player->attackTimer <= 0 && player->basicCooldownTimer <= 0) {
      if (input == IN_LIGHT) {
        player->state = ATTACK_LIGHT;
        player->attackTimer = ATTACK_LIGHT_TIME;
        player->attack.damage = ATTACK_LIGHT_DAMAGE;
        player->basicCooldownTimer = ATTACK_LIGHT_TIME_DELAY;
        ProcessAttack(player, enemies);
      } else if (input == IN_HEAVY) {
        player->state = ATTACK_HEAVY;
        player->state = ATTACK_HEAVY;
        player->attack.damage = ATTACK_HEAVY_DAMAGE;
        player->attackTimer = ATTACK_HEAVY_TIME;
        player->basicCooldownTimer = ATTACK_HEAVY_TIME_DELAY;
        ProcessAttack(player, enemies);
      }
    }
  }

  // 3. ANIMATION STATE LOGIC
  // Determine which animation to play
  if (player->attackTimer > 0) {
    // We are attacking
    player->maxFrames = MAX_ATTACK_FRAMES;
    player->animSpeed = 0.1;
  }
  // FIX: Check if ANY movement key is pressed. Don't check direction here.
  else if (keyState[SDL_SCANCODE_A] || keyState[SDL_SCANCODE_D] ||
           keyState[SDL_SCANCODE_LEFT] || keyState[SDL_SCANCODE_RIGHT]) {
    // We are walking
    player->maxFrames = MAX_WALK_FRAMES;
    player->animSpeed = 0.15;
  } else {
    // Idle - Only happens if NOT attacking and NOT moving
    player->currentFrame = 0;
  }

  // Update the frame counter (unless idle)
  // FIX: Only update animation if NOT idle
  if (player->currentFrame != 0 || player->state != IDLE ||
      keyState[SDL_SCANCODE_A] || keyState[SDL_SCANCODE_D] ||
      keyState[SDL_SCANCODE_LEFT] || keyState[SDL_SCANCODE_RIGHT]) {

    UpdateAnimation(&player->currentFrame, &player->animTimer,
                    player->maxFrames, player->animSpeed, delta);
  }

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
    if (player->attackTimer <= 0) {
      player->state = IDLE;
    }
  }

  if (player->basicCooldownTimer > 0)
    player->basicCooldownTimer -= delta;
  if (player->comboCooldownTimer > 0)
    player->comboCooldownTimer -= delta;

  // Pulse Decay
  if (player->multiplierScale > 1.0) {
    player->multiplierScale -= delta * 2.0;
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
