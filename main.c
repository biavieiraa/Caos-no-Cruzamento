#include "raylib.h"

int main(void)
{
    const int largura = 1000;
    const int altura = 700;

    InitWindow(largura, altura, "Caos no Cruzamento");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();

        // Fundo
        ClearBackground((Color){65, 135, 75, 255});

        // Ruas
        DrawRectangle(0, 270, largura, 160, DARKGRAY);
        DrawRectangle(420, 0, 160, altura, DARKGRAY);

        // Faixas da rua horizontal
        for (int x = 0; x < largura; x += 60)
        {
            DrawRectangle(x, 347, 30, 6, YELLOW);
        }

        // Faixas da rua vertical
        for (int y = 0; y < altura; y += 60)
        {
            DrawRectangle(497, y, 6, 30, YELLOW);
        }

        // Faixas de pedestres
        for (int i = 0; i < 6; i++)
        {
            DrawRectangle(435 + i * 22, 250, 12, 12, RAYWHITE);
            DrawRectangle(435 + i * 22, 438, 12, 12, RAYWHITE);
            DrawRectangle(400, 285 + i * 22, 12, 12, RAYWHITE);
            DrawRectangle(588, 285 + i * 22, 12, 12, RAYWHITE);
        }

        DrawText("CAOS NO CRUZAMENTO", 20, 20, 30, RAYWHITE);
        DrawText("Controle o transito!", 20, 60, 20, RAYWHITE);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}