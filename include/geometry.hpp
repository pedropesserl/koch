#ifndef GEOMETRY_HPP_
#define GEOMETRY_HPP_

#include "raylib.h"

using Point = Vector2;

struct triangle_t {
    Point p1, p2, p3;
};

struct rectangle_t {
    Point p1, p2, p3, p4;
};

Point rotate_point_around(Point point, Point center, float angle);

triangle_t rotate_tri_around(triangle_t tri, Point center, float angle);

bool check_collision_tri_rec(triangle_t tri, rectangle_t rec);

#endif // GEOMETRY_HPP_
