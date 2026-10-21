#include "raylib.h"

#define MAX_CARROS 8

#define CIMA 0
#define BAIXO 1
#define ESQUERDA 2
#define DIREITA 3
#define APROXIMANDO 0
#define ATRAVESSANDO 1
#define SAINDO 2

// --- CONFIGURAÇÕES DA MATRIZ DO MAPA ---
#define TAM_CELULA 50
#define MAPA_LARGURA 20 // 1000px / 50px = 20 colunas
#define MAPA_ALTURA 14  // 700px / 50px = 14 linhas

typedef struct {
    float x;
    float y;
    int direcao;
    Color cor;
    int estado;
} Carro;

// Matriz do mapa (0 = Livre, 1 = Ocupado por Carro)
int matrizMapa[MAPA_ALTURA][MAPA_LARGURA];

// Reseta a matriz zerando todas as posições
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

// Mapeia a posição (x, y) de cada carro na matriz
void atualizarMatrizComCarros(Carro carros[], int quantidade)
{
    limparMatriz();

    for (int i = 0; i < quantidade; i++)
    {
        int col = (int)(carros[i].x / TAM_CELULA);
        int lin = (int)(carros[i].y / TAM_CELULA);

        if (col >= 0 && col < MAPA_LARGURA && lin >= 0 && lin < MAPA_ALTURA)
        {
            matrizMapa[lin][col] = 1;
        }
    }
}

// Desenha a grade visual da matriz no cenário
void desenharGradeMatriz(bool mostrarGrade)
{
    if (!mostrarGrade) return;

    for (int l = 0; l < MAPA_ALTURA; l++)
    {
        for (int c = 0; c < MAPA_LARGURA; c++)
        {
            DrawRectangleLines(c * TAM_CELULA, l * TAM_CELULA, TAM_CELULA, TAM_CELULA, Fade(GRAY, 0.2f));

            if (matrizMapa[l][c] == 1)
            {
                DrawRectangle(c * TAM_CELULA, l * TAM_CELULA, TAM_CELULA, TAM_CELULA, Fade(RED, 0.3f));
            }
        }
    }
}

void desenharCarros(Carro carros[], int quantidade)
{
    for (int i = 0; i < quantidade; i++)
    {
        if (carros[i].direcao == CIMA || carros[i].direcao == BAIXO)
        {
            DrawRectangle((int)carros[i].x, (int)carros[i].y, 26, 38, carros[i].cor);
        }
        else
        {
            DrawRectangle((int)carros[i].x, (int)carros[i].y, 38, 26, carros[i].cor);
        }
    }
}

void atualizarCarros(
    Carro carros[],
    int quantidade,
    int largura,
    int altura,
    int fluxoVerticalVerde
)
{
    float velocidade = 180.0f * GetFrameTime();

    for (int i = 0; i < quantidade; i++)
    {
        int carroVertical = (carros[i].direcao == CIMA || carros[i].direcao == BAIXO);
        int sinalVerde = (carroVertical == fluxoVerticalVerde);

        switch (carros[i].direcao)
        {
            case CIMA:
            {
                float limite = 236.0f;

                if (carros[i].estado != APROXIMANDO || sinalVerde)
                {
                    carros[i].y += velocidade;
                }
                else if (carros[i].y + 38 < limite)
                {
                    carros[i].y += velocidade;

                    if (carros[i].y + 38 > limite)
                        carros[i].y = limite - 38;
                }

                if (carros[i].estado == APROXIMANDO && sinalVerde && carros[i].y + 38 >= limite)
                {
                    carros[i].estado = ATRAVESSANDO;
                }

                if (carros[i].estado == ATRAVESSANDO && carros[i].y >= 430)
                {
                    carros[i].estado = SAINDO;
                }

                if (carros[i].y > altura + 50)
                {
                    carros[i].y = -50;
                    carros[i].estado = APROXIMANDO;
                }

                break;
            }

            case BAIXO:
            {
                float limite = 450.0f;

                if (carros[i].estado != APROXIMANDO || sinalVerde)
                {
                    carros[i].y -= velocidade;
                }
                else if (carros[i].y > limite)
                {
                    carros[i].y -= velocidade;

                    if (carros[i].y < limite)
                        carros[i].y = limite;
                }

                if (carros[i].estado == APROXIMANDO && sinalVerde && carros[i].y <= limite)
                {
                    carros[i].estado = ATRAVESSANDO;
                }

                if (carros[i].estado == ATRAVESSANDO && carros[i].y + 38 < 270)
                {
                    carros[i].estado = SAINDO;
                }

                if (carros[i].y < -50)
                {
                    carros[i].y = altura + 50;
                    carros[i].estado = APROXIMANDO;
                }

                break;
            }

            case ESQUERDA:
            {
                float limite = 407.0f;

                if (carros[i].estado != APROXIMANDO || sinalVerde)
                {
                    carros[i].x += velocidade;
                }
                else if (carros[i].x + 38 < limite)
                {
                    carros[i].x += velocidade;

                    if (carros[i].x + 38 > limite)
                        carros[i].x = limite - 38;
                }

                if (carros[i].estado == APROXIMANDO && sinalVerde && carros[i].x + 38 >= limite)
                {
                    carros[i].estado = ATRAVESSANDO;
                }

                if (carros[i].estado == ATRAVESSANDO && carros[i].x >= 580)
                {
                    carros[i].estado = SAINDO;
                }

                if (carros[i].x > largura + 50)
                {
                    carros[i].x = -50;
                    carros[i].estado = APROXIMANDO;
                }

                break;
            }

            case DIREITA:
            {
                float limite = 590.0f;

                if (carros[i].estado != APROXIMANDO || sinalVerde)
                {
                    carros[i].x -= velocidade;
                }
                else if (carros[i].x > limite)
                {
                    carros[i].x -= velocidade;

                    if (carros[i].x < limite)
                        carros[i].x = limite;
                }

                if (carros[i].estado == APROXIMANDO && sinalVerde && carros[i].x <= limite)
                {
                    carros[i].estado = ATRAVESSANDO;
                }

                if (carros[i].estado == ATRAVESSANDO && carros[i].x + 38 < 420)
                {
                    carros[i].estado = SAINDO;
                }

                if (carros[i].x < -50)
                {
                    carros[i].x = largura + 50;
                    carros[i].estado = APROXIMANDO;
                }

                break;
            }
        }
    }
}

