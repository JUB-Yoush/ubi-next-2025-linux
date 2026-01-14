#pragma once
#include "ecs/world.h"

struct Scene {
    World world;

    virtual ~Scene() {
    }

    virtual void update() = 0;

    virtual void draw() = 0;

    virtual void entered() {
    };

    virtual void exited() {
    };
};
