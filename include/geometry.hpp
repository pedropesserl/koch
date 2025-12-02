#ifndef GEOMETRY_HPP_
#define GEOMETRY_HPP_

#include "raylib.h"

using Point = Vector2;

struct Triangle {
    Point p1, p2, p3;
};

Point rotate_point_around(Point point, Point center, float angle);

Triangle rotate_tri_around(Triangle tri, Point center, float angle);

bool check_collision_tri_rec(Triangle tri, Rectangle rec);

#endif // GEOMETRY_HPP_
