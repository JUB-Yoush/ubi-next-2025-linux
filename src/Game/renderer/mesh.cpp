#include "../math/calc.h"
#include <bits/types/FILE.h>
#include <cassert>
#include <cmath>
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <fmt/xchar.h>
#include <vector>

#include "physics/collision.h"

#include "mesh.h"

// typedef Vec3 Translation;
// typedef Vec3 Rotation;

// Overload the subscript operator
i32 &Triangle::operator[](u64 index) {
  if (index < 9) {
    return points[index];
  }
  throw out_of_range("Index out of range");
}

// const version for read-only access
const i32 &Triangle::operator[](u64 index) const {
  if (index < 9) {
    return points[index];
  }
  throw out_of_range("Index out of range");
}

bool z_compare(Vec3 v1, Vec3 v2) { return v1.z > v2.z; }

vector<Vec3> sorted_z_order(vector<Vec3> vec) {
  sort(vec.begin(), vec.end(), z_compare);
  return vec;
}

JMesh make_rectangle_mesh(f32 x_len, f32 y_len, f32 z_len) {
  const f32 x = x_len / 2;
  const f32 y = y_len / 2;
  const f32 z = z_len / 2;
  vector<Vec3> transformed_vertices;
  transformed_vertices.resize(8);

  vector<Vec3> transformed_normals;
  transformed_normals.resize(6);

  vector<Vec3> vertices;
  vertices.resize(8);
  vertices[0] = Vec3{-x, -y, -z};
  vertices[1] = Vec3{-x, y, -z};
  vertices[2] = Vec3{x, y, -z};
  vertices[3] = Vec3{x, -y, -z};
  vertices[4] = Vec3{x, y, z};
  vertices[5] = Vec3{x, -y, z};
  vertices[6] = Vec3{-x, y, z};
  vertices[7] = Vec3{-x, -y, z};

  vector<Vec3> normals;
  normals.resize(6);
  normals[0] = {0.0, 0.0, -1.0};
  normals[1] = {1.0, 0.0, 0.0};
  normals[2] = {0.0, 0.0, 1.0};
  normals[3] = {-1.0, 0.0, 0.0};
  normals[4] = {0.0, 1.0, 0.0};
  normals[5] = {0.0, -1.0, 0.0};

  vector<Vec2> uvs;
  uvs.resize(4);
  uvs[0] = Vec2{1.0, 1.0};
  uvs[1] = Vec2{1.0, 0.0};
  uvs[2] = Vec2{0.0, 0.0};
  uvs[3] = Vec2{0.0, 1.0};

  vector<Triangle> triangles;
  triangles.resize(12);

  // Front                 vert.     uvs       norm.
  triangles[0] = Triangle{0, 1, 2, 0, 1, 2, 0, 0, 0};
  triangles[1] = Triangle{0, 2, 3, 0, 2, 3, 0, 0, 0};
  // Right
  triangles[2] = Triangle{3, 2, 4, 0, 1, 2, 1, 1, 1};
  triangles[3] = Triangle{3, 4, 5, 0, 2, 3, 1, 1, 1};
  // Back
  triangles[4] = Triangle{5, 4, 6, 0, 1, 2, 2, 2, 2};
  triangles[5] = Triangle{5, 6, 7, 0, 2, 3, 2, 2, 2};
  // Left
  triangles[6] = Triangle{7, 6, 1, 0, 1, 2, 3, 3, 3};
  triangles[7] = Triangle{7, 1, 0, 0, 2, 3, 3, 3, 3};
  // Top
  triangles[8] = Triangle{1, 6, 4, 0, 1, 2, 4, 4, 4};
  triangles[9] = Triangle{1, 4, 2, 0, 2, 3, 4, 4, 4};
  // Bottom
  triangles[10] = Triangle{5, 7, 0, 0, 1, 2, 5, 5, 5};
  triangles[11] = Triangle{5, 0, 3, 0, 2, 3, 5, 5, 5};
  return JMesh{
      .transformed_vertices = transformed_vertices,
      .transformed_normals = transformed_normals,
      .vertices = vertices,
      .normals = normals,
      .uvs = uvs,
      .triangles = triangles,
  };
}

JMesh make_cube() { return make_rectangle_mesh(1, 1, 1); }

JMesh make_aabb_mesh(AABB aabb) {
  auto [x, y, z] = aabb.get_lengths();
  return make_rectangle_mesh(x, y, z);
}

Triangle make_triangle_from_obj_points(f32 points[10]) {
  Triangle tri;
  for (int i = 1; i <= 10; ++i) {
    // obj files are 1 indexed
    tri.points[i - 1] = points[i] - 1;
  }
  return tri;
}

MeshData load_mesh_from_file(const char *filename) {
  /*
  parses out into

  vertices: vec of Vector3 (v)
  normals: vec of Vector3 (vn)
  triangles: vec of Triangle (f)
  uvs: vec of Vector2 (vt)
  Thanks Ramy for the inspiration
  https://github.com/ramyzhang/ubi-next-2024-2025/blob/main/RuanGame/source/engine/graphics/Primitives.cpp#L135
  */
  vector<Vec3> vertices;
  vector<Vec3> normals;
  vector<Triangle> triangles;
  vector<Vec2> uvs;

  FILE *file = fopen(filename, "r");
  assert(file != nullptr);

  bool ended = false;
  while (!ended) {
    char head[128];

    if (fscanf(file, "%s", head) == EOF) {
      ended = true;
    }

    if (strcmp(head, "#") == 0) {
      continue;
    } else if (strcmp(head, "v") == 0) {
      Vec3 new_vert = {0, 0, 0};
      i32 res =
          fscanf(file, "%f %f %f\n", &new_vert.x, &new_vert.y, &new_vert.z);
      if (res != 3 || res == EOF) {
        ended = true;
        break;
      }
      vertices.push_back(new_vert);
    } else if (strncmp(head, "vn", 2) == 0) {
      Vec3 normal = {0, 0, 0};
      i32 res = fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);

      if (res != 3 || res == EOF) {
        ended = true;
        break;
      }
      normals.push_back(normal);
    } else if (strncmp(head, "vt", 2) == 0) {
      Vec2 uv = {0, 0};
      i32 res = fscanf(file, "%f %f\n", &uv.x, &uv.y);
      if (res != 2 || res == EOF) {
        ended = true;
        break;
      }
      uvs.push_back(uv);
    } else if (strcmp(head, "f") == 0) {
      f32 points[10];
      // vert/uv/normal vert/uv/normal vert/uv/normal
      i32 res = fscanf(file, "%f/%f/%f %f/%f/%f %f/%f/%f\n", &points[1],
                       &points[4], &points[7], &points[2], &points[5],
                       &points[8], &points[3], &points[6], &points[9]);
      if (res != 9 || res == EOF) {
        ended = true;
        break;
      }
      Triangle triangle = make_triangle_from_obj_points(points);
      triangles.push_back(triangle);
    }
  }
  fclose(file);
  return MeshData{.vertices = vertices,
                  .normals = normals,
                  .uvs = uvs,
                  .triangles = triangles};
}

JMesh make_mesh_instance(const MeshData &mesh_data) {
  return JMesh{.transformed_vertices = mesh_data.vertices,
               .transformed_normals = mesh_data.normals,
               .vertices = mesh_data.vertices,
               .normals = mesh_data.normals,
               .uvs = mesh_data.uvs,
               .triangles = mesh_data.triangles};
}
