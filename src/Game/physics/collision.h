#pragma once
#include "../helper/jtypes.h"
#include "../math/vectors.h"

struct AABB {
    f32 min_x;
    f32 max_x;
    f32 min_y;
    f32 max_y;
    f32 min_z;
    f32 max_z;

    static bool has_intersection(const AABB &a, const AABB &b) {
        return (
            a.min_x <= b.max_x &&
            a.max_x >= b.min_x &&
            a.min_y <= b.max_y &&
            a.max_y >= b.min_y &&
            a.min_z <= b.max_z &&
            a.max_z >= b.min_z
        );
    }

    static bool within_point(const AABB box, const Vec3 point) {
        return point.x >= box.min_x && point.x <= box.max_x &&
               point.y >= box.min_y && point.y <= box.max_y &&
               point.z >= box.min_z && point.z <= box.max_z;
    }

    static AABB make(
        f32 x,
        f32 y,
        f32 z,
        bool centered = true) {
        if (!centered) {
            return {0, x, 0, y, 0, z};
        }
        f32 mid_x = x / 2;
        f32 mid_y = y / 2;
        f32 mid_z = z / 2;
        return {-mid_x, mid_x, -mid_y, mid_y, -mid_z, mid_z};
    }

    Vec3 get_lengths() {
        return {max_x - min_x, max_y - min_y, max_z - min_z};
    }

    AABB translated(const Vec3 &translation) const {
        return {
            min_x + translation.x,
            max_x + translation.x,
            min_y + translation.y,
            max_y + translation.y,
            min_z + translation.z,
            max_z + translation.z,
        };
    }
};
