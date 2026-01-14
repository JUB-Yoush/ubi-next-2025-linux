#pragma once
#include <map>

#include "../helper/color.h"
#include "../helper/jtypes.h"
#include "physics/collision.h"

struct Triangle {
  i32 points[9];

  // Overload the subscript operator
  i32 &operator[](u64 index);

  // const version for read-only access
  const i32 &operator[](u64 index) const;
};

enum DrawMode {
  WIRE,
  WIRE_CULLED,
  FLAT,
  SHADED,
};

struct JMesh {
  // TODO seperate immutable values to be a pointer instead of making a new copy
  // per mesh
  vector<Vec3> transformed_vertices;
  vector<Vec3> transformed_normals;
  vector<Vec3> vertices;      // can be shared
  vector<Vec3> normals;       // can be shared
  vector<Vec2> uvs;           // can be shared
  vector<Triangle> triangles; // can be shared
  Color color = WHITE;
  DrawMode draw_mode = SHADED;
};

// immutable mesh data that can be shared across instances
struct MeshData {
  vector<Vec3> vertices;      // can be shared
  vector<Vec3> normals;       // can be shared
  vector<Vec2> uvs;           // can be shared
  vector<Triangle> triangles; // can be shared
};

bool z_compare(Vec3 v1, Vec3 v2);

vector<Vec3> sorted_z_order(vector<Vec3> vec);

JMesh make_rectangle_mesh(f32 x_len, f32 y_len, f32 z_len);

JMesh make_cube();

JMesh make_aabb_mesh(AABB aabb);

Triangle make_triangle_from_obj_points(f32 points[10]);

MeshData load_mesh_from_file(const char *filename);

class MeshManager {
public:
  static MeshManager &instance() {
    static auto *instance = new MeshManager();
    return *instance;
  }

  static map<string, MeshData> mesh_data_map;

  static MeshData get_mesh_data(string name) {
    if (mesh_data_map.find(name) == mesh_data_map.end()) {
      mesh_data_map[name] = load_mesh_from_file(name.c_str());
    }
    return mesh_data_map[name];
  }

private:
  MeshManager() = default;
};

JMesh make_mesh_instance(const MeshData &mesh_data);

struct JTransform {
  Vec3 translation{};
  Vec3 rotation{};
  f32 scale = 1.0f;
};
