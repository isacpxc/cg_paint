// primitivas.c
// Modulo de Criacao de Primitivas - Responsavel: Isac Peixoto Costa
// Contem as funcoes que transformam cliques do mouse em objetos na CenaGrafica.

#include <stdio.h>
#include <string.h>
#include "Estruturas.h"

// =============================================================================
// adicionarPonto
// -----------------------------------------------------------------------------
// Cria um PontoDesenho na CenaGrafica a partir das coordenadas do clique.
//
// Parametros:
//   cena    - ponteiro para a cena onde o ponto sera adicionado
//   x, y    - coordenadas do clique (ja convertidas para coordenadas do mundo)
//   cor     - array RGB [R, G, B] com valores entre 0.0 e 1.0
//   tamanho - tamanho do ponto em pixels (usado no glPointSize)
//
// Retorno:
//   1 se o ponto foi adicionado com sucesso, 0 se a cena esta cheia.
// =============================================================================
int adicionarPonto(CenaGrafica *cena, float x, float y, float cor[3], float tamanho) {
    if (cena->qtd_pontos >= MAX_OBJETOS) {
        printf("[ERRO] Limite maximo de pontos atingido (%d).\n", MAX_OBJETOS);
        return 0;
    }

    PontoDesenho *novo = &cena->pontos[cena->qtd_pontos];

    novo->posicao.x = x;
    novo->posicao.y = y;

    novo->cor[0] = cor[0];
    novo->cor[1] = cor[1];
    novo->cor[2] = cor[2];

    novo->tamanho = (tamanho > 0.0f) ? tamanho : 1.0f;

    novo->selecionado = 0;

    cena->qtd_pontos++;

    return 1;
}

// =============================================================================
// adicionarReta
// -----------------------------------------------------------------------------
// Cria uma Reta na CenaGrafica a partir de dois pontos.
//
// Parametros:
//   cena      - ponteiro para a cena onde a reta sera adicionada
//   p1, p2    - pontos inicial e final da reta
//   cor       - array RGB [R, G, B] com valores entre 0.0 e 1.0
//   espessura - espessura da linha em pixels (usado no glLineWidth)
//
// Retorno:
//   1 se a reta foi adicionada com sucesso, 0 se a cena esta cheia.
// =============================================================================
int adicionarReta(CenaGrafica *cena, Ponto p1, Ponto p2, float cor[3], float espessura) {
    if (cena->qtd_retas >= MAX_OBJETOS) {
        printf("[ERRO] Limite maximo de retas atingido (%d).\n", MAX_OBJETOS);
        return 0;
    }

    Reta *nova = &cena->retas[cena->qtd_retas];

    nova->p1 = p1;
    nova->p2 = p2;

    nova->cor[0] = cor[0];
    nova->cor[1] = cor[1];
    nova->cor[2] = cor[2];

    nova->espessura = (espessura > 0.0f) ? espessura : 1.0f;

    nova->selecionado = 0;

    cena->qtd_retas++;

    return 1;
}
