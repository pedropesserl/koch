#include <vector>
#include "geometry.hpp"
#include "raymath.h"

using namespace std;

Point rotate_point_around(Point point, Point center, float angle) {
    Point new_point = Vector2Subtract(point, center);
    new_point = Vector2Rotate(new_point, angle);
    new_point = Vector2Add(new_point, center);
    return new_point;
}

triangle_t rotate_tri_around(triangle_t tri, Point center, float angle) {
    return {
        rotate_point_around(tri.p1, center, angle),
        rotate_point_around(tri.p2, center, angle),
        rotate_point_around(tri.p3, center, angle),
    };
}

bool check_collision_tri_rec(triangle_t tri, rectangle_t rec) {
    vector<Point> tri_points = { tri.p1, tri.p2, tri.p3 };
    vector<Point> rec_points = { rec.p1, rec.p2, rec.p3, rec.p4 };
    for (Point p : tri_points) {
        if (CheckCollisionPointPoly(p, rec_points.data(), 4)) {
            return true;
        }
    }
    for (Point p : rec_points) {
        if (CheckCollisionPointTriangle(p, tri.p1, tri.p2, tri.p3)) {
            return true;
        }
    }
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 4; j++) {
            if (CheckCollisionLines(tri_points[i], tri_points[(i + 1) % 3],
                                    rec_points[j], rec_points[(j + 1) % 4], nullptr)) {
                return true;
            }
        }
    }
    return false;
}
