
#include "raylib.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#define MAX_CARROS 60
#define MAX_CICLISTAS 8
#define MAX_PEDESTRES 6

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

typedef struct NoCarro {
    Carro carro;
    struct NoCarro *proximo;
} NoCarro;

typedef struct {
    float x;
    float y;
    int direcao;
    Color cor;
} Ciclista;

typedef struct {
    float x;
    float y;
    int direcao;
    Color cor;
} Pedestre;


void atualizarMatrizComCarros(Carro carros[], int quantidade);
void desenharMapa(bool fluxoVerticalVerde, int largura, int altura);


void desenharCarros(Carro carros[], int quantidade);

void tentarGerarCiclista(Ciclista ciclistas[], int *quantidade, int largura, int *proximaDirecao);
void atualizarCiclistas(Ciclista ciclistas[], int *quantidade, int largura, float multiplicadorVelocidade);
void desenharCiclistas(Ciclista ciclistas[], int quantidade);

void tentarGerarPedestre(Pedestre pedestres[], int *quantidade, int *proximaDirecao);
void atualizarPedestres(Pedestre pedestres[], int *quantidade);
void desenharPedestres(Pedestre pedestres[], int quantidade);

void tentarGerarCarro(Carro carros[], int *quantidade, int largura, int altura);

void atualizarCarrosEFilas(
    Carro carros[],
    int *quantidade,
    int largura,
    int altura,
    bool fluxoVerticalVerde,
    float multiplicadorDificuldade,
    int *pontos
);

bool verificarBatidas(Carro carros[], int quantidade);

bool verificarAtropelamento(
    Carro carros[],
    int qtdCarros,
    Ciclista ciclistas[],
    int qtdCiclistas
);

bool verificarAtropelamentoPedestres(
    Carro carros[],
    int qtdCarros,
    Pedestre pedestres[],
    int qtdPedestres
);

void reiniciarJogo(
    Carro carros[],
    int *quantidade,
    int *pontos,
    float *tempoJogado,
    float *barraCongestionamento,
    bool *gameOver,
    int *causaGameOver,
    bool *fluxoVerticalVerde,
    float *tempoUltimoSpawn
);

//recordes

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


void adicionarCarro(NoCarro **lista, Carro *novoCarro)
{
    NoCarro *novo = malloc(sizeof(NoCarro));

    if (novo == NULL)
    {
        return;
    }

    novo->carro = *novoCarro;
    novo->proximo = *lista;
    *lista = novo;
}

void liberarCarros(NoCarro **lista)
{
    NoCarro *atual = *lista;

    while (atual != NULL)
    {
        NoCarro *proximo = atual->proximo;
        free(atual);
        atual = proximo;
    }

    *lista = NULL;
}


