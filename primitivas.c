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
