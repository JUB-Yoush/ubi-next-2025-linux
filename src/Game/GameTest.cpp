///////////////////////////////////////////////////////////////////////////////
// Filename: GameTest.cpp
// Provides a demo of how to use the API
///////////////////////////////////////////////////////////////////////////////

#if BUILD_PLATFORM_WINDOWS
//------------------------------------------------------------------------
#include <windows.h>
#endif

#include <iostream>
//------------------------------------------------------------------------
#include <math.h>
//------------------------------------------------------------------------
#include "../ContestAPI/app.h"
//------------------------------------------------------------------------

#include "ecs/query.h"
#include "ecs/world.h"
#include "helper/jinput.h"
#include "math/calc.h"
#include "math/matrix.h"
#include "renderer/camera.h"
#include "renderer/mesh.h"
#include "renderer/renderer.h"
#include "renderer/zbuffer.h"
#include <iostream>
#include <optional>

#include "game/game.h"
#include "game/player.h"
#include "helper/jinput.h"
#include "physics/collision.h"
//------------------------------------------------------------------------
// Example data....
//------------------------------------------------------------------------
CSimpleSprite *testSprite;
enum {
  ANIM_FORWARDS,
  ANIM_BACKWARDS,
  ANIM_LEFT,
  ANIM_RIGHT,
};
Game game;
void update_models(Game &game, f32 delta) {
  for (EntityId ent : Query<JMesh, JTransform, AABB>(game.world)) {
    auto &translation = game.world.get<JTransform>(ent)->translation;
    auto &rotation = game.world.get<JTransform>(ent)->rotation;
    auto &scale = game.world.get<JTransform>(ent)->scale;
    auto *mesh = game.world.get<JMesh>(ent);

    Matrix4x4 translation_matrix =
        make_translation_matrix(translation.x, translation.y, translation.z);

    Matrix4x4 rotation_matrix =
        make_rotation_matrix(rotation.x, rotation.y, rotation.z);

    Matrix4x4 scale_matrix = make_scale_matrix(scale, scale, scale);

    auto model_matrix = translation_matrix * (rotation_matrix * scale_matrix);

    auto view_model_matrix = game.camera->view_matrix * model_matrix;

    apply_transformations(mesh->transformed_vertices, mesh->vertices,
                          view_model_matrix);
  }
}

void update_control_mode(Game &game) {
  if (App::IsKeyPressed(App::KEY_1)) {
    game.control_mode = Game::PLAYER;
  }
  if (App::IsKeyPressed(App::KEY_2)) {
    game.control_mode = Game::CAMERA;
  }
}

void poll_inputs(Game &game, float delta) {
  switch (game.control_mode) {
  case Game::CAMERA: {
    f32 mi_bombo = 1;
    handle_inputs(game.camera->position, game.camera->rotation, mi_bombo,
                  delta);
    break;
  }

  case Game::PLAYER: {
    game.input_dir = Vec2::zero();
    if (App::IsKeyPressed(App::KEY_W))
      game.input_dir.y = -1;
    if (App::IsKeyPressed(App::KEY_S))
      game.input_dir.y = 1;
    if (App::IsKeyPressed(App::KEY_A))
      game.input_dir.x = -1;
    if (App::IsKeyPressed(App::KEY_D))
      game.input_dir.x = 1;
    game.input_dir = game.input_dir.normalized();
    break;
  }
  }
}

void update_camera(Game &game) {
  game.camera->view_matrix =
      make_view_matrix(game.camera->position, game.camera->target);
  game.camera->target = {game.camera->position.x, game.camera->position.y, 0};
}

void spawn_mob(Game &game) {
  EntityId mob_id = game.world.new_entity();
  game.world.assign<Mob>(mob_id);
  auto *mesh = game.world.assign<JMesh>(mob_id);
  auto *aabb = game.world.assign<AABB>(mob_id);
  auto *transform = game.world.assign<JTransform>(mob_id);
  auto *velocity = game.world.assign<Velocity>(mob_id);
  *aabb = AABB::make(.7, .5, .7);
  *mesh = make_aabb_mesh(*aabb);
  transform->translation = {Calc::rand_float(-5, 5), 0,
                            Calc::rand_float(-5, 5)};
  mesh->color = RED;
  transform->translation = {Calc::rand_float(-5, 5), 0,
                            Calc::rand_float(-5, 5)};
  *velocity = {Calc::rand_float(-5, 5), 0, Calc::rand_float(-5, 5)};
}

