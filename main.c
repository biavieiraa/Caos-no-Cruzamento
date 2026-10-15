#include "raylib.h"

#define MAX_CARROS 8

#define CIMA 0
#define BAIXO 1
#define ESQUERDA 2
#define DIREITA 3

typedef struct {
    float x;
    float y;
    int direcao;
    Color cor;
} Carro;

void desenharCarros(Carro carros[], int quantidade)
{
    for (int i = 0; i < quantidade; i++)
    {
        if (carros[i].direcao == CIMA ||
            carros[i].direcao == BAIXO)
        {
            DrawRectangle(
                (int)carros[i].x,
                (int)carros[i].y,
                26, 38,
                carros[i].cor
            );
        }
        else
        {
            DrawRectangle(
                (int)carros[i].x,
                (int)carros[i].y,
                38, 26,
                carros[i].cor
            );
        }
    }
}

int main(void)
{
    const int largura = 1000;
    const int altura = 700;
    Carro carros[MAX_CARROS] = {
    {455, 100, CIMA, RED},
    {515, 520, BAIXO, BLUE},
    {100, 290, ESQUERDA, PINK},
    {800, 370, DIREITA, ORANGE}
};

int quantidadeCarros = 4;

    InitWindow(largura, altura, "Caos no Cruzamento");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        // Fundo grama

        ClearBackground((Color){65, 135, 75, 255});

        

        // Ruas
        DrawRectangle(0, 270, largura, 160, DARKGRAY);
        DrawRectangle(420, 0, 160, altura, DARKGRAY);

        // Ciclovia (faixa vermelha na borda superior da rua horizontal)
        DrawRectangle(0, 270, 420, 20, RED);
        DrawRectangle(580, 270, 420, 20, RED);

        // Sinalização tracejada da ciclovia
        for (int x = 0; x < 420; x += 30)
        {
            DrawRectangle(x, 279, 15, 2, RAYWHITE);
        }
        for (int x = 580; x < largura; x += 30)
        {
            DrawRectangle(x, 279, 15, 2, RAYWHITE);
        }

        // Faixas centrais da rua horizontal
        for (int x = 0; x < largura; x += 60)
        {
            DrawRectangle(x, 347, 30, 6, YELLOW);
        }

        // Faixas centrais da rua vertical
        for (int y = 0; y < altura; y += 60)
        {
            DrawRectangle(497, y, 6, 30, YELLOW);
        }

        // Faixas de pedestres
        for (int i = 0; i < 6; i++)
        {
            DrawRectangle(435 + i * 22, 274, 12, 12, RAYWHITE);
            DrawRectangle(435 + i * 22, 438, 12, 12, RAYWHITE);
            DrawRectangle(400, 295 + i * 22, 12, 12, RAYWHITE);
            DrawRectangle(588, 295 + i * 22, 12, 12, RAYWHITE);
        }

        // Textos na tela
        DrawText("CAOS NO CRUZAMENTO", 20, 20, 30, RAYWHITE);
        DrawText("Controle o transito!", 20, 60, 20, RAYWHITE);

desenharCarros(carros, quantidadeCarros);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}