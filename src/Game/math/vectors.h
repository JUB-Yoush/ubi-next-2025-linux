#pragma once
#include "../helper/consts.h"
#include "../helper/jtypes.h"
#include <cmath>
#include <iostream>
#include <math.h>
#include <vector>

using namespace std;

// TODO implement operator overloading
struct Vec2 {
  f32 x;
  f32 y;

  Vec2 operator+(const Vec2 &v2) { return Vec2{x + v2.x, y + v2.y}; }
  Vec2 operator-(const Vec2 &v2) { return Vec2{x - v2.x, y - v2.y}; }
  Vec2 operator*(const Vec2 &v2) { return Vec2{x * v2.x, y * v2.y}; }
  Vec2 operator*(const f32 &scalar) { return Vec2{x * scalar, y * scalar}; }
  Vec2 operator*(const i32 &scalar) { return Vec2{x * scalar, y * scalar}; }
  bool operator==(const Vec2 &v2) { return x == v2.x && y == v2.y; };
  bool operator!=(const Vec2 &v2) { return x != v2.x || y != v2.y; };
  // bool operator<(const Vec2 &v2) { return x < v2.x && y < v2.y; };
  // bool operator>(const Vec2 &v2) { return x > v2.x && y > v2.y; };
  static Vec2 zero() { return {0, 0}; }

  Vec2 normalized() {
    f32 len = std::sqrt(x * x + y * y);
    if (len == 0.0) {
      return {0, 0};
    }

    return {
        x / len,
        y / len,
    };
  }
};

struct Vec3 {
  f32 x;
  f32 y;
  f32 z;

  Vec3 operator+(const Vec3 &v2) { return Vec3{x + v2.x, y + v2.y, z + v2.z}; }

  Vec3 &operator+=(const Vec3 &v2) {
    this->x += v2.x;
    this->y += v2.y;
    this->z += v2.z;
    return *this;
  }

  Vec3 operator-(const Vec3 &v2) { return Vec3{x - v2.x, y - v2.y, z - v2.z}; }

  Vec3 operator*(const Vec3 &v2) { return Vec3{x * v2.x, y * v2.y, z * v2.z}; }

  Vec3 operator*(const f32 &scalar) {
    return Vec3{x * scalar, y * scalar, z * scalar};
  }

  Vec3 normalized() {
    f32 len = std::sqrt(x * x + y * y + z * z);
    if (len == 0.0) {
      return {0, 0, 0};
    }

    return {
        x / len,
        y / len,
        z / len,
    };
  }

  f32 length() const { return sqrtf(x * x + y * y + z * z); }

  f32 length_squared() const { return (x * x + y * y + z * z); }

  Vec3 cross(Vec3 v2) {
    return Vec3{
        y * v2.z - z * v2.y,
        z * v2.x - x * v2.z,
        x * v2.y - y * v2.x,
    };
  }

  void floor_xy() {
    x = floor(x);
    y = floor(y);
  }

  f32 dot(Vec3 v2) { return x * v2.x + y * v2.y + z * v2.z; }
};

struct Vec4 {
  f32 x;
  f32 y;
  f32 z;
  f32 w;
};
