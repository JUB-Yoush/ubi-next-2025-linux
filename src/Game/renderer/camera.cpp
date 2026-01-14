#include "camera.h"
#include "game/game.h"

void JCamera::make_camera(Game &game, Vec3 positon, Vec3 target) {
    EntityId camera_id = game.world.new_entity();
    auto *cam = game.world.assign<JCamera>(camera_id);
    game.camera = cam;
    cam->position = positon;
    cam->target = target;
    cam->projection_matrix = make_projection_matrix(
        FOV, SCREEN_HEIGHT, SCREEN_WIDTH, NEAR_PLANE, FAR_PLANE);
}
