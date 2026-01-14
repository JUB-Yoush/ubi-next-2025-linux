#include "player.h"

void Player::make_player(Game &game) {
    EntityId player_id = game.world.new_entity();
    game.player_id = player_id;
    auto *player = game.world.assign<Player>(player_id);
    auto *pmesh = game.world.assign<JMesh>(player_id);
    auto *ptransform = game.world.assign<JTransform>(player_id);
    auto *player_aabb = game.world.assign<AABB>(player_id);
    auto *player_vel = game.world.assign<Velocity>(player_id);

    // TODO why is everything upside down
    ptransform->translation.y += -3;

    *pmesh = make_rectangle_mesh(1, 1, 1);
    *player_aabb = AABB::make(1, 1, 1);

    pmesh->color = GREEN;
    game.playerref.transform = ptransform;
    game.playerref.mesh = pmesh;
    game.playerref.velocity = player_vel;
}
