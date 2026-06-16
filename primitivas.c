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

// =============================================================================
// adicionarPoligono
// -----------------------------------------------------------------------------
// Cria um Poligono na CenaGrafica a partir de um array de vertices.
//
// Parametros:
//   cena              - ponteiro para a cena
//   vertices          - array de pontos (vertices do poligono)
//   qtd_vertices      - quantidade de vertices (minimo 3)
//   cor_contorno      - cor RGB do contorno
//   cor_preenchimento - cor RGB do preenchimento
//   preenchido        - 0 = so contorno, 1 = preenchido
//   espessura_contorno- espessura das arestas
//
// Retorno:
//   1 se o poligono foi adicionado com sucesso, 0 se falhou.
// =============================================================================
int adicionarPoligono(CenaGrafica *cena, Ponto vertices[], int qtd_vertices,
                      float cor_contorno[3], float cor_preenchimento[3],
                      int preenchido, float espessura_contorno) {
    if (cena->qtd_poligonos >= MAX_OBJETOS) {
        printf("[ERRO] Limite maximo de poligonos atingido (%d).\n", MAX_OBJETOS);
        return 0;
    }

    if (qtd_vertices < 3) {
        printf("[ERRO] Poligono precisa de pelo menos 3 vertices (recebeu %d).\n", qtd_vertices);
        return 0;
    }

    if (qtd_vertices > MAX_VERTICES) {
        printf("[AVISO] Poligono truncado para %d vertices (recebeu %d).\n", MAX_VERTICES, qtd_vertices);
        qtd_vertices = MAX_VERTICES;
    }

    Poligono *novo = &cena->poligonos[cena->qtd_poligonos];

    for (int i = 0; i < qtd_vertices; i++) {
        novo->vertices[i] = vertices[i];
    }
    novo->qtd_vertices = qtd_vertices;

    novo->cor_contorno[0] = cor_contorno[0];
    novo->cor_contorno[1] = cor_contorno[1];
    novo->cor_contorno[2] = cor_contorno[2];

    novo->cor_preenchimento[0] = cor_preenchimento[0];
    novo->cor_preenchimento[1] = cor_preenchimento[1];
    novo->cor_preenchimento[2] = cor_preenchimento[2];

    novo->preenchido = preenchido;
    novo->espessura_contorno = (espessura_contorno > 0.0f) ? espessura_contorno : 1.0f;
    novo->selecionado = 0;

    cena->qtd_poligonos++;

    return 1;
}
