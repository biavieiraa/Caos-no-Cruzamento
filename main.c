#include "raylib.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define MAX_CARROS 60

#define CIMA 0
#define BAIXO 1
#define ESQUERDA 2
#define DIREITA 3

#define APROXIMANDO 0
#define ATRAVESSANDO 1
#define SAINDO 2

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

int matrizMapa[MAPA_ALTURA][MAPA_LARGURA];

// --- GESTÃO DE RECORDES (FICHEIROS EM C) ---
int carregarRecorde(void)
{
    FILE *f = fopen("recorde.txt", "r");
    int rec = 0;
    if (f)
    {
        fscanf(f, "%d", &rec);
        fclose(f);
    }
    return rec;
}

void salvarRecorde(int pontos)
{
    int recAtual = carregarRecorde();
    if (pontos > recAtual)
    {
        FILE *f = fopen("recorde.txt", "w");
        if (f)
        {
            fprintf(f, "%d", pontos);
            fclose(f);
        }
    }
}

// --- LIMPEZA E MATRIZ ---
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

        if (carros[i].direcao == CIMA ||
            carros[i].direcao == BAIXO)
        {
            larguraCarro = 26;
            alturaCarro = 38;
        }

        int colInicial = (int)(carros[i].x / TAM_CELULA);
        int linInicial = (int)(carros[i].y / TAM_CELULA);

        int colFinal = (int)((carros[i].x + larguraCarro - 1) / TAM_CELULA);
        int linFinal = (int)((carros[i].y + alturaCarro - 1) / TAM_CELULA);

        for (int l = linInicial; l <= linFinal; l++)
        {
            for (int c = colInicial; c <= colFinal; c++)
            {
                if (l >= 0 && l < MAPA_ALTURA &&
                    c >= 0 && c < MAPA_LARGURA)
                {
                    matrizMapa[l][c] = 1;
                }
            }
        }
    }
}

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

// --- DESENHO DOS CARROS ---
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

// --- GERADOR AUTOMÁTICO DE CARROS ---
void tentarGerarCarro(Carro carros[], int *quantidade, int largura, int altura)
{
    if (*quantidade >= MAX_CARROS) return;

    int dir = rand() % 4;
    float startX = 0, startY = 0;
    Color cores[] = { RED, BLUE, PINK, ORANGE, PURPLE, YELLOW, LIME, SKYBLUE };
    Color corSorteada = cores[rand() % 8];

    switch (dir)
    {
        case CIMA:     startX = 455; startY = -60; break;
        case BAIXO:    startX = 515; startY = altura + 20; break;
        case ESQUERDA: startX = -60; startY = 290; break;
        case DIREITA:  startX = largura + 20; startY = 370; break;
    }

    // Verifica se a entrada está livre para não gerar em cima de outro carro
    for (int i = 0; i < *quantidade; i++)
    {
        if (carros[i].direcao == dir)
        {
        if (dir == CIMA &&
    carros[i].y >= startY &&
    carros[i].y < startY + 80)
    return;

if (dir == BAIXO &&
    carros[i].y <= startY &&
    carros[i].y > startY - 80)
    return;

if (dir == ESQUERDA &&
    carros[i].x >= startX &&
    carros[i].x < startX + 80)
    return;

if (dir == DIREITA &&
    carros[i].x <= startX &&
    carros[i].x > startX - 80)
    return;
        }
    }

    carros[*quantidade] = (Carro){ startX, startY, dir, corSorteada, APROXIMANDO, false };
    (*quantidade)++;
}

