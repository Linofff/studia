#include "../headers/camera.h"

void UpdateCamera(CameraType *camera, PlayerType *player) {
  camera->X = player->X - SCREEN_WIDTH / 2;
  camera->Y = player->Y - SCREEN_HEIGHT / 2;

  if (camera->X < 0)
    camera->X = 0;
  if (camera->Y < 0)
    camera->Y = 0;
  if (camera->X > LEVEL_WIDTH - SCREEN_WIDTH)
    camera->X = LEVEL_WIDTH - SCREEN_WIDTH;
  if (camera->Y > LEVEL_HEIGHT - SCREEN_HEIGHT)
    camera->Y = LEVEL_HEIGHT - SCREEN_HEIGHT;
}
