#include <cassert>
#include "koch.hpp"
#include "raymath.h"

void translate_camera_on_m2(Camera2D *camera) {
    Vector2 delta = GetMouseDelta();
    delta = Vector2Scale(delta, -1.0f/camera->zoom);
    camera->target = Vector2Add(camera->target, delta);
}

void zoom_camera_on_m1(Camera2D *camera) {
    Vector2 mouse_world_pos = GetScreenToWorld2D(GetMousePosition(), *camera);
    camera->offset = GetMousePosition();
    camera->target = mouse_world_pos;
    float scale = 0.01f;
    camera->zoom = max(expf(logf(camera->zoom) + scale), 0.125f);
}

Rectangle field_of_view(Camera2D *camera) {
    Vector2 zero = GetScreenToWorld2D(Vector2Zero(), *camera);
    Vector2 vector_corner = { (float)GetScreenWidth(), (float)GetScreenHeight() };
    Vector2 corner = GetScreenToWorld2D(vector_corner, *camera);
    return (Rectangle){
        .x = zero.x,
        .y = zero.y,
        .width = corner.x - zero.x,
        .height = corner.y - zero.y,
    };
}

void increase_detail(list<Point> &list, Koch &k) {
    for (auto it = next(list.begin()); it != list.end(); it++) {
        // cada par de pontos vira 5 pontos
        Point a = *prev(it);
        Point b = *it;
        Vector2 difference = Vector2Subtract(b, a);
        Vector2 direction = Vector2Normalize(difference);
        float magnitude = Vector2Length(difference) / 3.0f;
        difference = Vector2Scale(direction, magnitude);
        Point new1 = Vector2Add(a, difference);
        Point new2 = Vector2Add(new1, Vector2Rotate(difference, k.angle));
        Point new3 = Vector2Add(new1, difference);
        list.insert(it, new1);
        list.insert(it, new2);
        list.insert(it, new3);
    }
}

void fill_vector_and_triangles(Koch &k, list<Point> &list) {
    assert(k.bounding_triangles.empty());
    assert(k.points.empty());

    auto it = list.begin();
    Triangle tri1, tri2;
    tri1.p1 = *it;
    for (size_t i = 0; i < list.size() / 4; i++) {
        k.points.push_back(*it++);
    }
    tri1.p2 = *it;
    for (size_t i = list.size() / 4; i < list.size() / 2; i++) {
        k.points.push_back(*it++);
    }
    tri1.p3 = *it;
    k.bounding_triangles.push_back(tri1);
    tri2.p1 = *it;
    for (size_t i = list.size() / 2; i < 3 * list.size() / 4; i++) {
        k.points.push_back(*it++);
    }
    tri2.p2 = *it;
    for (size_t i = 3 * list.size() / 4; i < list.size(); i++) {
        k.points.push_back(*it++);
    }
    tri2.p3 = *prev(it);
    k.bounding_triangles.push_back(tri2);

    for (Point point : list) {
        k.points.push_back(rotate_point_around(point, k.center, 2 * PI / 3.0f));
    }
    k.bounding_triangles.push_back(rotate_tri_around(tri1, k.center, 2 * PI / 3.0f));
    k.bounding_triangles.push_back(rotate_tri_around(tri2, k.center, 2 * PI / 3.0f));

    for (Point point : list) {
        k.points.push_back(rotate_point_around(point, k.center, -2 * PI / 3.0f));
    }
    k.bounding_triangles.push_back(rotate_tri_around(tri1, k.center, -2 * PI / 3.0f));
    k.bounding_triangles.push_back(rotate_tri_around(tri2, k.center, -2 * PI / 3.0f));
}

Koch create_koch(int iterations, bool inverted, float angle, float width, Point center) {
    Koch k = {
        .height = (float)sqrt(3) / 2 * width,
        .angle = inverted ? -angle : angle,
        .width = width,
        .center = center,
        .points = {},
        .bounding_triangles = {},
    };
    list<Point> points_list = {{ -k.width / 2.0f, -k.height / 3.0f },
                                { k.width / 2.0f, -k.height / 3.0f }};
    for (Point &point : points_list) {
        point = Vector2Add(point, k.center);
    }
    for (int i = 0; i < iterations; i++) {
        increase_detail(points_list, k);
    }
    fill_vector_and_triangles(k, points_list);
    return k;
}

size_t draw_koch(Koch &k, Rectangle visible_area) {
    size_t segs = 0;
    for (Triangle tri : k.bounding_triangles) {
        DrawTriangleLines(tri.p1, tri.p2, tri.p3, BLACK);
    }
    for (int i = 0; i < 6; i++) {
        if (check_collision_tri_rec(k.bounding_triangles[i], visible_area)) {
            for (size_t j = i * k.points.size() / 6; j < (i + 1) * k.points.size() / 6 - 1; j++) {
                DrawLineV(k.points[j], k.points[j + 1], BLACK);
                segs++;
            }
        }
    }
    return segs;
}
