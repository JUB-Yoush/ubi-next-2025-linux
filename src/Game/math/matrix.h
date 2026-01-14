#pragma once
#include "../helper/jtypes.h"
#include "../math/vectors.h"

struct Matrix4x4 {
    f32 mat[4][4];

    Matrix4x4();

    Matrix4x4(const f32 val[4][4]);

    f32 *operator[](i32 i);

    const f32 *operator[](i32 i) const;

    Matrix4x4 operator*(const Matrix4x4 &b) const;

    Vec3 operator*(const Vec3 &vec) const;

    Vec4 operator*(const Vec4 &vec) const;
};

Matrix4x4 make_translation_matrix(f32 x, f32 y, f32 z);

Matrix4x4 make_scale_matrix(f32 sx, f32 sy, f32 sz);

Matrix4x4 make_rotation_matrix(f32 pitch, f32 yaw, f32 roll);

Matrix4x4 make_view_matrix(Vec3 eye, Vec3 target);

Matrix4x4 make_projection_matrix(f32 fov, i32 screenHeight, i32 screenWidth,
                                 f32 near, f32 far);

// TODO make
Matrix4x4 rotate_matrix(const Matrix4x4 &mat, const Vec3 &euler);
