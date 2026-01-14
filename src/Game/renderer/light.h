#pragma once

#include "../math/vectors.h"

struct Game;

struct Light {
    Vec3 direction;
    f32 strength;

    static void make_light(Game &game, Vec3 direction, f32 strength);
};



