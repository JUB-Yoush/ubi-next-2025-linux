#pragma once
#include <stack>

#include "../ecs/world.h"
#include "../renderer/camera.h"
#include "../renderer/light.h"
#include "renderer/mesh.h"
#include "scene.h"

// TODO make player struct bag of pointers to values in ECS
typedef Vec3 Velocity;

struct Mob {};

struct Floor {};

struct Game; // guh?

constexpr f32 MOB_SPAWN_TIME = 1.0;

// struct App {
//   stack<Scene *> scene_stack;
//   Scene &current_scene;
//   // add like application options n here n shii
// };

struct Game {
  enum RenderMode {
    WIREFRAME,
    WIREFRAME_CULLED,
    FLAT,
    SHADED,
  };

  struct PlayerRef {
    JTransform *transform;
    JMesh *mesh;
    Velocity *velocity;
  };

  enum ControlMode { CAMERA, PLAYER };

  u8 render_mode = 4;
  ControlMode control_mode = PLAYER;
  World world;
  JCamera *camera;
  Light *light;
  PlayerRef playerref;
  Vec2 input_dir{};
  EntityId player_id;
  EntityId floor_id;
  f32 mob_spawn_timer = MOB_SPAWN_TIME;

  static void make_floor(Game &game, Vec3 floor_dimensions);
};
