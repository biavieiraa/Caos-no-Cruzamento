
#include "raylib.h"
#include <stdbool.h>
#include <stdlib.h>
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

typedef struct {
    float x;
    float y;
    int direcao;
    Color cor;
    int estado;
    bool parado;
} Carro;

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

//carros

void desenharCarros(Carro carros[], int quantidade)
{
    for (int i = 0; i < quantidade; i++)
    {
        float x = carros[i].x;
        float y = carros[i].y;

        Color cor = carros[i].cor;
        Color sombra = (Color){25, 25, 25, 255};
        Color vidro = (Color){160, 210, 225, 255};
        Color pneu = (Color){20, 20, 20, 255};
        Color detalhe = (Color){230, 230, 230, 255};

        //carros vert
        if (carros[i].direcao == CIMA || carros[i].direcao == BAIXO)
        {
            DrawRectangleRounded(
                (Rectangle){x + 2, y + 2, 26, 38},
                0.25f, 6, sombra
            );

            DrawRectangleRounded(
                (Rectangle){x - 2, y + 5, 5, 10},
                0.4f, 4, pneu
            );

            DrawRectangleRounded(
                (Rectangle){x + 23, y + 5, 5, 10},
                0.4f, 4, pneu
            );

            DrawRectangleRounded(
                (Rectangle){x - 2, y + 24, 5, 10},
                0.4f, 4, pneu
            );

            DrawRectangleRounded(
                (Rectangle){x + 23, y + 24, 5, 10},
                0.4f, 4, pneu
            );

            DrawRectangleRounded(
                (Rectangle){x, y, 26, 38},
                0.25f, 6, cor
            );

            DrawRectangleRounded(
                (Rectangle){x + 4, y + 5, 18, 9},
                0.2f, 5, vidro
            );

            DrawRectangleRounded(
                (Rectangle){x + 4, y + 24, 18, 8},
                0.2f, 5, vidro
            );

            DrawRectangle(
                (int)x + 4,
                (int)y + 17,
                18,
                2,
                Fade(BLACK, 0.25f)
            );

            DrawRectangle((int)x + 3, (int)y + 1, 5, 2, detalhe);
            DrawRectangle((int)x + 18, (int)y + 1, 5, 2, detalhe);
            DrawRectangle((int)x + 3, (int)y + 35, 5, 2, RED);
            DrawRectangle((int)x + 18, (int)y + 35, 5, 2, RED);
        }
        //carros horiz
        else
        {
            DrawRectangleRounded(
                (Rectangle){x + 2, y + 2, 38, 26},
                0.25f, 6, sombra
            );

            DrawRectangleRounded(
                (Rectangle){x + 5, y - 2, 10, 5},
                0.4f, 4, pneu
            );

            DrawRectangleRounded(
                (Rectangle){x + 24, y - 2, 10, 5},
                0.4f, 4, pneu
            );

            DrawRectangleRounded(
                (Rectangle){x + 5, y + 23, 10, 5},
                0.4f, 4, pneu
            );

            DrawRectangleRounded(
                (Rectangle){x + 24, y + 23, 10, 5},
                0.4f, 4, pneu
            );

            DrawRectangleRounded(
                (Rectangle){x, y, 38, 26},
                0.25f, 6, cor
            );

            DrawRectangleRounded(
                (Rectangle){x + 5, y + 4, 9, 18},
                0.2f, 5, vidro
            );

            DrawRectangleRounded(
                (Rectangle){x + 24, y + 4, 9, 18},
                0.2f, 5, vidro
            );

            DrawRectangle(
                (int)x + 17,
                (int)y + 4,
                2,
                18,
                Fade(BLACK, 0.25f)
            );

            DrawRectangle((int)x + 1, (int)y + 3, 2, 5, detalhe);
            DrawRectangle((int)x + 1, (int)y + 18, 2, 5, detalhe);
            DrawRectangle((int)x + 35, (int)y + 3, 2, 5, RED);
            DrawRectangle((int)x + 35, (int)y + 18, 2, 5, RED);
        }
    }
}

