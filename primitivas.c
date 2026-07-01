#include "Estruturas.h"
#include <stdio.h>
#include <string.h>

int adicionarPonto(CenaGrafica *cena, float x, float y, float cor[3],
                   float tamanho) {
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

int adicionarReta(CenaGrafica *cena, Ponto p1, Ponto p2, float cor[3],
                  float espessura) {
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

int adicionarPoligono(CenaGrafica *cena, Ponto vertices[], int qtd_vertices,
                      float cor_contorno[3], float cor_preenchimento[3],
                      int preenchido, float espessura_contorno) {
  if (cena->qtd_poligonos >= MAX_OBJETOS) {
    printf("[ERRO] Limite maximo de poligonos atingido (%d).\n", MAX_OBJETOS);
    return 0;
  }

  if (qtd_vertices < 3) {
    printf("[ERRO] Poligono precisa de pelo menos 3 vertices (recebeu %d).\n",
           qtd_vertices);
    return 0;
  }

  if (qtd_vertices > MAX_VERTICES) {
    printf("[AVISO] Poligono truncado para %d vertices (recebeu %d).\n",
           MAX_VERTICES, qtd_vertices);
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
  novo->espessura_contorno =
      (espessura_contorno > 0.0f) ? espessura_contorno : 1.0f;
  novo->selecionado = 0;

  cena->qtd_poligonos++;

  return 1;
}
