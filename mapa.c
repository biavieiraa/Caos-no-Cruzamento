
#include "raylib.h"
#include <stdbool.h>

#define CIMA 0
#define BAIXO 1

#define TAM_CELULA 50
#define MAPA_LARGURA 20
#define MAPA_ALTURA 14

typedef struct {
    float x;
    float y;
    int direcao;
    Color cor;
    int estado;
    bool parado;
} Carro;

static int matrizMapa[MAPA_ALTURA][MAPA_LARGURA];


void limparMatriz(void)
{
    for (int l = 0; l < MAPA_ALTURA; l++)
    {
        for (int c = 0; c < MAPA_LARGURA; c++)
        {
            matrizMapa[l][c] = 0;
        }
    }
}

void atualizarMatrizComCarros(Carro carros[], int quantidade)
{
    limparMatriz();

    for (int i = 0; i < quantidade; i++)
    {
        int larguraCarro = 38;
        int alturaCarro = 26;

        if (carros[i].direcao == CIMA || carros[i].direcao == BAIXO)
        {
            larguraCarro = 26;
            alturaCarro = 38;
        }

        int colInicial = (int)(carros[i].x / TAM_CELULA);
        int linInicial = (int)(carros[i].y / TAM_CELULA);

        int colFinal = (int)(
            (carros[i].x + larguraCarro - 1) / TAM_CELULA
        );

        int linFinal = (int)(
            (carros[i].y + alturaCarro - 1) / TAM_CELULA
        );

        for (int l = linInicial; l <= linFinal; l++)
        {
            for (int c = colInicial; c <= colFinal; c++)
            {
                if (
                    l >= 0 &&
                    l < MAPA_ALTURA &&
                    c >= 0 &&
                    c < MAPA_LARGURA
                )
                {
                    matrizMapa[l][c] = 1;
                }
            }
        }
    }
}


void desenharMapa(bool fluxoVerticalVerde, int largura, int altura)
{

    DrawRectangle(0, 270, largura, 160, DARKGRAY);
    DrawRectangle(420, 0, 160, altura, DARKGRAY);


    DrawRectangle(0, 270, 420, 20, RED);
    DrawRectangle(580, 270, 420, 20, RED);

    for (int x = 0; x < 420; x += 30)
    {
        DrawRectangle(x, 279, 15, 2, RAYWHITE);
    }

    for (int x = 580; x < largura; x += 30)
    {
        DrawRectangle(x, 279, 15, 2, RAYWHITE);
    }


    for (int x = 0; x < largura; x += 60)
    {
        DrawRectangle(x, 347, 30, 6, YELLOW);
    }

    for (int y = 0; y < altura; y += 60)
    {
        DrawRectangle(497, y, 6, 30, YELLOW);
    }

    for (int i = 0; i < 6; i++)
    {
        DrawRectangle(435 + i * 22, 274, 12, 12, RAYWHITE);
        DrawRectangle(435 + i * 22, 438, 12, 12, RAYWHITE);
        DrawRectangle(400, 295 + i * 22, 12, 12, RAYWHITE);
        DrawRectangle(588, 295 + i * 22, 12, 12, RAYWHITE);
    }


    DrawText("SEMAFORO HORIZONTAL", 335, 155, 16, RAYWHITE);

    DrawRectangle(390, 190, 30, 55, BLACK);

    if (fluxoVerticalVerde)
    {
        DrawCircle(405, 205, 8, GREEN);
        DrawCircle(405, 230, 8, RED);
    }
    else
    {
        DrawCircle(405, 205, 8, RED);
        DrawCircle(405, 230, 8, GREEN);
    }
}