//ciclistas

void tentarGerarCiclista(
    Ciclista ciclistas[],
    int *quantidade,
    int largura,
    int *proximaDirecao
)
{
    if (*quantidade >= MAX_CICLISTAS) return;

    int direcao = *proximaDirecao;

    float xInicial = (direcao == DIREITA)
        ? -25.0f
        : (float)largura + 25.0f;

    for (int i = 0; i < *quantidade; i++)
    {
        if (ciclistas[i].direcao == direcao)
        {
            if (direcao == DIREITA && ciclistas[i].x < 75.0f)
                return;

            if (direcao == ESQUERDA && ciclistas[i].x > largura - 75.0f)
                return;
        }
    }

    Color cores[] = {
        (Color){30, 90, 210, 255},
        (Color){245, 145, 30, 255},
        (Color){220, 50, 80, 255},
        (Color){40, 160, 90, 255}
    };

    ciclistas[*quantidade] = (Ciclista){
        xInicial,
        282.0f,
        direcao,
        cores[rand() % 4]
    };

    (*quantidade)++;

    *proximaDirecao = (direcao == DIREITA) ? ESQUERDA : DIREITA;
}

void atualizarCiclistas(
    Ciclista ciclistas[],
    int *quantidade,
    int largura,
    float multiplicadorVelocidade
)
{
    float velocidade = 100.0f * multiplicadorVelocidade * GetFrameTime();

    for (int i = 0; i < *quantidade; i++)
    {
        if (ciclistas[i].direcao == DIREITA)
            ciclistas[i].x += velocidade;
        else
            ciclistas[i].x -= velocidade;

        if (
            (ciclistas[i].direcao == DIREITA && ciclistas[i].x > largura + 30.0f) ||
            (ciclistas[i].direcao == ESQUERDA && ciclistas[i].x < -30.0f)
        )
        {
            for (int j = i; j < *quantidade - 1; j++)
            {
                ciclistas[j] = ciclistas[j + 1];
            }

            (*quantidade)--;
            i--;
        }
    }
}

void desenharCiclistas(Ciclista ciclistas[], int quantidade)
{
    for (int i = 0; i < quantidade; i++)
    {
        float x = ciclistas[i].x;
        float y = ciclistas[i].y;

        float sentido = (ciclistas[i].direcao == DIREITA)
            ? 1.0f
            : -1.0f;

        Color escuro = (Color){25, 35, 35, 255};

        DrawCircle((int)(x - 7), (int)y, 4, escuro);
        DrawCircle((int)(x + 7), (int)y, 4, escuro);

        DrawCircleLines((int)(x - 7), (int)y, 4, RAYWHITE);
        DrawCircleLines((int)(x + 7), (int)y, 4, RAYWHITE);

        DrawLineEx(
            (Vector2){x - 7, y},
            (Vector2){x, y - 5},
            2,
            ciclistas[i].cor
        );

        DrawLineEx(
            (Vector2){x, y - 5},
            (Vector2){x + 7, y},
            2,
            ciclistas[i].cor
        );

        DrawLineEx(
            (Vector2){x - 7, y},
            (Vector2){x + 7, y},
            2,
            ciclistas[i].cor
        );

        DrawLineEx(
            (Vector2){x + 5 * sentido, y - 5},
            (Vector2){x + 9 * sentido, y - 8},
            2,
            escuro
        );

        DrawCircle(
            (int)x,
            (int)(y - 13),
            3,
            (Color){245, 195, 150, 255}
        );

        DrawLineEx(
            (Vector2){x, y - 10},
            (Vector2){x - 2 * sentido, y - 5},
            3,
            ciclistas[i].cor
        );

        DrawLineEx(
            (Vector2){x - 1 * sentido, y - 8},
            (Vector2){x + 6 * sentido, y - 8},
            2,
            escuro
        );

        DrawLineEx(
            (Vector2){x - 2 * sentido, y - 5},
            (Vector2){x - 7, y},
            2,
            escuro
        );

        DrawLineEx(
            (Vector2){x - 2 * sentido, y - 5},
            (Vector2){x + 7, y},
            2,
            escuro
        );
    }
}

