#pragma once
#include "../game/game.h"
#include "../math/calc.h"
#include "app.h"
#include "jtypes.h"

void handle_inputs(Vec3 &translation, Vec3 &rotation, f32 &scale,
                   f32 delta_time) {
  f32 linear_step = 1;
  f32 angular_step = 48;

  if (App::IsKeyPressed(App::KEY_W))
    translation.z += linear_step;
  if (App::IsKeyPressed(App::KEY_S))
    translation.z -= linear_step;
  if (App::IsKeyPressed(App::KEY_A))
    translation.x += linear_step;
  if (App::IsKeyPressed(App::KEY_D))
    translation.x -= linear_step;
  if (App::IsKeyPressed(App::KEY_E))
    translation.y += linear_step;
  if (App::IsKeyPressed(App::KEY_Q))
    translation.y -= linear_step;

  if (App::IsKeyPressed(App::KEY_J))
    rotation.x -= angular_step;
  if (App::IsKeyPressed(App::KEY_L))
    rotation.x += angular_step;
  if (App::IsKeyPressed(App::KEY_O))
    rotation.y += angular_step;
  if (App::IsKeyPressed(App::KEY_U))
    rotation.y -= angular_step;
  if (App::IsKeyPressed(App::KEY_I))
    rotation.z += angular_step;
  if (App::IsKeyPressed(App::KEY_K))
    rotation.z -= angular_step;

  if (App::IsKeyPressed(App::KEY_N))
    scale += linear_step;
  if (App::IsKeyPressed(App::KEY_M))
    scale -= linear_step;
}

void update_render_mode(u8 &render_mode, u8 render_modes_count) {
  if (App::IsKeyPressed(App::KEY_LEFT)) {
    render_mode = (render_mode + render_modes_count - 1) % render_modes_count;
  } else if (App::IsKeyPressed(App::KEY_RIGHT)) {
    render_mode = (render_mode + 1) % render_modes_count;
  }
}
