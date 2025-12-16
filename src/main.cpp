#include <cmath>
#include <cstring>
#include <iostream>
#include "geometry.hpp"
#include "koch.hpp"

using namespace std;

inline float log3(float x) {
    return log(x) / log(3);
}

int main(int argc, char **argv) {
    bool is_inverted = false;
    if (argc == 2) {
        if (strcmp(argv[1], "-i") == 0) {
            is_inverted = true;
        } else if (strcmp(argv[1], "-h") == 0) {
            cout << "Uso:\n";
            cout << "    " << argv[0] << "    Mostra o fractal normalmente\n";
            cout << "    " << argv[0] << " -i Mostra o fractal invertido\n";
            cout << "    " << argv[0] << " -h Imprime essa mensagem de ajuda\n";
            return 0;
        }
    }

    InitWindow(1800, 950, "koch");
    SetTargetFPS(30);

    Camera2D camera = {
        .offset = (Vector2){ GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f },
        .target = (Vector2){ GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f },
        .rotation = 0.0f,
        .zoom = 1.0f,
    };

    vector<Koch> pre_renders;
    int base_iteration_count = 6;
    for (int i = 0; i < 6; i++) {
        pre_renders.push_back(create_koch(i + base_iteration_count, is_inverted, i));
    }

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_SPACE)) {
            camera = {0};
            camera.zoom = 1.0f;
        }
        if (IsKeyPressed(KEY_DOWN)) {
            camera.zoom /= 3.0f;
        }
        if (IsKeyPressed(KEY_UP)) {
            camera.zoom *= 3.0f;
        }
        if (IsKeyPressed(KEY_LEFT)) {
            camera.rotation--;
        }
        if (IsKeyPressed(KEY_RIGHT)) {
            camera.rotation++;
        }
        if (IsKeyPressed(KEY_MINUS)) {
            camera.zoom *= 0.99f;
        }
        if (IsKeyPressed(KEY_EQUAL)) {
            camera.zoom *= 1.01f;
        }
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) { 
            zoom_in_camera_on_m1(&camera);
        }
        if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
            translate_camera_on_m2(&camera);
        }
        zoom_camera_on_mouse_wheel(&camera, GetMouseWheelMove());

        BeginDrawing();
        ClearBackground(WHITE);
        BeginMode2D(camera);

        rectangle_t vp = get_viewport(&camera);
        float zoom_limit_factor = 2.5f;
        int render_index = pre_renders.size() - 1;
        if (camera.zoom < zoom_limit_factor * pow(3, render_index - 1)) {
            if (camera.zoom <= 1) {
                render_index = 0;
            } else {
                render_index = floor(log3(camera.zoom / zoom_limit_factor)) + 1;
            }
        }
        size_t segs = draw_koch(pre_renders[render_index], vp);

        EndMode2D();

        DrawText(TextFormat("zoom = %f", camera.zoom), 10, 10, 50, RED);
        DrawText(TextFormat("segments = %zu", segs), 10, 70, 50, RED);
        DrawText(TextFormat("rotation = %f", camera.rotation), 10, 130, 50, RED);

        EndDrawing();
    }
    CloseWindow();

    return 0;
}
