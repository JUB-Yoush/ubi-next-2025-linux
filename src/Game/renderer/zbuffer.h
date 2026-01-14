#pragma once
#include "../helper/consts.h"

struct ZBuffer {
  f32 buff[SCREEN_WIDTH * SCREEN_HEIGHT];
};

void ClearZBuffer(ZBuffer &zbuff) {
  for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
    zbuff.buff[i] = 999999;
  }
}
