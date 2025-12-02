#ifndef KOCH_HPP_
#define KOCH_HPP_

#include <list>
#include <vector>
#include "geometry.hpp"

using namespace std;

struct Koch {
    float height, angle, width;
    Point center;
    vector<Point> points;
    vector<Triangle> bounding_triangles;
};

void translate_camera_on_m2(Camera2D *camera);

void zoom_camera_on_m1(Camera2D *camera);

Rectangle field_of_view(Camera2D *camera);

void increase_detail(list<Point> &list, Koch &k);

void fill_vector_and_triangles(Koch &k, list<Point> &list);

Koch create_koch(int iterations, bool inverted, float angle = -PI / 3.0f, float width = 800.0f, Point center = { GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f });

size_t draw_koch(Koch &k, Rectangle visible_area);

#endif // KOCH_HPP_
