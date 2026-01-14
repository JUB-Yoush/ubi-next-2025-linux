#include "game.h"

void Game::make_floor(Game &game, Vec3 floor_dimensions) {
    EntityId floor = game.world.new_entity();
    auto *floor_mesh = game.world.assign<JMesh>(floor);
    game.world.assign<Floor>(floor);
    auto *floor_trans = game.world.assign<JTransform>(floor);
    auto *floor_aabb = game.world.assign<AABB>(floor);
    auto [x,y,z] = floor_dimensions;
    *floor_aabb = AABB::make(x, y, z);
    *floor_mesh = make_aabb_mesh(*floor_aabb);
    game.floor_id = floor;
}