// --- MOVIMENTAÇÃO E SISTEMA DE FILAS ---
void atualizarCarrosEFilas(
    Carro carros[],
    int *quantidade,
    int largura,
    int altura,
    bool fluxoVerticalVerde,
    float multiplicadorDificuldade,
    int *pontos
)
{
    float velocidadeBase = 180.0f * multiplicadorDificuldade * GetFrameTime();

    for (int i = 0; i < *quantidade; i++)
    {
        int carroVertical = (carros[i].direcao == CIMA || carros[i].direcao == BAIXO);
        int sinalVerde = (carroVertical == fluxoVerticalVerde);
        int estadoAnterior = carros[i].estado;
        bool deveParar = false;

// Verifica a distância disponível até o carro da frente
float movimentoPermitido = velocidadeBase;
const float distanciaSeguranca = 12.0f;

for (int j = 0; j < *quantidade; j++)
{
    if (i == j || carros[i].direcao != carros[j].direcao)
        continue;

    float distancia = 0.0f;
    bool carroNaFrente = false;

    switch (carros[i].direcao)
    {
        case CIMA:
            carroNaFrente = carros[j].y > carros[i].y;
            distancia = carros[j].y - (carros[i].y + 38);
            break;

        case BAIXO:
            carroNaFrente = carros[j].y < carros[i].y;
            distancia = carros[i].y - (carros[j].y + 38);
            break;

        case ESQUERDA:
            carroNaFrente = carros[j].x > carros[i].x;
            distancia = carros[j].x - (carros[i].x + 38);
            break;

        case DIREITA:
            carroNaFrente = carros[j].x < carros[i].x;
            distancia = carros[i].x - (carros[j].x + 38);
            break;
    }

    if (carroNaFrente)
    {
        float espacoDisponivel = distancia - distanciaSeguranca;

        if (espacoDisponivel < 0.0f)
            espacoDisponivel = 0.0f;

        if (espacoDisponivel < movimentoPermitido)
            movimentoPermitido = espacoDisponivel;
    }
}

if (movimentoPermitido <= 0.0f)
{
    deveParar = true;
}
        if (!deveParar)
        {
            switch (carros[i].direcao)
            {
                case CIMA:
                {
                    float limite = 236.0f;
                    if (carros[i].estado != APROXIMANDO || sinalVerde) carros[i].y += movimentoPermitido;
                    else if (carros[i].y + 38 < limite)
                    {
                        carros[i].y += movimentoPermitido;
                        if (carros[i].y + 38 > limite) carros[i].y = limite - 38;
                    }
                    else deveParar = true;

                    if (carros[i].estado == APROXIMANDO && sinalVerde && carros[i].y + 38 >= limite) carros[i].estado = ATRAVESSANDO;
                    if (carros[i].estado == ATRAVESSANDO && carros[i].y >= 430) carros[i].estado = SAINDO;
                    break;
                }
                case BAIXO:
                {
                    float limite = 450.0f;
                    if (carros[i].estado != APROXIMANDO || sinalVerde) carros[i].y -= movimentoPermitido;
                    else if (carros[i].y > limite)
                    {
                        carros[i].y -= movimentoPermitido;
                        if (carros[i].y < limite) carros[i].y = limite;
                    }
                    else deveParar = true;

                    if (carros[i].estado == APROXIMANDO && sinalVerde && carros[i].y <= limite) carros[i].estado = ATRAVESSANDO;
                    if (carros[i].estado == ATRAVESSANDO && carros[i].y + 38 < 270) carros[i].estado = SAINDO;
                    break;
                }
                case ESQUERDA:
                {
                    float limite = 407.0f;
                    if (carros[i].estado != APROXIMANDO || sinalVerde) carros[i].x += movimentoPermitido;
                    else if (carros[i].x + 38 < limite)
                    {
                        carros[i].x += movimentoPermitido;
                        if (carros[i].x + 38 > limite) carros[i].x = limite - 38;
                    }
                    else deveParar = true;

                    if (carros[i].estado == APROXIMANDO && sinalVerde && carros[i].x + 38 >= limite) carros[i].estado = ATRAVESSANDO;
                    if (carros[i].estado == ATRAVESSANDO && carros[i].x >= 580) carros[i].estado = SAINDO;
                    break;
                }
                case DIREITA:
                {
                    float limite = 590.0f;
                    if (carros[i].estado != APROXIMANDO || sinalVerde) carros[i].x -= movimentoPermitido;
                    else if (carros[i].x > limite)
                    {
                        carros[i].x -= movimentoPermitido;
                        if (carros[i].x < limite) carros[i].x = limite;
                    }
                    else deveParar = true;

                    if (carros[i].estado == APROXIMANDO && sinalVerde && carros[i].x <= limite) carros[i].estado = ATRAVESSANDO;
                    if (carros[i].estado == ATRAVESSANDO && carros[i].x + 38 < 420) carros[i].estado = SAINDO;
                    break;
                }
            }
        }

  carros[i].parado = deveParar || movimentoPermitido <= 0.0f;

        // Contagem de Pontuação ao sair
        if (estadoAnterior == ATRAVESSANDO && carros[i].estado == SAINDO)
        {
            *pontos += 10;
        }

        // Remoção do carro quando sai completamente do ecrã
        if ((carros[i].direcao == CIMA && carros[i].y > altura + 40) ||
    (carros[i].direcao == BAIXO && carros[i].y < -40) ||
    (carros[i].direcao == ESQUERDA && carros[i].x > largura + 40) ||
    (carros[i].direcao == DIREITA && carros[i].x < -40))
        {
            for (int k = i; k < *quantidade - 1; k++)
            {
                carros[k] = carros[k + 1];
            }
            (*quantidade)--;
            i--;
        }
    }
}

