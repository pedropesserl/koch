#include <list>
#include <vector>
#include <cstring>
#include <iostream>
#include "raylib.h"
#include "raymath.h"

using namespace std;

struct Koch {
    float altura, angulo, comprimento;
    Vector2 centro;
    vector<Vector2> pontos;
};

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

Rectangle campo_de_visao(Camera2D *camera) {
    Vector2 posicao_zero = GetScreenToWorld2D(Vector2Zero(), *camera);
    Vector2 vetor_canto = { (float)GetScreenWidth(), (float)GetScreenHeight() };
    Vector2 posicao_canto = GetScreenToWorld2D(vetor_canto, *camera);
    return (Rectangle){
        .x = posicao_zero.x,
        .y = posicao_zero.y,
        .width = posicao_canto.x,
        .height = posicao_canto.y,
    };
}

Koch criar_koch(int n_iteracoes, bool invertido, float angulo = -PI / 3.0f, float comprimento = 800.0f, Vector2 centro = { GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f }) {
    Koch k = {
        .altura = (float)sqrt(3) / 2 * comprimento,
        .angulo = invertido ? -angulo : angulo,
        .comprimento = comprimento,
        .centro = centro,
        .pontos = {},
    };

    // Cria lista de pontos
    list<Vector2> lista_pontos = {{ -k.comprimento / 2.0f, -k.altura / 3.0f }, { k.comprimento / 2.0f, -k.altura / 3.0f }};
    for (Vector2 &ponto : lista_pontos) {
        ponto = Vector2Add(ponto, k.centro);
    }

    // Atualiza lista de pontos com base no número de iterações
    for (int i = 0; i < n_iteracoes; i++) {
        for (auto it = next(lista_pontos.begin()); it != lista_pontos.end(); it++) {
            // cada par de pontos vira 5 pontos
            Vector2 a = *prev(it);
            Vector2 b = *it;
            Vector2 diferenca = Vector2Subtract(b, a);
            Vector2 direcao = Vector2Normalize(diferenca);
            float modulo = Vector2Length(diferenca) / 3.0f;
            diferenca = Vector2Scale(direcao, modulo);
            Vector2 novo1 = Vector2Add(a, diferenca);
            Vector2 novo2 = Vector2Add(novo1, Vector2Rotate(diferenca, k.angulo));
            Vector2 novo3 = Vector2Add(novo1, diferenca);

            lista_pontos.insert(it, novo1);
            lista_pontos.insert(it, novo2);
            lista_pontos.insert(it, novo3);
        }
    }

    // Copia a lista de pontos 3 vezes para o vetor k.pontos
    for (Vector2 ponto : lista_pontos) {
        k.pontos.push_back(ponto);
    }
    for (Vector2 ponto : lista_pontos) {
        Vector2 ponto2 = Vector2Subtract(ponto, k.centro);
        ponto2 = Vector2Rotate(ponto2, 2 * PI / 3.0f);
        ponto2 = Vector2Add(ponto2, k.centro);
        k.pontos.push_back(ponto2);
    }
    for (Vector2 ponto : lista_pontos) {
        Vector2 ponto3 = Vector2Subtract(ponto, k.centro);
        ponto3 = Vector2Rotate(ponto3, -2 * PI / 3.0f);
        ponto3 = Vector2Add(ponto3, k.centro);
        k.pontos.push_back(ponto3);
    }

    return k;
}

size_t desenhar_koch(Koch &k, Rectangle area_visivel) {
    size_t segs = 0;
    for (size_t i = 0; i < k.pontos.size() - 1; i++) {
        DrawLineV(k.pontos[i], k.pontos[i + 1], BLACK);
        segs++;
    }
    return segs;
}

int main(int argc, char **argv) {
    bool invertido = false;
    if (argc == 2) {
        if (strcmp(argv[1], "-i") == 0) {
            invertido = true;
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

    Koch k_0 = criar_koch(6, invertido);
    Koch k_1 = criar_koch(7, invertido);
    Koch k_2 = criar_koch(8, invertido);

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
            segs = desenhar_koch(k_0, campo_de_visao(&camera));
        } else if (camera.zoom <= 8) {
            segs = desenhar_koch(k_1, campo_de_visao(&camera));
        } else if (camera.zoom <= 27) {
            segs = desenhar_koch(k_2, campo_de_visao(&camera));
        } else {
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
