#include "player.h"
#include "main.h"
#include "utils.h"

// combo sequances
const InputType COMBO_SEQ_TRIPLE[] = {IN_LIGHT, IN_LIGHT, IN_LIGHT};
const InputType COMBO_SEQ_MIX[] = {IN_LIGHT, IN_HEAVY, IN_LIGHT};
const InputType COMBO_SEQ_DASH_R[] = {IN_RIGHT, IN_RIGHT};
const InputType COMBO_SEQ_DASH_L[] = {IN_LEFT, IN_LEFT};
const InputType COMBO_SEQ_DASH_U[] = {IN_UP, IN_UP};
const InputType COMBO_SEQ_DASH_D[] = {IN_DOWN, IN_DOWN};

void LoadPlayerAssets(PlayerType *player) {
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

  player->animSpeed = 0.15;
  player->maxFrames = MAX_WALK_FRAMES;
}

void ResetPlayer(PlayerType *player) {
  player->X = LEVEL_WIDTH / 2;
  player->Y = LEVEL_HEIGHT / 2;
  player->landingY = player->Y;

  player->speed = PLAYER_MOVE_SPEED;
  player->dy = 0;
  player->direction = RIGHT;
  player->facingLeft = 0;
  player->onGround = 1;
  player->wasHitTimer = 0;

  player->health = PLAYER_HEALTH;
  player->state = IDLE;
  player->score = 0;

  player->animTimer = 0;
  player->currentFrame = 0;
  player->attackTimer = 0;
  player->basicCooldownTimer = 0;
  player->comboCooldownTimer = 0;

  player->multiplier = 1;
  player->multiplierTimer = 0;

  player->buffer.count = 0;
  memset(player->prev_keys, 0, sizeof(player->prev_keys));
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

int check(InputBuffer *buf, const InputType *seq, int len, double currentTime) {
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

InputType ResolveInput(const Uint8 *keyState, int *prevKeys) {
  InputType input = IN_NONE;
  if (keyState[SDL_SCANCODE_J] && !prevKeys[SDL_SCANCODE_J])
    input = IN_LIGHT;
  else if (keyState[SDL_SCANCODE_K] && !prevKeys[SDL_SCANCODE_K])
    input = IN_HEAVY;
  else if (keyState[SDL_SCANCODE_A] && !prevKeys[SDL_SCANCODE_A])
    input = IN_LEFT;
  else if (keyState[SDL_SCANCODE_D] && !prevKeys[SDL_SCANCODE_D])
    input = IN_RIGHT;
  else if (keyState[SDL_SCANCODE_W] && !prevKeys[SDL_SCANCODE_W])
    input = IN_UP;
  else if (keyState[SDL_SCANCODE_S] && !prevKeys[SDL_SCANCODE_S])
    input = IN_DOWN;

  prevKeys[SDL_SCANCODE_J] = keyState[SDL_SCANCODE_J];
  prevKeys[SDL_SCANCODE_K] = keyState[SDL_SCANCODE_K];
  prevKeys[SDL_SCANCODE_A] = keyState[SDL_SCANCODE_A];
  prevKeys[SDL_SCANCODE_D] = keyState[SDL_SCANCODE_D];
  prevKeys[SDL_SCANCODE_W] = keyState[SDL_SCANCODE_W];
  prevKeys[SDL_SCANCODE_S] = keyState[SDL_SCANCODE_S];
  return input;
}

void UpdateAnimation(int *frame, double *timer, int maxFrames, double speed,
                     double delta) {
  *timer += delta;
  if (*timer >= speed) {
    *timer -= speed;
    *frame += 1;
    if (*frame >= maxFrames) {
      *frame = 0;
    }
  }
}

void HandleAnimationState(PlayerType *player, const Uint8 *keyState,
                          double delta) {
  if (player->attackTimer > 0) {
    if (player->state == DASH) {
      player->maxFrames = MAX_DASH_FRAMES;
      player->animSpeed = 0.08;
    } else {
      player->maxFrames = MAX_ATTACK_FRAMES;
      player->animSpeed = 0.1;
    }
  } else if (keyState[SDL_SCANCODE_A] || keyState[SDL_SCANCODE_D] ||
             keyState[SDL_SCANCODE_W] || keyState[SDL_SCANCODE_S]) {
    player->state = RUNNING;
    player->maxFrames = MAX_WALK_FRAMES;
    player->animSpeed = 0.15;
  } else {
    player->state = IDLE;
    player->maxFrames = MAX_IDLE_FRAMES;
    player->animSpeed = 0.2;
  }

  UpdateAnimation(&player->currentFrame, &player->animTimer, player->maxFrames,
                  player->animSpeed, delta);
}

int CheckCollision(double x1, double y1, int w1, int h1, double x2, double y2,
                   int w2, int h2) {
  return (x1 < x2 + w2 && x1 + w1 > x2 && y1 < y2 + h2 && y1 + h1 > y2);
}

void GetAttackHitbox(PlayerType *player, double *hitX, double *hitY, int *hitW,
                     int *hitH) {
  int pHalfW = player->attack_frames_right[0]->w / 2;
  int pHalfH = player->attack_frames_right[0]->h / 2;
  int range;

  if (player->state == ATTACK_LIGHT)
    range = ATTACK_LIGHT_RANGE;
  else if (player->state == ATTACK_HEAVY)
    range = ATTACK_HEAVY_RANGE;
  else if (player->state == COMBO_MIX)
    range = ATTACK_MIX_COMBO_RANGE;
  else if (player->state == COMBO_TRIPLE)
    range = ATTACK_LIGHT_COMBO_RANGE;
  else if (player->state == DASH)
    range = ATTACK_DASH_COMBO_RANGE;
  else
    range = 100; // default range

  if (player->direction == UP) {
    *hitW = ATTACK_HITBOX_H;
    *hitH = range;
    *hitX = player->X - (*hitW / 2);
    *hitY = player->Y - pHalfH - *hitH;
  } else if (player->direction == DOWN) {
    *hitW = ATTACK_HITBOX_H;
    *hitH = range;
    *hitX = player->X - (*hitW / 2);
    *hitY = player->Y + pHalfH;
  } else if (player->direction == LEFT) {
    *hitW = range;
    *hitH = ATTACK_HITBOX_H;
    *hitX = player->X - pHalfW - *hitW;
    *hitY = player->Y - (*hitH / 2);
  } else {
    *hitW = range;
    *hitH = ATTACK_HITBOX_H;
    *hitX = player->X + pHalfW;
    *hitY = player->Y - (*hitH / 2);
  }
}

void ApplyAttackDamage(PlayerType *player, EnemyType *enemies, double hX,
                       double hY, int hW, int hH) {
  for (int i = 0; i < NUMBER_OF_ENEMIES; i++) {
    if (!enemies[i].alive || !enemies[i].walk_frames_right[0])
      continue;

    int eW = enemies[i].walk_frames_right[0]->w;
    int eH = enemies[i].walk_frames_right[0]->h;
    double eLeft = enemies[i].X - (eW / 2);
    double eTop = enemies[i].Y - (eH / 2);

    if (CheckCollision(hX, hY, hW, hH, eLeft, eTop, eW, eH)) {
      enemies[i].health -= player->damage;
      if (enemies[i].health <= 0)
        enemies[i].alive = 0;

      enemies[i].stun_timer = ENEMY_STUN;

      player->score += POINTS_PER_HIT * player->multiplier;
      player->multiplier++;
      player->multiplierTimer = 2.0;
    }
  }
}

void ProcessAttack(PlayerType *player, EnemyType *enemies) {
  if (!player->attack_frames_right[0])
    return;

  double hitX, hitY;
  int hitW, hitH;
  GetAttackHitbox(player, &hitX, &hitY, &hitW, &hitH);
  ApplyAttackDamage(player, enemies, hitX, hitY, hitW, hitH);
}

void HandleCombatState(PlayerType *player, EnemyType *enemies, InputType input,
                       double worldTime) {
  if (input == IN_NONE)
    return;

  PushInput(&player->buffer, input, worldTime);
  int combo = CheckCombos(&player->buffer, worldTime);

  if (combo != -1 && player->comboCooldownTimer <= 0) {
    player->buffer.count = 0;
    player->currentFrame = 0;
    player->animTimer = 0;

    if (combo == COMBO_TRIPLE) {
      player->state = COMBO_TRIPLE;
      player->attackTimer = ATTACK_LIGHT_COMBO_TIME;
      player->damage = ATTACK_LIGHT_COMBO_DAMAGE;
      player->comboCooldownTimer = ATTACK_LIGHT_COMBO_DELAY;
    } else if (combo == COMBO_MIX) {
      player->state = COMBO_MIX;
      player->attackTimer = ATTACK_MIX_COMBO_TIME;
      player->damage = ATTACK_MIX_COMBO_DAMAGE;
      player->comboCooldownTimer = ATTACK_MIX_COMBO_DELAY;
    } else if (combo == DASH) {
      player->state = DASH;
      player->damage = ATTACK_DASH_COMBO_DAMAGE;
      player->attackTimer = ATTACK_DASH_COMBO_TIME;
      player->comboCooldownTimer = ATTACK_DASH_COMBO_DELAY;
    }
    ProcessAttack(player, enemies);
  } else if (player->attackTimer <= 0 && player->basicCooldownTimer <= 0) {
    if (input == IN_LIGHT) {
      player->currentFrame = 0;
      player->animTimer = 0;
      player->state = ATTACK_LIGHT;
      player->attackTimer = ATTACK_LIGHT_TIME;
      player->damage = ATTACK_LIGHT_DAMAGE;
      player->basicCooldownTimer = ATTACK_LIGHT_TIME_DELAY;
      ProcessAttack(player, enemies);
    } else if (input == IN_HEAVY) {
      player->currentFrame = 0;
      player->animTimer = 0;
      player->state = ATTACK_HEAVY;
      player->damage = ATTACK_HEAVY_DAMAGE;
      player->attackTimer = ATTACK_HEAVY_TIME;
      player->basicCooldownTimer = ATTACK_HEAVY_TIME_DELAY;
      ProcessAttack(player, enemies);
    }
  }
}

int HandleDeathLogic(PlayerType *player, double delta) {
  if (player->health <= 0 && player->state != DEAD) {
    player->health = 0;
    player->state = DEAD;
    player->currentFrame = 0;
    player->animTimer = 0;
    player->attackTimer = 0;
  }

  if (player->state == DEAD) {
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
    return 1;
  }
  return 0;
}

void HandlePhysics(PlayerType *player, const Uint8 *keyState, double delta) {
  if (player->state == DASH) {
    double dashSpeed = player->speed * DASH_SPEED_MULTIPLIER;
    if (player->direction == RIGHT)
      player->X += dashSpeed * delta;
    else if (player->direction == LEFT)
      player->X -= dashSpeed * delta;
    else if (player->direction == UP)
      player->Y -= dashSpeed * delta;
    else if (player->direction == DOWN)
      player->Y += dashSpeed * delta;
  } else if (player->attackTimer <= 0) {
    if (keyState[SDL_SCANCODE_A]) {
      player->X -= player->speed * delta;
      player->direction = LEFT;
      player->facingLeft = 1;
    }
    if (keyState[SDL_SCANCODE_D]) {
      player->X += player->speed * delta;
      player->direction = RIGHT;
      player->facingLeft = 0;
    }
  }

  if (player->onGround) {
    if (player->attackTimer <= 0) {
      if (keyState[SDL_SCANCODE_W]) {
        player->Y -= player->speed * delta;
        player->direction = UP;
      }
      if (keyState[SDL_SCANCODE_S]) {
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
}

void HandleTimersAndBounds(PlayerType *player, double delta) {
  if (player->attackTimer > 0)
    player->attackTimer -= delta;
  if (player->wasHitTimer > 0)
    player->wasHitTimer -= delta;
  if (player->basicCooldownTimer > 0)
    player->basicCooldownTimer -= delta;
  if (player->comboCooldownTimer > 0)
    player->comboCooldownTimer -= delta;

  if (player->multiplierTimer > 0) {
    if (player->wasHitTimer > 0)
      player->multiplierTimer = 0;
    else
      player->multiplierTimer -= delta;
  } else {
    player->multiplier = 1;
  }

  int widthBound = player->walk_frames_left[0]->w / 2;
  int heightBound = player->walk_frames_left[0]->h / 2;
  if (player->X < widthBound)
    player->X = widthBound;
  if (player->X > LEVEL_WIDTH - widthBound)
    player->X = LEVEL_WIDTH - widthBound;
  if (player->onGround) {
    if (player->Y < LEVEL_HEIGHT / 4 - 20)
      player->Y = LEVEL_HEIGHT / 4 - 20;
    if (player->Y > LEVEL_HEIGHT - heightBound)
      player->Y = LEVEL_HEIGHT - heightBound;
  }
}

void UpdatePlayer(PlayerType *player, EnemyType *enemies, double delta,
                  double worldTime) {
  if (HandleDeathLogic(player, delta))
    return;

  const Uint8 *keyState = SDL_GetKeyboardState(NULL);

  InputType input = ResolveInput(keyState, player->prev_keys);
  HandleCombatState(player, enemies, input, worldTime);
  HandleAnimationState(player, keyState, delta);
  HandlePhysics(player, keyState, delta);
  HandleTimersAndBounds(player, delta);
}