// --- VERIFICAÇÃO DE ACIDENTES ---
bool verificarBatidas(Carro carros[], int quantidade)
{
    for (int i = 0; i < quantidade; i++)
    {
        for (int j = i + 1; j < quantidade; j++)
        {
            Rectangle rectA = (carros[i].direcao == CIMA || carros[i].direcao == BAIXO) ?
                (Rectangle){ carros[i].x, carros[i].y, 26, 38 } :
                (Rectangle){ carros[i].x, carros[i].y, 38, 26 };

            Rectangle rectB = (carros[j].direcao == CIMA || carros[j].direcao == BAIXO) ?
                (Rectangle){ carros[j].x, carros[j].y, 26, 38 } :
                (Rectangle){ carros[j].x, carros[j].y, 38, 26 };

            if (CheckCollisionRecs(rectA, rectB)) return true;
        }
    }
    return false;
}

// --- REINICIALIZAÇÃO ---
void reiniciarJogo(
    Carro carros[],
    int *quantidade,
    int *pontos,
    float *tempoJogado,
    float *barraCongestionamento,
    bool *gameOver,
    bool *colapso,
    bool *fluxoVerticalVerde,
    float *tempoUltimoSpawn
)
{
    *quantidade = 0;
    *pontos = 0;
    *tempoJogado = 0.0f;
    *barraCongestionamento = 0.0f;
    *gameOver = false;
    *colapso = false;
    *fluxoVerticalVerde = true;
    *tempoUltimoSpawn = 0.0f;
}

