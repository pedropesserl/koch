#ifndef KOCH_HPP_
#define KOCH_HPP_

#include <list>
#include <vector>
#include "geometry.hpp"

using namespace std;

// TODO: bounding triangles not very helpful on inverted
#define KOCH_ANGLE (-PI / 3.0f)

struct Koch_point {
    Point point;
    int iteration;
};

struct Koch {
    bool is_inverted;
    float height, width;
    Point center;
    vector<Koch_point> points;
    vector<triangle_t> bounding_triangles;
};

void translate_camera_on_m2(Camera2D *camera);

void zoom_in_camera_on_m1(Camera2D *camera);

void zoom_camera_on_mouse_wheel(Camera2D *camera, float wheel);

rectangle_t get_viewport(Camera2D *camera);

void increase_detail(list<Point> &list, Koch &k, int iteration);

void fill_vector_and_triangles(Koch &k, list<Point> &list, int num_triangles);

Koch create_koch(int iterations, bool is_inverted, int num_subdivisions, float width = 800.0f, Point center = { GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f });

size_t draw_koch(Koch &k, rectangle_t visible_area);

#endif // KOCH_HPP_
