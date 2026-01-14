#pragma once
#include "vectors.h"
#include <algorithm>
#include <map>
using namespace std;

void sort_points(Vec3 &p1, Vec3 &p2, Vec3 &p3) {
  // sort y values in decending order p3 > p2 > p1
  if (p1.y > p2.y) {
    swap(p1, p2);
  }

  if (p2.y > p3.y) {
    swap(p2, p3);
  }

  if (p1.y > p2.y) {
    swap(p1, p2);
  }
}

void sort_points_and_uvs(Vec3 &p1, Vec3 &p2, Vec3 &p3, Vec2 &uv1, Vec2 &uv2,
                         Vec2 &uv3) {
  if (p1.y > p2.y) {
    swap(p1, p2);
    swap(uv1, uv2);
  }

  if (p2.y > p3.y) {
    swap(p2, p3);
    swap(uv2, uv3);
  }

  if (p1.y > p2.y) {
    swap(p1, p2);
    swap(uv1, uv2);
  }
}

bool ccw_cmp(pair<f32, Vec2> &a, pair<f32, Vec2> &b) {
  return a.first < b.first;
}

// vector<Vec2> ccw_order(Vec2 p1, Vec2 p2, Vec2 p3) {
//   auto center = Vec2{(p1.x + p2.x + p3.x) / 3, (p1.y + p2.y + p3.y) / 3};
//   vector<pair<f32, Vec2> > angles{};
//   angles.emplace_back(atan2(p1.y - center.y, p1.x - center.x), p1);
//   angles.emplace_back(atan2(p2.y - center.y, p2.x - center.x), p2);
//   angles.emplace_back(atan2(p3.y - center.y, p3.x - center.x), p3);
//
//   sort(angles, ccw_cmp);
//   vector<Vec2> res;
//   res.reserve(3);
//
//   for (const auto &angle: angles) {
//     res.push_back(angle.second); // Get the original Vec2 point
//   }
//   return res;
// }
