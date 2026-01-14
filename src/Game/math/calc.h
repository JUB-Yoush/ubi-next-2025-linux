#pragma once
#include "../helper/jtypes.h"
#include "vectors.h"

namespace Calc {
    Vec3 barycentric_weights(Vec2 a, Vec2 b, Vec2 c, Vec2 p);

    f32 rand_float(f32 min, f32 max);
}