//pessoas

void tentarGerarPedestre(
    Pedestre pedestres[],
    int *quantidade,
    int *proximaDirecao
)
{
    if (*quantidade >= MAX_PEDESTRES) return;

    int direcao = *proximaDirecao;

    float x = (direcao == BAIXO) ? 400.0f : 588.0f;
    float y = (direcao == BAIXO) ? 245.0f : 455.0f;

    for (int i = 0; i < *quantidade; i++)
    {
        if (
            pedestres[i].direcao == direcao &&
            fabsf(pedestres[i].y - y) < 60.0f
        )
        {
            return;
        }
    }

    pedestres[*quantidade] = (Pedestre){
        x,
        y,
        direcao,
        (Color){245, 195, 150, 255}
    };

    (*quantidade)++;

    *proximaDirecao = (direcao == BAIXO) ? CIMA : BAIXO;
}

void atualizarPedestres(Pedestre pedestres[], int *quantidade)
{
    float velocidade = 75.0f * GetFrameTime();

    for (int i = 0; i < *quantidade; i++)
    {
        if (pedestres[i].direcao == BAIXO)
            pedestres[i].y += velocidade;
        else
            pedestres[i].y -= velocidade;

        if (pedestres[i].y > 475.0f || pedestres[i].y < 225.0f)
        {
            for (int j = i; j < *quantidade - 1; j++)
            {
                pedestres[j] = pedestres[j + 1];
            }

            (*quantidade)--;
            i--;
        }
    }
}

void desenharPedestres(Pedestre pedestres[], int quantidade)
{
    for (int i = 0; i < quantidade; i++)
    {
        float x = pedestres[i].x;
        float y = pedestres[i].y;

        Color corpo = (Color){40, 90, 200, 255};
        Color pele = pedestres[i].cor;
        Color escuro = (Color){35, 35, 35, 255};

        DrawCircle((int)x, (int)(y - 7), 5, pele);

        DrawLineEx(
            (Vector2){x, y - 2},
            (Vector2){x, y + 10},
            5,
            corpo
        );

        DrawLineEx(
            (Vector2){x, y + 1},
            (Vector2){x - 7, y + 6},
            3,
            corpo
        );

        DrawLineEx(
            (Vector2){x, y + 1},
            (Vector2){x + 7, y + 6},
            3,
            corpo
        );

        DrawLineEx(
            (Vector2){x, y + 10},
            (Vector2){x - 5, y + 17},
            3,
            escuro
        );

        DrawLineEx(
            (Vector2){x, y + 10},
            (Vector2){x + 5, y + 17},
            3,
            escuro
        );
    }
}

void tentarGerarCarro(
    Carro carros[],
    int *quantidade,
    int largura,
    int altura
)
{
    if (*quantidade >= MAX_CARROS) return;

    int dir = rand() % 4;

    float startX = 0;
    float startY = 0;

    Color cores[] = {
        RED, BLUE, PINK, ORANGE,
        PURPLE, YELLOW, LIME, SKYBLUE
    };

    Color corSorteada = cores[rand() % 8];

    switch (dir)
    {
        case CIMA:
            startX = 455;
            startY = -60;
            break;

        case BAIXO:
            startX = 515;
            startY = altura + 20;
            break;

        case ESQUERDA:
            startX = -60;
            startY = 290;
            break;

        case DIREITA:
            startX = largura + 20;
            startY = 370;
            break;
    }

    for (int i = 0; i < *quantidade; i++)
    {
        if (carros[i].direcao == dir)
        {
            if (dir == CIMA && carros[i].y >= startY && carros[i].y < startY + 80)
                return;

            if (dir == BAIXO && carros[i].y <= startY && carros[i].y > startY - 80)
                return;

            if (dir == ESQUERDA && carros[i].x >= startX && carros[i].x < startX + 80)
                return;

            if (dir == DIREITA && carros[i].x <= startX && carros[i].x > startX - 80)
                return;
        }
    }

    carros[*quantidade] = (Carro){
        startX,
        startY,
        dir,
        corSorteada,
        APROXIMANDO,
        false
    };

    (*quantidade)++;
}

