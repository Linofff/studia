#include "../headers/player.h"
#include "../headers/drawing.h"

// Triple Tap (Light, Light, Light)
InputType COMBO_SEQ_TRIPLE[] = {IN_LIGHT, IN_LIGHT, IN_LIGHT};
// Mix Sequence (Light, Heavy, Light)
InputType COMBO_SEQ_MIX[] = {IN_LIGHT, IN_HEAVY, IN_LIGHT};
// Dash Right (Right, Right)
InputType COMBO_SEQ_DASH_R[] = {IN_RIGHT, IN_RIGHT};
// Dash Left (Left, Left)
InputType COMBO_SEQ_DASH_L[] = {IN_LEFT, IN_LEFT};

int CheckCollision(double x1, double y1, int w1, int h1, double x2, double y2,
                   int w2, int h2) {
  return (x1 < x2 + w2 && x1 + w1 > x2 && y1 < y2 + h2 && y1 + h2 > y2);
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

void InitPlayer(PlayerType *player) {
  player->speed = PLAYER_MOVE_SPEED;
  player->direction = 0;
  player->dy = 0;
  player->onGround = 1;

  player->Y = LEVEL_HEIGHT / 2;
  player->X = LEVEL_WIDTH / 2;
  player->landingY = player->Y;

  player->state = IDLE;
  player->score = 0;
  player->multiplier = 1;
  player->multiplierTimer = 0;

  player->attackTimer = 0;
  player->basicCooldownTimer = 0;
  player->comboCooldownTimer = 0;

  player->buffer.count = 0;
  memset(player->prev_keys, 0, sizeof(player->prev_keys));

  player->surface_right = SDL_LoadBMP("textures/player.bmp");
  if (player->surface_right == NULL) {
    printf("Error loading player bmp: %s\n", SDL_GetError());
    exit(1);
  }
  player->surface_left = FlipSurfaceHorizontal(player->surface_right);
  if (player->surface_left == NULL) {
    printf("Error creating flipped surface: %s\n", SDL_GetError());
  }
}

int check(InputBuffer *buf, InputType *seq, int len, double currentTime) {
  if (buf->count < len)
    return 0;
  for (int i = 0; i < len; i++) {
    // Compare buffer end vs sequence end
    int bufIdx = buf->count - 1 - i;
    int seqIdx = len - 1 - i;

    // 1. Check Input Type
    if (buf->events[bufIdx].input != seq[seqIdx])
      return 0;

    // 2. Check Timing (Must be recent)
    if (currentTime - buf->events[bufIdx].timestamp > BUFFER_WINDOW)
      return 0;
  }
  return 1;
}

int CheckCombos(InputBuffer *buf, double currentTime) {
  if (buf->count == 0)
    return -1;

  // PRIORITY 1: Complex Combos (Longer first)
  if (check(buf, COMBO_SEQ_TRIPLE, 3, currentTime))
    return COMBO_TRIPLE;
  if (check(buf, COMBO_SEQ_MIX, 3, currentTime))
    return COMBO_MIX;

  // PRIORITY 2: Movement Combos (Dash)
  if (check(buf, COMBO_SEQ_DASH_R, 2, currentTime))
    return DASH;
  if (check(buf, COMBO_SEQ_DASH_L, 2, currentTime))
    return DASH;

  return -1;
}

void ProcessAttack(PlayerType *player, EnemyType *enemies) {
  if (!player->surface_right)
    return;

  double hitX, hitY;
  int hitW;

  int pHalfW = player->surface_right->w / 2;
  hitY = player->Y - (HITBOX_H / 2);

  // Different hitboxes for different attacks
  if (player->state == ATTACK_LIGHT) {
    hitW = 60;
  } else if (player->state == COMBO_TRIPLE) {
    hitW = 120; // Massive range
  } else if (player->state == DASH) {
    hitW = 200; // Heavy / Mix
  } else {
    hitW = 100;
  }

  if (player->direction == 0) {
    hitX = player->X + pHalfW;
  } else {
    hitX = player->X - pHalfW - hitW;
  }

  int hit = 0;
  for (int i = 0; i < NUMBER_OF_ENEMIES; i++) {
    if (!enemies[i].alive || !enemies[i].surface_right)
      continue;

    int eW = enemies[i].surface_right->w;
    int eH = enemies[i].surface_right->h;

    double eLeft = enemies[i].X - (eW / 2);
    double eTop = enemies[i].Y - (eH / 2);

    if (CheckCollision(hitX, hitY, hitW, HITBOX_H, eLeft, eTop, eW, eH)) {
      enemies[i].health -= player->attack.damage;
      if (enemies[i].health <= 0)
        enemies[i].alive = 0;
      hit = 1;
    }
  }

  if (hit) {
    player->score += 100 * player->multiplier;
    player->multiplier++;
    player->multiplierTimer = 2.0;
  }
}

void UpdatePlayer(PlayerType *player, EnemyType *enemies, double delta,
                  double worldTime) {
  const Uint8 *keyState = SDL_GetKeyboardState(NULL);

  // 1. INPUT DETECTION (Edge Detection for Buffer)
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

  // 2. BUFFER & COMBO LOGIC
  if (input != IN_NONE) {
    PushInput(&player->buffer, input, worldTime);

    // Check for specific combos
    int combo = CheckCombos(&player->buffer, worldTime);

    // --- ADVANCED LOGIC START ---

    // PRIORITY 1: COMBOS (Ignore Cooldowns / Cancel Animations)
    if (combo != -1 && player->comboCooldownTimer <= 0) {
      ClearBuffer(&player->buffer); // Consume inputs

      if (combo == COMBO_TRIPLE) {
        player->state = COMBO_TRIPLE;
        player->attackTimer = 0.5;
        player->attack.damage = ATTACK_LIGHT_COMBO_DAMAGE;
        player->comboCooldownTimer = ATTACK_LIGHT_COMBO_DELAY;
        SDL_SetSurfaceColorMod(player->surface_right, 0, 255, 255); // Cyan
        SDL_SetSurfaceColorMod(player->surface_left, 0, 255, 255);
        ProcessAttack(player, enemies);
      } else if (combo == COMBO_MIX) {
        player->state = COMBO_MIX;
        player->attackTimer = 0.6;
        player->attack.damage = ATTACK_MIX_COMBO_DAMAGE;
        player->comboCooldownTimer = ATTACK_MIX_COMBO_DELAY;
        SDL_SetSurfaceColorMod(player->surface_right, 255, 0, 255); // Purple
        SDL_SetSurfaceColorMod(player->surface_left, 255, 0, 255);
        ProcessAttack(player, enemies);
      } else if (combo == DASH) {
        player->state = DASH;
        player->attack.damage = ATTACK_DASH_COMBO_DAMAGE;
        player->attackTimer = 0.25;
        player->comboCooldownTimer = ATTACK_DASH_COMBO_DELAY;
        ProcessAttack(player, enemies);
        SDL_SetSurfaceColorMod(player->surface_right, 0, 0, 255); // Blue
        SDL_SetSurfaceColorMod(player->surface_left, 0, 0, 255);
        player->direction = (input == IN_LEFT) ? 1 : 0;
      }
    }

    // PRIORITY 2: BASIC ATTACKS (Strict Cooldown Check)
    // Only execute if animation is done AND Cooldown is done
    else if (player->attackTimer <= 0 && player->basicCooldownTimer <= 0) {
      if (input == IN_LIGHT) {
        player->state = ATTACK_LIGHT;
        player->attackTimer = ATTACK_LIGHT_TIME;
        player->attack.damage = ATTACK_LIGHT_DAMAGE;
        player->basicCooldownTimer = ATTACK_LIGHT_TIME_DELAY;
        SDL_SetSurfaceColorMod(player->surface_right, 255, 255, 0); // Yellow
        SDL_SetSurfaceColorMod(player->surface_left, 255, 255, 0);
        ProcessAttack(player, enemies);
      } else if (input == IN_HEAVY) {
        player->state = ATTACK_HEAVY;
        player->attack.damage = ATTACK_HEAVY_DAMAGE;
        player->attackTimer = ATTACK_HEAVY_TIME;
        player->basicCooldownTimer = ATTACK_HEAVY_TIME_DELAY;
        SDL_SetSurfaceColorMod(player->surface_right, 255, 0, 0); // Red
        SDL_SetSurfaceColorMod(player->surface_left, 255, 0, 0);
        ProcessAttack(player, enemies);
      }
    }
  }

  // 3. MOVEMENT LOGIC

  // DASH MOVEMENT
  if (player->state == DASH) {
    double dashSpeed = player->speed * 3.0;
    if (player->direction == 1)
      player->X -= dashSpeed * delta;
    else
      player->X += dashSpeed * delta;
  }
  // STANDARD MOVEMENT
  // Allow movement during Cooldown, but not during Active Attack Frames
  else if (player->attackTimer <= 0) {
    if (keyState[SDL_SCANCODE_A] || keyState[SDL_SCANCODE_LEFT]) {
      player->X -= player->speed * delta;
      player->direction = 1;
    }
    if (keyState[SDL_SCANCODE_D] || keyState[SDL_SCANCODE_RIGHT]) {
      player->X += player->speed * delta;
      player->direction = 0;
    }
  }

  // Vertical / Ground Physics
  if (player->onGround) {
    if (player->attackTimer <= 0) {
      if (keyState[SDL_SCANCODE_W] || keyState[SDL_SCANCODE_UP])
        player->Y -= player->speed * delta;
      if (keyState[SDL_SCANCODE_S] || keyState[SDL_SCANCODE_DOWN])
        player->Y += player->speed * delta;
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

  // 4. TIMERS & CLEANUP

  // Reset State/Colors when ATTACK timer ends (ignore cooldown for visual
  // reset)
  if (player->attackTimer > 0) {
    player->attackTimer -= delta;
    if (player->attackTimer <= 0) {
      // Don't reset to IDLE if we are transitioning into a combo (handled
      // above) But generally safe to reset here, next frame picks up new state
      player->state = IDLE;
      SDL_SetSurfaceColorMod(player->surface_right, 255, 255, 255);
      SDL_SetSurfaceColorMod(player->surface_left, 255, 255, 255);
    }
  }

  // Cooldown ticks down independently
  if (player->basicCooldownTimer > 0)
    player->basicCooldownTimer -= delta;

  if (player->comboCooldownTimer)
    player->comboCooldownTimer -= delta;

  if (player->multiplierTimer > 0) {
    player->multiplierTimer -= delta;
  } else {
    player->multiplier = 1;
  }

  // Bounds Checking
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
