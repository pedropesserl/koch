#include <cassert>
#include <cmath>
#include "koch.hpp"
#include "raymath.h"

using namespace std;

void translate_camera_on_m2(Camera2D *camera) {
    Vector2 delta = GetMouseDelta();
    delta = Vector2Scale(delta, -1.0f/camera->zoom);
    camera->target = Vector2Add(camera->target, delta);
}

static void zoom_camera_to_mouse(Camera2D *camera, float scale) {
    Vector2 mouse_world_pos = GetScreenToWorld2D(GetMousePosition(), *camera);
    camera->offset = GetMousePosition();
    camera->target = mouse_world_pos;
    camera->zoom = max(expf(logf(camera->zoom) + scale), 0.125f);
}

void zoom_in_camera_on_m1(Camera2D *camera) {
    zoom_camera_to_mouse(camera, 0.01f);
}

void zoom_camera_on_mouse_wheel(Camera2D *camera, float wheel) {
    if (wheel != 0) {
        zoom_camera_to_mouse(camera, wheel * 0.2f);
    }
}

rectangle_t get_viewport(Camera2D *camera) {
    return {
        GetScreenToWorld2D({0.0f, 0.0f}, *camera),
        GetScreenToWorld2D({(float)GetScreenWidth(), 0.0f}, *camera),
        GetScreenToWorld2D({(float)GetScreenWidth(), (float)GetScreenHeight()}, *camera),
        GetScreenToWorld2D({0.0f, (float)GetScreenHeight()}, *camera),
    };
}

void increase_detail(list<Koch_point> &list, Koch &k, int iteration) {
    float angle = k.is_inverted ? -KOCH_ANGLE : KOCH_ANGLE;
    for (auto it = next(list.begin()); it != list.end(); it++) {
        // cada par de pontos vira 5 pontos
        Point a = (*prev(it)).point;
        Point b = (*it).point;
        Vector2 difference = Vector2Subtract(b, a);
        Vector2 direction = Vector2Normalize(difference);
        float magnitude = Vector2Length(difference) / 3.0f;
        difference = Vector2Scale(direction, magnitude);
        Point new1 = Vector2Add(a, difference);
        Point new2 = Vector2Add(new1, Vector2Rotate(difference, angle));
        Point new3 = Vector2Add(new1, difference);
        list.insert(it, {new1, iteration});
        list.insert(it, {new2, iteration});
        list.insert(it, {new3, iteration});
    }
}

void fill_vector_and_triangles(Koch &k, list<Koch_point> &list, int num_subdivisions) {
    assert(k.bounding_triangles.empty());
    assert(k.points.empty());

    int num_triangles_in_list = pow(2, num_subdivisions);
    int num_stops = num_triangles_in_list * 2;
    auto it = list.begin();
    vector<triangle_t> tris;
    triangle_t t;
    for (int i = 0; i < num_triangles_in_list; i++) {
        t.p1 = it->point;
        for (size_t j = 2 * i * list.size() / num_stops; j < (2 * i + 1) * list.size() / num_stops; j++) {
            k.points.push_back(*it++);
        }
        t.p2 = it->point;
        for (size_t j = (2 * i + 1) * list.size() / num_stops; j < (2 * i + 2) * list.size() / num_stops; j++) {
            k.points.push_back(*it++);
        }
        t.p3 = i != num_triangles_in_list - 1 ? it->point : prev(it)->point;
        tris.push_back(t);
    }
    k.bounding_triangles = tris;

    vector<float> copy_angles = {2 * PI / 3.0f, -2 * PI / 3.0f};
    for (float angle : copy_angles) {
        for (Koch_point point : list) {
            k.points.push_back({rotate_point_around(point.point, k.center, angle), point.iteration});
        }
        for (triangle_t tri : tris) {
            k.bounding_triangles.push_back(rotate_tri_around(tri, k.center, angle));
        }
    }
}

Koch create_koch(int iterations, bool is_inverted, int num_bounding_triangles, float width, Point center) {
    Koch k = {
        .is_inverted = is_inverted,
        .height = (float)sqrt(3) / 2 * width,
        .width = width,
        .center = center,
        .points = {},
        .bounding_triangles = {},
    };
    list<Koch_point> points_list = {{{ -k.width / 2.0f, -k.height / 3.0f }, 0},
                                     {{ k.width / 2.0f, -k.height / 3.0f }, 0}};
    for (Koch_point &point : points_list) {
        point.point = Vector2Add(point.point, k.center);
    }
    for (int i = 0; i < iterations; i++) {
        increase_detail(points_list, k, i);
    }
    fill_vector_and_triangles(k, points_list, num_bounding_triangles);
    return k;
}

size_t draw_koch(Koch &k, rectangle_t visible_area) {
    size_t segs = 0;
    /* for (triangle_t tri : k.bounding_triangles) { */
    /*     DrawTriangleLines(tri.p1, tri.p2, tri.p3, RED); */
    /* } */
    for (size_t i = 0; i < k.bounding_triangles.size(); i++) {
        if (check_collision_tri_rec(k.bounding_triangles[i], visible_area)) {
            size_t num_segments = k.points.size() / k.bounding_triangles.size();
            for (size_t j = 0; j < num_segments - 1; j++) {
                Point p1 = k.points[i * num_segments + j].point;
                Point p2 = k.points[i * num_segments + j + 1].point;
                DrawLineV(p1, p2, BLACK);
                segs++;
            }
            int rem = k.points.size() % k.bounding_triangles.size();
            for (int j = 0; j < rem - 1; j++) {
                Point p1 = k.points[(i + 1) * num_segments + j].point;
                Point p2 = k.points[(i + 1) * num_segments + j + 1].point;
                DrawLineV(p1, p2, BLACK);
                segs++;
            }
        }
    }
    return segs;
}
