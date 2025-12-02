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

Triangle rotate_tri_around(Triangle tri, Point center, float angle) {
    return (Triangle){
        .p1 = rotate_point_around(tri.p1, center, angle),
        .p2 = rotate_point_around(tri.p2, center, angle),
        .p3 = rotate_point_around(tri.p3, center, angle),
    };
}

bool check_collision_tri_rec(Triangle tri, Rectangle rec) {
    if (CheckCollisionPointRec(tri.p1, rec)
        || CheckCollisionPointRec(tri.p2, rec)
        || CheckCollisionPointRec(tri.p3, rec)) {
        return true;
    }
    vector<Point> tri_points = { tri.p1, tri.p2, tri.p3 };
    vector<Point> rec_points = {
        { rec.x, rec.y },
        { rec.x + rec.width, rec.y },
        { rec.x + rec.width, rec.y + rec.height },
        { rec.x, rec.y + rec.height },
    };
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
