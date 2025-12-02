#include <cstring>
#include <iostream>
#include "geometry.hpp"
#include "koch.hpp"

using namespace std;

int main(int argc, char **argv) {
    bool inverted = false;
    if (argc == 2) {
        if (strcmp(argv[1], "-i") == 0) {
            inverted = true;
        } else if (strcmp(argv[1], "-h") == 0) {
            cout << "Uso:\n";
            cout << "    " << argv[0] << "    Mostra o fractal normalmente\n";
            cout << "    " << argv[0] << " -i Mostra o fractal invertido\n";
            cout << "    " << argv[0] << " -h Imprime essa mensagem de ajuda\n";
            return 0;
        }
    }

    InitWindow(1800, 950, "koch");
    SetTargetFPS(60);

    Camera2D camera = {0};
    camera.zoom = 1.0f;

    Koch k_0 = create_koch(6, inverted);
    Koch k_1 = create_koch(7, inverted);
    Koch k_2 = create_koch(8, inverted);

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_SPACE)) {
            camera = {0};
            camera.zoom = 1.0f;
        }
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) { 
            zoom_camera_on_m1(&camera);
        }
        if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
            translate_camera_on_m2(&camera);
        }

        BeginDrawing();
        ClearBackground(WHITE);
        BeginMode2D(camera);

        size_t segs;
        if (camera.zoom <= 2.5) {
            segs = draw_koch(k_0, field_of_view(&camera));
        } else if (camera.zoom <= 8) {
            segs = draw_koch(k_1, field_of_view(&camera));
        } else if (camera.zoom <= 27) {
            segs = draw_koch(k_2, field_of_view(&camera));
        } else {
            segs = draw_koch(k_2, field_of_view(&camera));
            camera.zoom = 9.0f;
        }

        EndDrawing();
        EndMode2D();

        DrawText(TextFormat("ZOOM = %f", camera.zoom), 10, 10, 50, RED);
        DrawText(TextFormat("SEGS = %zu", segs), 10, 70, 50, RED);

    }
    CloseWindow();

    return 0;
}