void update_timer(Game &game, const f32 delta) {
  game.mob_spawn_timer = max(game.mob_spawn_timer - delta, 0.0f);
  if (game.mob_spawn_timer == 0) {
    game.mob_spawn_timer = MOB_SPAWN_TIME;
    spawn_mob(game);
  }
}

// returns 3 optional values that list if a collision happened in that axis and
// what the current veloctiy was
array<pair<f32, bool>, 3> test_collision(Game &game, EntityId moving,
                                         EntityId other, f32 delta) {
  auto *transform = game.world.get<JTransform>(moving);
  auto *velocity = game.world.get<Velocity>(moving);
  auto *aabb = game.world.get<AABB>(moving);

  auto *other_transform = game.world.get<JTransform>(other);
  auto *other_aabb = game.world.get<AABB>(other);

  array<pair<f32, bool>, 3> out = {{{0, false}, {0, false}, {0, false}}};

  // test x
  transform->translation.x += velocity->x * delta;
  if (AABB::has_intersection(
          other_aabb->translated(other_transform->translation),
          aabb->translated(transform->translation))) {
    // modifying the transform should be a seperate step afterwards
    out[0].first = velocity->x;
    out[0].second = true;
  }
  transform->translation.x -= velocity->x * delta;

  // test y
  transform->translation.y += velocity->y * delta;
  if (AABB::has_intersection(
          other_aabb->translated(other_transform->translation),
          aabb->translated(transform->translation))) {
    out[1].first = velocity->y;
    out[1].second = true;
  }
  transform->translation.y -= velocity->y * delta;

  // test z
  transform->translation.z += velocity->z * delta;
  if (AABB::has_intersection(
          other_aabb->translated(other_transform->translation),
          aabb->translated(transform->translation))) {
    out[2].first = velocity->z;
    out[2].second = true;
  }
  transform->translation.z -= velocity->z * delta;

  return out;
}

bool player_touching_ground(Game &game, f32 delta) {
  return test_collision(game, game.player_id, game.floor_id, delta)[1].second;
}

void move_things(Game &game, f32 delta) {
  // things without collisions
  for (const EntityId ent : Query<Velocity, JTransform>(game.world)) {
    if (game.world.has<AABB>(ent)) {
      continue;
    }
    auto *transform = game.world.get<JTransform>(ent);
    auto *velocity = game.world.get<Velocity>(ent);
    transform->translation += *velocity * delta;
  }

  // player checking for floor:
  auto *ptransform = game.world.get<JTransform>(game.player_id);
  auto *pvelocity = game.world.get<Velocity>(game.player_id);
  auto *paabb = game.world.get<AABB>(game.player_id);

  auto *transform = game.world.get<JTransform>(game.floor_id);
  auto *aabb = game.world.get<AABB>(game.floor_id);
  array<pair<f32, bool>, 3> collision_result =
      test_collision(game, game.player_id, game.floor_id, delta);
  // std::cout << collision_result[1].has_value() << std::endl;

  if (collision_result[0].second) {
    pvelocity->x = 0;
  }
  // no ceilings i guess
  if (collision_result[1].second) {
    pvelocity->y = std::min(pvelocity->y, 0.0f);
  }
  if (collision_result[2].second) {
    pvelocity->z = 0;
  }

  for (const EntityId ent : Query<Velocity, JTransform, AABB>(game.world)) {
    auto *transform = game.world.get<JTransform>(ent);
    auto *velocity = game.world.get<Velocity>(ent);
    auto *aabb = game.world.get<AABB>(ent);
    transform->translation += *velocity * delta;
  }
}

