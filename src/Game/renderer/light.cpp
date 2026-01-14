#include "light.h"
#include "game/game.h"

void Light::make_light(Game &game, Vec3 direction, f32 strength) {
    EntityId light_id = game.world.new_entity();
    auto *light = game.world.assign<Light>(light_id);
    game.light = light;
    light->direction = direction.normalized();
    light->strength = strength;
}
