#include "raylib.h"

#define MAX_CARROS 8

#define CIMA 0
#define BAIXO 1
#define ESQUERDA 2
#define DIREITA 3

#define SEMAFORO_VERMELHO 0
#define SEMAFORO_VERDE 1

typedef struct {
    float x;
    float y;
    int direcao;
    Color cor;
} Carro;

typedef struct {
    Rectangle bounds; 
    int estado;       
} Semaforo;

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

// Função para desenhar os semáforos na tela
void desenharSemaforos(Semaforo semaforos[], int quantidade)
{
    for (int i = 0; i < quantidade; i++)
    {
        // Caixa do semáforo (fundo preto)
        DrawRectangleRec(semaforos[i].bounds, BLACK);
        DrawRectangleLinesEx(semaforos[i].bounds, 2, GRAY);

        // Posição central x da caixa
        int centroX = semaforos[i].bounds.x + semaforos[i].bounds.width / 2;

        // Luz Vermelha (topo)
        Color corVermelha = (semaforos[i].estado == SEMAFORO_VERMELHO) ? RED : DARKGRAY;
        DrawCircle(centroX, semaforos[i].bounds.y + 12, 7, corVermelha);

        // Luz Verde (base)
        Color corVerde = (semaforos[i].estado == SEMAFORO_VERDE) ? LIME : DARKGRAY;
        DrawCircle(centroX, semaforos[i].bounds.y + 32, 7, corVerde);
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

    // Inicialização dos 4 semáforos posicionados antes de cada entrada do cruzamento
    Semaforo semaforos[4] = {
        { { 385, 215, 24, 44 }, SEMAFORO_VERMELHO }, // Semáforo para quem vem de CIMA
        { { 590, 440, 24, 44 }, SEMAFORO_VERMELHO }, // Semáforo para quem vem de BAIXO
        { { 385, 440, 24, 44 }, SEMAFORO_VERDE },    // Semáforo para quem vem da ESQUERDA
        { { 590, 215, 24, 44 }, SEMAFORO_VERDE }     // Semáforo para quem vem da DIREITA
    };

    InitWindow(largura, altura, "Caos no Cruzamento");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        // Interação com o semáforo
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            Vector2 mousePos = GetMousePosition();
            for (int i = 0; i < 4; i++)
            {
                if (CheckCollisionPointRec(mousePos, semaforos[i].bounds))
                {
                    semaforos[i].estado = (semaforos[i].estado == SEMAFORO_VERDE) ? SEMAFORO_VERMELHO : SEMAFORO_VERDE;
                }
            }
        }

        // --- DESENHO ---
        BeginDrawing();

        // Fundo grama
        ClearBackground((Color){65, 135, 75, 255});

        // Ruas
        DrawRectangle(0, 270, largura, 160, DARKGRAY);
        DrawRectangle(420, 0, 160, altura, DARKGRAY);

        // Ciclovia
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

        // Desenhar os semáforos
        desenharSemaforos(semaforos, 4);

        // Textos na tela
        DrawText("CAOS NO CRUZAMENTO", 20, 20, 30, RAYWHITE);
        DrawText("Clique nos semaforos para alterar o sinal!", 20, 60, 20, RAYWHITE);

        // Desenhar os carros
        desenharCarros(carros, quantidadeCarros);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
