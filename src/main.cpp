#include <iostream>
#include <list>
#include <cstring>
#include "raylib.h"
#include "raymath.h"

using namespace std;

static void translate_camera_on_m2(Camera2D *camera) {
    Vector2 delta = GetMouseDelta();
    delta = Vector2Scale(delta, -1.0f/camera->zoom);
    camera->target = Vector2Add(camera->target, delta);
}

static void zoom_camera_on_mouse_wheel(Camera2D *camera, float wheel) {
    Vector2 mouse_world_pos = GetScreenToWorld2D(GetMousePosition(), *camera);
    camera->offset = GetMousePosition();
    camera->target = mouse_world_pos;
    float scale = 0.2f * wheel;
    camera->zoom = max(expf(logf(camera->zoom) + scale), 0.125f);
}

int main(int argc, char **argv) {
    float angulo = -PI / 3.0f;
    if (argc == 2 && strcmp(argv[1], "-i") == 0) {
        angulo = -angulo;
    }

    InitWindow(950, 950, "koch");
    SetTargetFPS(60);

    Camera2D camera = {0};
    camera.zoom = 1.0f;

    float comprimento = 800;
    float altura = sqrt(3) / 2 * comprimento;
    Vector2 centro = { GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };

    list<Vector2> pontos1 = {{-comprimento / 2.0f, -altura / 3.0f}, {comprimento / 2.0f, -altura / 3.0f}};
    for (Vector2 &ponto : pontos1) {
        ponto = Vector2Add(ponto, centro);
    }

    while (!WindowShouldClose()) {
        list<Vector2> pontos2;
        for (Vector2 ponto : pontos1) {
            Vector2 ponto2 = Vector2Subtract(ponto, centro);
            ponto2 = Vector2Rotate(ponto2, 2 * PI / 3.0f);
            ponto2 = Vector2Add(ponto2, centro);
            pontos2.push_back(ponto2);
        }
        list<Vector2> pontos3;
        for (Vector2 ponto : pontos1) {
            Vector2 ponto3 = Vector2Subtract(ponto, centro);
            ponto3 = Vector2Rotate(ponto3, -2 * PI / 3.0f);
            ponto3 = Vector2Add(ponto3, centro);
            pontos3.push_back(ponto3);
        }

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            for (auto it = next(pontos1.begin()); it != pontos1.end(); it++) {
                // cada par de pontos vira 5 pontos
                Vector2 a = *prev(it);
                Vector2 b = *it;
                Vector2 diferenca = Vector2Subtract(b, a);
                Vector2 direcao = Vector2Normalize(diferenca);
                float modulo = Vector2Length(diferenca) / 3.0f;
                diferenca = Vector2Scale(direcao, modulo);
                Vector2 novo1 = Vector2Add(a, diferenca);
                Vector2 novo2 = Vector2Add(novo1, Vector2Rotate(diferenca, angulo));
                Vector2 novo3 = Vector2Add(novo1, diferenca);
                
                pontos1.insert(it, novo1);
                pontos1.insert(it, novo2);
                pontos1.insert(it, novo3);
            }
        }

        float wheel = GetMouseWheelMove();
        if (wheel != 0) {
            zoom_camera_on_mouse_wheel(&camera, wheel);
        }
        if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
            translate_camera_on_m2(&camera);
        }

        BeginDrawing();
        ClearBackground(WHITE);
        BeginMode2D(camera);
        for (auto it = pontos1.begin(); it != prev(pontos1.end()); it++) {
            DrawLineV(*it, *next(it), BLACK);
        }
        for (auto it = pontos2.begin(); it != prev(pontos2.end()); it++) {
            DrawLineV(*it, *next(it), BLACK);
        }
        for (auto it = pontos3.begin(); it != prev(pontos3.end()); it++) {
            DrawLineV(*it, *next(it), BLACK);
        }
        EndDrawing();
        EndMode2D();
    }
    CloseWindow();

    return 0;
}
