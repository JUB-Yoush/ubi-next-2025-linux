#pragma once
#include "helper/jtypes.h"

struct Color {
  u8 r;
  u8 g;
  u8 b;
  u8 a = 255;
};

constexpr Color RED = Color{255, 0, 0};
constexpr Color GREEN = Color{0, 255, 0};
constexpr Color BLUE = Color{0, 0, 255};
constexpr Color WHITE = Color{255, 255, 255};
constexpr Color BLACK = Color{0, 0, 0};