int main(void)
{
    srand((unsigned int)time(NULL));

    const int largura = 1000;
    const int altura = 700;

    Carro carros[MAX_CARROS];
    int quantidadeCarros = 0;

    Ciclista ciclistas[MAX_CICLISTAS];
    int quantidadeCiclistas = 0;
    int proximaDirecaoCiclista = DIREITA;
    float tempoUltimoCiclistaSpawn = 0.0f;

    Pedestre pedestres[MAX_PEDESTRES];
    int quantidadePedestres = 0;
    int proximaDirecaoPedestre = BAIXO;
    float tempoUltimoPedestreSpawn = 0.0f;

    bool fluxoVerticalVerde = true;
    bool gameOver = false;
    int causaGameOver = 0;

    int pontos = 0;
    int recorde = carregarRecorde();

    float tempoJogado = 0.0f;
    float barraCongestionamento = 0.0f;
    float tempoUltimoSpawn = 0.0f;

    InitWindow(largura, altura, "Caos no Cruzamento");
    SetTargetFPS(60);

    reiniciarJogo(
        carros,
        &quantidadeCarros,
        &pontos,
        &tempoJogado,
        &barraCongestionamento,
        &gameOver,
        &causaGameOver,
        &fluxoVerticalVerde,
        &tempoUltimoSpawn
    );

    while (!WindowShouldClose())
    {
        if (!gameOver)
        {
            tempoJogado += GetFrameTime();
            tempoUltimoSpawn += GetFrameTime();
            tempoUltimoCiclistaSpawn += GetFrameTime();
            tempoUltimoPedestreSpawn += GetFrameTime();

            //frequencia aparecer coisas
            float intervaloSpawn = 3.0f;
            float multiplicadorVelocidade = 1.0f;

            if (tempoJogado >= 90.0f)
            {
                intervaloSpawn = 1.0f;
                multiplicadorVelocidade = 1.5f;
            }
            else if (tempoJogado >= 60.0f)
            {
                intervaloSpawn = 1.5f;
                multiplicadorVelocidade = 1.3f;
            }
            else if (tempoJogado >= 30.0f)
            {
                intervaloSpawn = 2.0f;
                multiplicadorVelocidade = 1.15f;
            }

            //gera ciclistas
            if (tempoUltimoCiclistaSpawn >= 5.0f)
            {
                tentarGerarCiclista(
                    ciclistas,
                    &quantidadeCiclistas,
                    largura,
                    &proximaDirecaoCiclista
                );

                tempoUltimoCiclistaSpawn = 0.0f;
            }

            atualizarCiclistas(
                ciclistas,
                &quantidadeCiclistas,
                largura,
                multiplicadorVelocidade
            );

            //movimentacao pedestres
            if (tempoUltimoPedestreSpawn >= 3.5f)
            {
                tentarGerarPedestre(
                    pedestres,
                    &quantidadePedestres,
                    &proximaDirecaoPedestre
                );

                tempoUltimoPedestreSpawn = 0.0f;
            }

            atualizarPedestres(pedestres, &quantidadePedestres);

            //gera carros
            if (tempoUltimoSpawn >= intervaloSpawn)
            {
                tentarGerarCarro(
                    carros,
                    &quantidadeCarros,
                    largura,
                    altura
                );

                tempoUltimoSpawn = 0.0f;
            }

            if (IsKeyPressed(KEY_SPACE))
            {
                fluxoVerticalVerde = !fluxoVerticalVerde;
            }

            atualizarCarrosEFilas(
                carros,
                &quantidadeCarros,
                largura,
                altura,
                fluxoVerticalVerde,
                multiplicadorVelocidade,
                &pontos
            );

            atualizarMatrizComCarros(carros, quantidadeCarros);

            //calculo da barra cong
            int carrosParados = 0;

            for (int i = 0; i < quantidadeCarros; i++)
            {
                if (carros[i].parado)
                {
                    carrosParados++;
                }
            }

            if (carrosParados >= 3)
            {
                barraCongestionamento +=
                    (carrosParados - 2) * 1.8f * GetFrameTime();
            }
            else
            {
                barraCongestionamento -= 3.0f * GetFrameTime();
            }

            if (barraCongestionamento < 0.0f)
            {
                barraCongestionamento = 0.0f;
            }

            //condicoes de game Over
            if (barraCongestionamento >= 100.0f)
            {
                barraCongestionamento = 100.0f;
                gameOver = true;
                causaGameOver = 1;
                salvarRecorde(pontos);
                recorde = carregarRecorde();
            }
            else if (verificarBatidas(carros, quantidadeCarros))
            {
                gameOver = true;
                causaGameOver = 2;
                salvarRecorde(pontos);
                recorde = carregarRecorde();
            }
            else if (
                verificarAtropelamento(
                    carros,
                    quantidadeCarros,
                    ciclistas,
                    quantidadeCiclistas
                )
            )
            {
                gameOver = true;
                causaGameOver = 3;
                salvarRecorde(pontos);
                recorde = carregarRecorde();
            }
            else if (
                verificarAtropelamentoPedestres(
                    carros,
                    quantidadeCarros,
                    pedestres,
                    quantidadePedestres
                )
            )
            {
                gameOver = true;
                causaGameOver = 4;
                salvarRecorde(pontos);
                recorde = carregarRecorde();
            }
        }
        else
        {
            if (IsKeyPressed(KEY_R))
            {
                reiniciarJogo(
                    carros,
                    &quantidadeCarros,
                    &pontos,
                    &tempoJogado,
                    &barraCongestionamento,
                    &gameOver,
                    &causaGameOver,
                    &fluxoVerticalVerde,
                    &tempoUltimoSpawn
                );

                quantidadeCiclistas = 0;
                quantidadePedestres = 0;

                proximaDirecaoPedestre = BAIXO;
                tempoUltimoPedestreSpawn = 0.0f;

                proximaDirecaoCiclista = DIREITA;
                tempoUltimoCiclistaSpawn = 0.0f;
            }
        }

        BeginDrawing();

        ClearBackground((Color){65, 135, 75, 255});

        desenharMapa(fluxoVerticalVerde, largura, altura);

        desenharCiclistas(ciclistas, quantidadeCiclistas);
        desenharPedestres(pedestres, quantidadePedestres);
        desenharCarros(carros, quantidadeCarros);

        DrawText("CAOS NO CRUZAMENTO", 20, 20, 28, RAYWHITE);
        DrawText("ESPACO: Alterna fluxo", 20, 55, 16, RAYWHITE);

        DrawText(TextFormat("Pontos: %d", pontos), 20, 85, 22, YELLOW);
        DrawText(TextFormat("Recorde: %d", recorde), 20, 110, 18, GOLD);
        DrawText(TextFormat("Tempo: %.0fs", tempoJogado), 20, 132, 18, RAYWHITE);

        DrawText("Congestionamento:", 730, 20, 18, RAYWHITE);
        DrawRectangle(730, 45, 240, 20, DARKGRAY);

        Color corBarra = GREEN;

        if (barraCongestionamento > 60.0f)
        {
            corBarra = RED;
        }
        else if (barraCongestionamento > 30.0f)
        {
            corBarra = ORANGE;
        }

        DrawRectangle(
            730,
            45,
            (int)(2.4f * barraCongestionamento),
            20,
            corBarra
        );

        DrawRectangleLines(730, 45, 240, 20, RAYWHITE);

        DrawText(
            TextFormat("%.0f%%", barraCongestionamento),
            830,
            47,
            16,
            BLACK
        );


        if (gameOver)
        {
            DrawRectangle(0, 0, largura, altura, Fade(BLACK, 0.8f));

            if (causaGameOver == 1)
            {
                DrawText(
                    "CONGESTIONAMENTO TOTAL!",
                    largura / 2 - MeasureText("CONGESTIONAMENTO TOTAL!", 34) / 2,
                    260,
                    34,
                    RED
                );

                DrawText(
                    "A cidade entrou em colapso!",
                    largura / 2 - MeasureText("A cidade entrou em colapso!", 20) / 2,
                    305,
                    20,
                    RAYWHITE
                );
            }
            else if (causaGameOver == 2)
            {
                DrawText(
                    "ACIDENTE NO CRUZAMENTO!",
                    largura / 2 - MeasureText("ACIDENTE NO CRUZAMENTO!", 34) / 2,
                    260,
                    34,
                    RED
                );

                DrawText(
                    "Dois carros colidiram!",
                    largura / 2 - MeasureText("Dois carros colidiram!", 20) / 2,
                    305,
                    20,
                    RAYWHITE
                );
            }
            else if (causaGameOver == 3)
            {
                DrawText(
                    "ATROPELAMENTO NA CICLOVIA!",
                    largura / 2 - MeasureText("ATROPELAMENTO NA CICLOVIA!", 34) / 2,
                    260,
                    34,
                    RED
                );

                DrawText(
                    "Um carro atingiu um ciclista!",
                    largura / 2 - MeasureText("Um carro atingiu um ciclista!", 20) / 2,
                    305,
                    20,
                    RAYWHITE
                );
            }
            else if (causaGameOver == 4)
            {
                DrawText(
                    "ATROPELAMENTO DE PEDESTRE!",
                    largura / 2 - MeasureText("ATROPELAMENTO DE PEDESTRE!", 34) / 2,
                    260,
                    34,
                    RED
                );

                DrawText(
                    "Um carro atingiu um pedestre!",
                    largura / 2 - MeasureText("Um carro atingiu um pedestre!", 20) / 2,
                    305,
                    20,
                    RAYWHITE
                );
            }

            DrawText(
                TextFormat("Pontuacao Final: %d", pontos),
                largura / 2 - MeasureText(
                    TextFormat("Pontuacao Final: %d", pontos),
                    22
                ) / 2,
                350,
                22,
                YELLOW
            );

            DrawText(
                TextFormat("Melhor Recorde: %d", recorde),
                largura / 2 - MeasureText(
                    TextFormat("Melhor Recorde: %d", recorde),
                    20
                ) / 2,
                380,
                20,
                GOLD
            );

            DrawText(
                "Pressione 'R' para reiniciar",
                largura / 2 - MeasureText("Pressione 'R' para reiniciar", 20) / 2,
                430,
                20,
                LIGHTGRAY
            );
        }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