int main(void)
{
    srand((unsigned int)time(NULL));

    const int largura = 1000;
    const int altura = 700;

    Carro carros[MAX_CARROS];
    int quantidadeCarros = 0;
    bool fluxoVerticalVerde = true;
    bool exibirGrade = true;
    bool gameOver = false;
    bool colapso = false;

    int pontos = 0;
    int recorde = carregarRecorde();
    float tempoJogado = 0.0f;
    float barraCongestionamento = 0.0f;
    float tempoUltimoSpawn = 0.0f;

    InitWindow(largura, altura, "Caos no Cruzamento - Modo Sobrevivência");
    SetTargetFPS(60);

    reiniciarJogo(carros, &quantidadeCarros, &pontos, &tempoJogado, &barraCongestionamento, &gameOver, &colapso, &fluxoVerticalVerde, &tempoUltimoSpawn);

    while (!WindowShouldClose())
    {
        if (!gameOver)
        {
            tempoJogado += GetFrameTime();
            tempoUltimoSpawn += GetFrameTime();

            // Dificuldade e Frequência de Spawn
            float intervaloSpawn = 3.0f;
            float multiplicadorVelocidade = 1.0f;

            if (tempoJogado >= 90.0f) { intervaloSpawn = 1.0f; multiplicadorVelocidade = 1.5f; }
            else if (tempoJogado >= 60.0f) { intervaloSpawn = 1.5f; multiplicadorVelocidade = 1.3f; }
            else if (tempoJogado >= 30.0f) { intervaloSpawn = 2.0f; multiplicadorVelocidade = 1.15f; }

            // Gera novos carros automaticamente
            if (tempoUltimoSpawn >= intervaloSpawn)
            {
                tentarGerarCarro(carros, &quantidadeCarros, largura, altura);
                tempoUltimoSpawn = 0.0f;
            }

            if (IsKeyPressed(KEY_SPACE)) fluxoVerticalVerde = !fluxoVerticalVerde;
            if (IsKeyPressed(KEY_G)) exibirGrade = !exibirGrade;

            atualizarCarrosEFilas(carros, &quantidadeCarros, largura, altura, fluxoVerticalVerde, multiplicadorVelocidade, &pontos);
            atualizarMatrizComCarros(carros, quantidadeCarros);

            // Cálculo da Barra de Congestionamento
            int carrosParados = 0;
            for (int i = 0; i < quantidadeCarros; i++) if (carros[i].parado) carrosParados++;

if (carrosParados >= 3)
{
    barraCongestionamento +=
        (carrosParados - 2) * 1.8f * GetFrameTime();
}
else
{
    barraCongestionamento -= 3.0f * GetFrameTime();
}

            if (barraCongestionamento < 0.0f) barraCongestionamento = 0.0f;

            // Verificação de Condições de Game Over
            if (barraCongestionamento >= 100.0f)
            {
                barraCongestionamento = 100.0f;
                gameOver = true;
                colapso = true;
                salvarRecorde(pontos);
                recorde = carregarRecorde();
            }
            else if (verificarBatidas(carros, quantidadeCarros))
            {
                gameOver = true;
                colapso = false;
                salvarRecorde(pontos);
                recorde = carregarRecorde();
            }
        }
        else
        {
            if (IsKeyPressed(KEY_R))
            {
                reiniciarJogo(carros, &quantidadeCarros, &pontos, &tempoJogado, &barraCongestionamento, &gameOver, &colapso, &fluxoVerticalVerde, &tempoUltimoSpawn);
            }
        }

        BeginDrawing();

        ClearBackground((Color){65, 135, 75, 255});

        // Desenhar Ruas
        DrawRectangle(0, 270, largura, 160, DARKGRAY);
        DrawRectangle(420, 0, 160, altura, DARKGRAY);

        // Ciclovia e Marcas
        DrawRectangle(0, 270, 420, 20, RED);
        DrawRectangle(580, 270, 420, 20, RED);
        for (int x = 0; x < 420; x += 30) DrawRectangle(x, 279, 15, 2, RAYWHITE);
        for (int x = 580; x < largura; x += 30) DrawRectangle(x, 279, 15, 2, RAYWHITE);

        // Faixas Centrais
        for (int x = 0; x < largura; x += 60) DrawRectangle(x, 347, 30, 6, YELLOW);
        for (int y = 0; y < altura; y += 60) DrawRectangle(497, y, 6, 30, YELLOW);

        // Faixas de Pedestres
        for (int i = 0; i < 6; i++)
        {
            DrawRectangle(435 + i * 22, 274, 12, 12, RAYWHITE);
            DrawRectangle(435 + i * 22, 438, 12, 12, RAYWHITE);
            DrawRectangle(400, 295 + i * 22, 12, 12, RAYWHITE);
            DrawRectangle(588, 295 + i * 22, 12, 12, RAYWHITE);
        }

        desenharGradeMatriz(exibirGrade);

        // Desenhar Semáforo
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

        // --- HUD ---
        DrawText("CAOS NO CRUZAMENTO", 20, 20, 28, RAYWHITE);
        DrawText("ESPACO: Alterna fluxo | G: Grade", 20, 55, 16, RAYWHITE);

        DrawText(TextFormat("Pontos: %d", pontos), 20, 85, 22, YELLOW);
        DrawText(TextFormat("Recorde: %d", recorde), 20, 110, 18, GOLD);
        DrawText(TextFormat("Tempo: %.0fs", tempoJogado), 20, 132, 18, RAYWHITE);

        // Desenhar Barra de Congestionamento
        DrawText("Congestionamento:", 730, 20, 18, RAYWHITE);
        DrawRectangle(730, 45, 240, 20, DARKGRAY);

        Color corBarra = GREEN;
        if (barraCongestionamento > 60.0f) corBarra = RED;
        else if (barraCongestionamento > 30.0f) corBarra = ORANGE;

        DrawRectangle(730, 45, (int)(2.4f * barraCongestionamento), 20, corBarra);
        DrawRectangleLines(730, 45, 240, 20, RAYWHITE);
        DrawText(TextFormat("%.0f%%", barraCongestionamento), 830, 47, 16, BLACK);

        // --- TELA DE GAME OVER ---
        if (gameOver)
        {
            DrawRectangle(0, 0, largura, altura, Fade(BLACK, 0.8f));
            if (colapso)
            {
                DrawText("CONGESTIONAMENTO TOTAL!", largura/2 - MeasureText("CONGESTIONAMENTO TOTAL!", 34)/2, 260, 34, RED);
                DrawText("A cidade entrou em colapso!", largura/2 - MeasureText("A cidade entrou em colapso!", 20)/2, 305, 20, RAYWHITE);
            }
            else
            {
                DrawText("ACIDENTE NO CRUZAMENTO!", largura/2 - MeasureText("ACIDENTE NO CRUZAMENTO!", 34)/2, 260, 34, RED);
                DrawText("Dois carros colidiram!", largura/2 - MeasureText("Dois carros colidiram!", 20)/2, 305, 20, RAYWHITE);
            }

            DrawText(TextFormat("Pontuacao Final: %d", pontos), largura/2 - MeasureText(TextFormat("Pontuacao Final: %d", pontos), 22)/2, 350, 22, YELLOW);
            DrawText(TextFormat("Melhor Recorde: %d", recorde), largura/2 - MeasureText(TextFormat("Melhor Recorde: %d", recorde), 20)/2, 380, 20, GOLD);
            DrawText("Pressione 'R' para reiniciar", largura/2 - MeasureText("Pressione 'R' para reiniciar", 20)/2, 430, 20, LIGHTGRAY);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}