//movimentos

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
    float velocidadeBase =
        180.0f * multiplicadorDificuldade * GetFrameTime();

    for (int i = 0; i < *quantidade; i++)
    {
        int carroVertical =
            (carros[i].direcao == CIMA || carros[i].direcao == BAIXO);

        int sinalVerde = (carroVertical == fluxoVerticalVerde);

        int estadoAnterior = carros[i].estado;

        bool deveParar = false;

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
            deveParar = true;

        if (!deveParar)
        {
            switch (carros[i].direcao)
            {
                case CIMA:
                {
                    float limite = 236.0f;

                    if (carros[i].estado != APROXIMANDO || sinalVerde)
                    {
                        carros[i].y += movimentoPermitido;
                    }
                    else if (carros[i].y + 38 < limite)
                    {
                        carros[i].y += movimentoPermitido;

                        if (carros[i].y + 38 > limite)
                            carros[i].y = limite - 38;
                    }
                    else
                    {
                        deveParar = true;
                    }

                    if (
                        carros[i].estado == APROXIMANDO &&
                        sinalVerde &&
                        carros[i].y + 38 >= limite
                    )
                    {
                        carros[i].estado = ATRAVESSANDO;
                    }

                    if (
                        carros[i].estado == ATRAVESSANDO &&
                        carros[i].y >= 430
                    )
                    {
                        carros[i].estado = SAINDO;
                    }

                    break;
                }

                case BAIXO:
                {
                    float limite = 450.0f;

                    if (carros[i].estado != APROXIMANDO || sinalVerde)
                    {
                        carros[i].y -= movimentoPermitido;
                    }
                    else if (carros[i].y > limite)
                    {
                        carros[i].y -= movimentoPermitido;

                        if (carros[i].y < limite)
                            carros[i].y = limite;
                    }
                    else
                    {
                        deveParar = true;
                    }

                    if (
                        carros[i].estado == APROXIMANDO &&
                        sinalVerde &&
                        carros[i].y <= limite
                    )
                    {
                        carros[i].estado = ATRAVESSANDO;
                    }

                    if (
                        carros[i].estado == ATRAVESSANDO &&
                        carros[i].y + 38 < 270
                    )
                    {
                        carros[i].estado = SAINDO;
                    }

                    break;
                }

                case ESQUERDA:
                {
                    float limite = 407.0f;

                    if (carros[i].estado != APROXIMANDO || sinalVerde)
                    {
                        carros[i].x += movimentoPermitido;
                    }
                    else if (carros[i].x + 38 < limite)
                    {
                        carros[i].x += movimentoPermitido;

                        if (carros[i].x + 38 > limite)
                            carros[i].x = limite - 38;
                    }
                    else
                    {
                        deveParar = true;
                    }

                    if (
                        carros[i].estado == APROXIMANDO &&
                        sinalVerde &&
                        carros[i].x + 38 >= limite
                    )
                    {
                        carros[i].estado = ATRAVESSANDO;
                    }

                    if (
                        carros[i].estado == ATRAVESSANDO &&
                        carros[i].x >= 580
                    )
                    {
                        carros[i].estado = SAINDO;
                    }

                    break;
                }

                case DIREITA:
                {
                    float limite = 590.0f;

                    if (carros[i].estado != APROXIMANDO || sinalVerde)
                    {
                        carros[i].x -= movimentoPermitido;
                    }
                    else if (carros[i].x > limite)
                    {
                        carros[i].x -= movimentoPermitido;

                        if (carros[i].x < limite)
                            carros[i].x = limite;
                    }
                    else
                    {
                        deveParar = true;
                    }

                    if (
                        carros[i].estado == APROXIMANDO &&
                        sinalVerde &&
                        carros[i].x <= limite
                    )
                    {
                        carros[i].estado = ATRAVESSANDO;
                    }

                    if (
                        carros[i].estado == ATRAVESSANDO &&
                        carros[i].x + 38 < 420
                    )
                    {
                        carros[i].estado = SAINDO;
                    }

                    break;
                }
            }
        }

        carros[i].parado =
            deveParar || movimentoPermitido <= 0.0f;

        if (
            estadoAnterior == ATRAVESSANDO &&
            carros[i].estado == SAINDO
        )
        {
            *pontos += 10;
        }

        if (
            (carros[i].direcao == CIMA && carros[i].y > altura + 40) ||
            (carros[i].direcao == BAIXO && carros[i].y < -40) ||
            (carros[i].direcao == ESQUERDA && carros[i].x > largura + 40) ||
            (carros[i].direcao == DIREITA && carros[i].x < -40)
        )
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

//batidas e atropelamentos

bool verificarBatidas(Carro carros[], int quantidade)
{
    for (int i = 0; i < quantidade; i++)
    {
        for (int j = i + 1; j < quantidade; j++)
        {
            Rectangle rectA =
                (carros[i].direcao == CIMA || carros[i].direcao == BAIXO)
                ? (Rectangle){carros[i].x, carros[i].y, 26, 38}
                : (Rectangle){carros[i].x, carros[i].y, 38, 26};

            Rectangle rectB =
                (carros[j].direcao == CIMA || carros[j].direcao == BAIXO)
                ? (Rectangle){carros[j].x, carros[j].y, 26, 38}
                : (Rectangle){carros[j].x, carros[j].y, 38, 26};

            if (CheckCollisionRecs(rectA, rectB))
                return true;
        }
    }

    return false;
}


bool verificarAtropelamento(
    Carro carros[],
    int qtdCarros,
    Ciclista ciclistas[],
    int qtdCiclistas
)
{
    for (int i = 0; i < qtdCarros; i++)
    {
        Rectangle rectCarro =
            (carros[i].direcao == CIMA || carros[i].direcao == BAIXO)
            ? (Rectangle){carros[i].x, carros[i].y, 26, 38}
            : (Rectangle){carros[i].x, carros[i].y, 38, 26};

        for (int j = 0; j < qtdCiclistas; j++)
        {
            Rectangle rectCiclista = {
                ciclistas[j].x - 8,
                ciclistas[j].y - 14,
                16,
                18
            };

            if (CheckCollisionRecs(rectCarro, rectCiclista))
            {
                return true;
            }
        }
    }

    return false;
}


bool verificarAtropelamentoPedestres(
    Carro carros[],
    int qtdCarros,
    Pedestre pedestres[],
    int qtdPedestres
)
{
    for (int i = 0; i < qtdCarros; i++)
    {
        Rectangle rectCarro =
            (carros[i].direcao == CIMA || carros[i].direcao == BAIXO)
            ? (Rectangle){carros[i].x, carros[i].y, 26, 38}
            : (Rectangle){carros[i].x, carros[i].y, 38, 26};

        for (int j = 0; j < qtdPedestres; j++)
        {
            Rectangle rectPedestre = {
                pedestres[j].x - 6,
                pedestres[j].y - 7,
                12,
                24
            };

            if (CheckCollisionRecs(rectCarro, rectPedestre))
            {
                return true;
            }
        }
    }

    return false;
}

//reiniciar

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
)
{
    *quantidade = 0;
    *pontos = 0;
    *tempoJogado = 0.0f;
    *barraCongestionamento = 0.0f;
    *gameOver = false;

    *causaGameOver = 0;

    *fluxoVerticalVerde = true;
    *tempoUltimoSpawn = 0.0f;
}