int main(void)
{
    const int largura = 1000;
    const int altura = 700;

    Carro carros[MAX_CARROS] = {
        {455, 100, CIMA, RED, 0},
        {515, 520, BAIXO, BLUE, 0},
        {100, 290, ESQUERDA, PINK, 0},
        {800, 370, DIREITA, ORANGE, 0}
    };

    int quantidadeCarros = 4;
    int fluxoVerticalVerde = 1;
    bool exibirGrade = true;

    InitWindow(largura, altura, "Caos no Cruzamento");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_SPACE))
        {
            fluxoVerticalVerde = !fluxoVerticalVerde;
        }

        // Alterna visualização da grade ao pressionar a tecla G
        if (IsKeyPressed(KEY_G))
        {
            exibirGrade = !exibirGrade;
        }

        atualizarCarros(
            carros,
            quantidadeCarros,
            largura,
            altura,
            fluxoVerticalVerde
        );

        // Atualiza a matriz de ocupação a cada frame
        atualizarMatrizComCarros(carros, quantidadeCarros);

        BeginDrawing();

        ClearBackground((Color){65, 135, 75, 255});

        // Ruas
        DrawRectangle(0, 270, largura, 160, DARKGRAY);
        DrawRectangle(420, 0, 160, altura, DARKGRAY);

        // Ciclovia
        DrawRectangle(0, 270, 420, 20, RED);
        DrawRectangle(580, 270, 420, 20, RED);

        // Sinalizacao tracejada da ciclovia
        for (int x = 0; x < 420; x += 30) DrawRectangle(x, 279, 15, 2, RAYWHITE);
        for (int x = 580; x < largura; x += 30) DrawRectangle(x, 279, 15, 2, RAYWHITE);

        // Faixas centrais
        for (int x = 0; x < largura; x += 60) DrawRectangle(x, 347, 30, 6, YELLOW);
        for (int y = 0; y < altura; y += 60) DrawRectangle(497, y, 6, 30, YELLOW);

        // Faixas de pedestres
        for (int i = 0; i < 6; i++)
        {
            DrawRectangle(435 + i * 22, 274, 12, 12, RAYWHITE);
            DrawRectangle(435 + i * 22, 438, 12, 12, RAYWHITE);
            DrawRectangle(400, 295 + i * 22, 12, 12, RAYWHITE);
            DrawRectangle(588, 295 + i * 22, 12, 12, RAYWHITE);
        }

        // Desenha a grade e células ocupadas da matriz
        desenharGradeMatriz(exibirGrade);

        // Titulo e instrucoes
        DrawText("CAOS NO CRUZAMENTO", 20, 20, 30, RAYWHITE);
        DrawText("ESPACO: Alterna fluxo | G: Ligas/Desliga Grade", 20, 60, 20, RAYWHITE);

        // Semaforo
        DrawRectangle(390, 235, 30, 55, BLACK);

        if (fluxoVerticalVerde)
        {
            DrawCircle(405, 250, 8, GREEN);
            DrawCircle(405, 275, 8, RED);
        }
        else
        {
            DrawCircle(405, 250, 8, RED);
            DrawCircle(405, 275, 8, GREEN);
        }

        desenharCarros(carros, quantidadeCarros);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}