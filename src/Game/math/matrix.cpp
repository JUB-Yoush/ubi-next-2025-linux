#include "matrix.h"

Matrix4x4::Matrix4x4()
    : mat{{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}} {};

Matrix4x4::Matrix4x4(const f32 val[4][4]) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      mat[i][j] = val[i][j];
    };
  };
}

f32 *Matrix4x4::operator[](i32 i) { return mat[i]; }

const f32 *Matrix4x4::operator[](i32 i) const { return mat[i]; }

Matrix4x4 Matrix4x4::operator*(const Matrix4x4 &b) const {
  Matrix4x4 result;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      result.mat[i][j] = mat[i][0] * b.mat[0][j] + mat[i][1] * b.mat[1][j] +
                         mat[i][2] * b.mat[2][j] + mat[i][3] * b.mat[3][j];
    };
  };
  return result;
}

Vec3 Matrix4x4::operator*(const Vec3 &vec) const {
  return Vec3{
      .x =
          mat[0][0] * vec.x + mat[0][1] * vec.y + mat[0][2] * vec.z + mat[0][3],
      .y =
          mat[1][0] * vec.x + mat[1][1] * vec.y + mat[1][2] * vec.z + mat[1][3],
      .z =
          mat[2][0] * vec.x + mat[2][1] * vec.y + mat[2][2] * vec.z + mat[2][3],
  };
}

Vec4 Matrix4x4::operator*(const Vec4 &vec) const {
  return Vec4{
      .x = mat[0][0] * vec.x + mat[0][1] * vec.y + mat[0][2] * vec.z +
           mat[0][3] * vec.w,
      .y = mat[1][0] * vec.x + mat[1][1] * vec.y + mat[1][2] * vec.z +
           mat[1][3] * vec.w,
      .z = mat[2][0] * vec.x + mat[2][1] * vec.y + mat[2][2] * vec.z +
           mat[2][3] * vec.w,
      .w = mat[3][0] * vec.x + mat[3][1] * vec.y + mat[3][2] * vec.z +
           mat[3][3] * vec.w,
  };
}

Matrix4x4 make_translation_matrix(f32 x, f32 y, f32 z) {
  f32 values[4][4] = {{1, 0, 0, x}, {0, 1, 0, y}, {0, 0, 1, z}, {0, 0, 0, 1}};
  return {values};
}

Matrix4x4 make_scale_matrix(f32 sx, f32 sy, f32 sz) {
  f32 values[4][4] = {
      {sx, 0, 0, 0}, {0, sy, 0, 0}, {0, 0, sz, 0}, {0, 0, 0, 1}};
  return {values};
}

Matrix4x4 make_rotation_matrix(f32 pitch, f32 yaw, f32 roll) {
  f32 alpha = yaw * DEG_TO_RAD;
  f32 beta = pitch * DEG_TO_RAD;
  f32 gamma = roll * DEG_TO_RAD;

  f32 ca = cos(alpha);
  f32 sa = sin(alpha);

  f32 cb = cos(beta);
  f32 sb = sin(beta);

  f32 cg = cos(gamma);
  f32 sg = sin(gamma);

  f32 values[4][4] = {
      {ca * cb, ca * sb * sg - sa * cg, ca * sb * cg + sa * sg, 0.0},
      {sa * cb, sa * sb * sg + ca * cg, sa * sb * cg - ca * sg, 0.0},
      {-sb, cb * sg, cb * cg, 0.0},
      {0.0, 0.0, 0.0, 1.0}};
  return Matrix4x4(values);
};

Matrix4x4 make_view_matrix(Vec3 eye, Vec3 target) {
  Vec3 forward = (eye - target).normalized();
  Vec3 right = Vec3{0, 1, 0}.cross(forward);
  Vec3 up = forward.cross(right);
  f32 values[4][4] = {{right.x, right.y, right.z, -right.dot(eye)},
                      {up.x, up.y, up.z, -up.dot(eye)},
                      {forward.x, forward.y, forward.z, -forward.dot(eye)},
                      {0, 0, 0, 1}};
  return Matrix4x4(values);
};

Matrix4x4 make_projection_matrix(f32 fov, i32 screenHeight, i32 screenWidth,
                                 f32 near, f32 far) {
  f32 f = 1.0 / tan(fov * 0.5 * DEG_TO_RAD);
  f32 aspect = f32(screenWidth) / f32(screenHeight);

  f32 values[4][4] = {{f / aspect, 0, 0, 0},
                      {0, f, 0, 0},
                      {0, 0, -far / (far - near), -1},
                      {0, 0, -far * near / (far - near), 0}};
  return Matrix4x4(values);
}

// TODO make
Matrix4x4 rotate_matrix(const Matrix4x4 &mat, const Vec3 &euler) {
  Matrix4x4 rotated;

  rotated[0][0] = cos(euler.y) * cos(euler.z);
  rotated[0][1] = cos(euler.y) * cos(euler.z);
  rotated[0][2] = cos(euler.y) * cos(euler.z);
  return rotated;
}