void update_player(Game &game, f32 delta) {
  f32 speed = 5;
  f32 gravity = .1;
  if (game.input_dir != Vec2::zero()) {
    *game.playerref.velocity =
        Velocity{game.input_dir.x * speed, game.playerref.velocity->y,
                 game.input_dir.y * speed};
  } else {
    *game.playerref.velocity = Velocity{0, game.playerref.velocity->y, 0};
  }

  bool is_on_floor = player_touching_ground(game, delta);
  if (!is_on_floor) {
    game.playerref.velocity->y += gravity;
  }

  is_on_floor = player_touching_ground(game, delta);
  if (App::IsKeyPressed(App::KEY_SPACE) && is_on_floor) {
    game.playerref.velocity->y -= 5;
  }
  // check stompin
  for (const EntityId ent : Query<Mob>(game.world)) {
    auto collsions = test_collision(game, game.player_id, ent, delta);
    // the player perpetually has a
    if (collsions[1].second != false && collsions[1].first > 1) {
      game.world.destroy_entity(ent);
      game.playerref.velocity->y += -8;
    }
  }
}
//------------------------------------------------------------------------

//------------------------------------------------------------------------
// Called before first update. Do any initial setup here.
//------------------------------------------------------------------------
//
void Init() {

  JCamera::make_camera(game, {0, -2, 8}, {0, 0, 1});
  Light::make_light(game, {0, -1, 0}, 1);

  // setup player
  Player::make_player(game);
  Game::make_floor(game, {10, .5, 10});
}

//------------------------------------------------------------------------
// Update your simulation here. deltaTime is the elapsed time since the last
// update in ms. This will be called at no greater frequency than the value of
// APP_MAX_FRAME_RATE
//------------------------------------------------------------------------
void Update(const float delta) {
  update_render_mode(game.render_mode, 5);
  update_control_mode(game);
  poll_inputs(game, delta);
  update_player(game, delta);
  move_things(game, delta);
  update_camera(game);
  update_models(game, delta);
  update_timer(game, delta);
}

void draw_floor(Game &game) {
  for (EntityId ent : Query<Floor>(game.world)) {
    auto *mesh = game.world.get<JMesh>(ent);
    draw_mesh(*mesh, game.camera->projection_matrix, *game.light);
  }
}

void draw_models(Game &game) {
  vector<EntityId> entities_to_draw{};
  JCamera *cam = game.camera;
  // just store the ids, get the components in the comparison function
  for (EntityId ent : Query<JMesh, JTransform>(game.world)) {
    if (game.world.has<Floor>(ent)) {
      continue;
    }
    entities_to_draw.emplace_back(ent);
  }

  /* z depth based mesh sorting
   * I'd need to implment some polygon cliping related algorithm that seperates
   * overlapping parts into seperate polygons that are drawn seperatley.
   * something like
   * https://www.geeksforgeeks.org/dsa/polygon-clipping-sutherland-hodgman-algorithm/
   * for now just not being able to change the y of the camera and always
   * drawing the floor last seems like a fine solution
   */
  sort(entities_to_draw.begin(), entities_to_draw.end(),
       [&cam, &game](EntityId a, EntityId b) {
         // sort decending, furthest elements are drawn first
         auto *a_trans = game.world.get<JTransform>(a);
         auto *b_trans = game.world.get<JTransform>(b);
         f32 a_dist = (a_trans->translation - cam->position).length_squared();
         f32 b_dist = (b_trans->translation - cam->position).length_squared();
         return a_dist > b_dist;
       });

  for (const EntityId id : entities_to_draw) {
    if (game.world.has<Floor>(id)) {
      continue;
    }

    // if (game.world.has<Player>(id)) {
    //   DrawText(game.world.get<JTransform>(id)->translation.to_string(), 0,
    //   30, 10, WHITE);
    // }

    auto *mesh = game.world.get<JMesh>(id);
    draw_mesh(*mesh, game.camera->projection_matrix, *game.light);
  }
};

//------------------------------------------------------------------------
// Add your display calls here (DrawLine,Print, DrawSprite.)
// See App.h
//------------------------------------------------------------------------
void Render() {
  App::DrawLine(10, 10, 50, 100);
  App::DrawTriangle(0, 0, 5, 10, 10, 5);
  draw_floor(game);
  draw_models(game);
}
//------------------------------------------------------------------------
// Add your shutdown code here. Called when the APP_QUIT_KEY is pressed.
// Just before the app exits.
//------------------------------------------------------------------------
void Shutdown() {
  //------------------------------------------------------------------------
  // Example Sprite Code....
  delete testSprite;
  //------------------------------------------------------------------------
}