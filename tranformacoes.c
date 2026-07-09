#include "Estruturas.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

static Ponto aplicaMatriz(Ponto p, Matriz3x3 m) {
  Ponto resultado;
  resultado.x = m.m[0][0] * p.x + m.m[0][1] * p.y + m.m[0][2];
  resultado.y = m.m[1][0] * p.x + m.m[1][1] * p.y + m.m[1][2];
  return resultado;
}

static Matriz3x3 multiplicaMatriz(Matriz3x3 A, Matriz3x3 B) {
  Matriz3x3 R;
  int i, j, k;

  for (i = 0; i < 3; i++) {
    for (j = 0; j < 3; j++) {
      R.m[i][j] = 0;

      for (k = 0; k < 3; k++)
        R.m[i][j] += A.m[i][k] * B.m[k][j];
    }
  }

  return R;
}

static Matriz3x3 matrizTranslacao(float tx, float ty) {
  Matriz3x3 T = {{{1, 0, tx}, {0, 1, ty}, {0, 0, 1}}};

  return T;
}

static Matriz3x3 matrizComposta(Matriz3x3 M, Ponto centro) {
  Matriz3x3 T1 = matrizTranslacao(-centro.x, -centro.y);
  Matriz3x3 T2 = matrizTranslacao(centro.x, centro.y);

  Matriz3x3 Aux;

  Aux = multiplicaMatriz(M, T1);

  return multiplicaMatriz(T2, Aux);
}

static void transformarReta(Reta *r, Matriz3x3 m) {

  r->p1 = aplicaMatriz(r->p1, m);
  r->p2 = aplicaMatriz(r->p2, m);
}

static void transformarPoligono(Poligono *p, Matriz3x3 m) {
  int i;

  for (i = 0; i < p->qtd_vertices; i++) {
    p->vertices[i] = aplicaMatriz(p->vertices[i], m);
  }
}

static Ponto centroReta(Reta *r) {
  Ponto c;

  c.x = (r->p1.x + r->p2.x) / 2.0f;
  c.y = (r->p1.y + r->p2.y) / 2.0f;

  return c;
}

Ponto calcularCentro(Ponto vertices[], int qtd) {
  Ponto centro;
  int i;

  centro.x = 0.0f;
  centro.y = 0.0f;

  if (qtd <= 0)
    return centro;

  for (i = 0; i < qtd; i++) {
    centro.x += vertices[i].x;
    centro.y += vertices[i].y;
  }

  centro.x /= qtd;
  centro.y /= qtd;

  return centro;
}

void transladarObjeto(CenaGrafica *cena, float tx, float ty) {
  int i;
  Matriz3x3 T = {{{1, 0, tx}, {0, 1, ty}, {0, 0, 1}}};

  for (i = 0; i < cena->qtd_pontos; i++) {
    if (cena->pontos[i].selecionado) {
      cena->pontos[i].posicao = aplicaMatriz(cena->pontos[i].posicao, T);
    }
  }

  for (i = 0; i < cena->qtd_retas; i++) {
    if (cena->retas[i].selecionado) {
      transformarReta(&cena->retas[i], T);
    }
  }

  for (i = 0; i < cena->qtd_poligonos; i++) {
    if (cena->poligonos[i].selecionado) {
      transformarPoligono(&cena->poligonos[i], T);
    }
  }
}

void refletirObjeto(CenaGrafica *cena, EixoReflexao eixo) {

  int i;
  Matriz3x3 R;

  switch (eixo) {
  case REFLEXAO_X:

    R = (Matriz3x3){{{1, 0, 0}, {0, -1, 0}, {0, 0, 1}}};
    break;

  case REFLEXAO_Y:

    R = (Matriz3x3){{{-1, 0, 0}, {0, 1, 0}, {0, 0, 1}}};
    break;

  default:

    R = (Matriz3x3){{{-1, 0, 0}, {0, -1, 0}, {0, 0, 1}}};
    break;
  }

  for (i = 0; i < cena->qtd_pontos; i++) {
    if (cena->pontos[i].selecionado) {
      cena->pontos[i].posicao = aplicaMatriz(cena->pontos[i].posicao, R);
    }
  }

  for (i = 0; i < cena->qtd_retas; i++) {
    if (cena->retas[i].selecionado) {

      Ponto centro = centroReta(&cena->retas[i]);

      Matriz3x3 M = matrizComposta(R, centro);

      transformarReta(&cena->retas[i], M);
    }
  }

  for (i = 0; i < cena->qtd_poligonos; i++) {
    if (cena->poligonos[i].selecionado) {

      Ponto centro = calcularCentro(cena->poligonos[i].vertices,
                                    cena->poligonos[i].qtd_vertices);

      Matriz3x3 M = matrizComposta(R, centro);

      transformarPoligono(&cena->poligonos[i], M);
    }
  }
}

void cisalharObjeto(CenaGrafica *cena, float shx, float shy) {
  int i;

  Matriz3x3 C = {{{1, shx, 0}, {shy, 1, 0}, {0, 0, 1}}};

  for (i = 0; i < cena->qtd_pontos; i++) {
    if (cena->pontos[i].selecionado) {
      cena->pontos[i].posicao = aplicaMatriz(cena->pontos[i].posicao, C);
    }
  }

  for (i = 0; i < cena->qtd_retas; i++) {
    if (cena->retas[i].selecionado) {

      Ponto centro = centroReta(&cena->retas[i]);
      Matriz3x3 M = matrizComposta(C, centro);
      transformarReta(&cena->retas[i], M);
    }
  }

  for (i = 0; i < cena->qtd_poligonos; i++) {
    if (cena->poligonos[i].selecionado) {
      Ponto centro = calcularCentro(cena->poligonos[i].vertices,
                                    cena->poligonos[i].qtd_vertices);

      Matriz3x3 M = matrizComposta(C, centro);
      transformarPoligono(&cena->poligonos[i], M);
    }
  }
}

void rotacionarObjeto(CenaGrafica *cena, float angulo) {
  int i;

  float rad = angulo * M_PI / 180.0f;

  float cosseno = cos(rad);
  float seno = sin(rad);

  Matriz3x3 R = {{{cosseno, -seno, 0}, {seno, cosseno, 0}, {0, 0, 1}}};

  for (i = 0; i < cena->qtd_pontos; i++) {
    if (cena->pontos[i].selecionado) {
      cena->pontos[i].posicao = aplicaMatriz(cena->pontos[i].posicao, R);
    }
  }

  for (i = 0; i < cena->qtd_retas; i++) {
    if (cena->retas[i].selecionado) {
      Ponto centro = centroReta(&cena->retas[i]);

      Matriz3x3 M = matrizComposta(R, centro);

      transformarReta(&cena->retas[i], M);
    }
  }

  for (i = 0; i < cena->qtd_poligonos; i++) {
    Ponto centro;

    if (cena->poligonos[i].selecionado) {
      Ponto centro = calcularCentro(cena->poligonos[i].vertices,
                                    cena->poligonos[i].qtd_vertices);

      Matriz3x3 M = matrizComposta(R, centro);

      transformarPoligono(&cena->poligonos[i], M);
    }
  }
}

void escalarObjeto(CenaGrafica *cena, float sx, float sy) {

  Matriz3x3 S = {{{sx, 0, 0}, {0, sy, 0}, {0, 0, 1}}};

  int i;

  for (i = 0; i < cena->qtd_pontos; i++) {
    if (cena->pontos[i].selecionado) {
      cena->pontos[i].posicao = aplicaMatriz(cena->pontos[i].posicao, S);
    }
  }

  for (i = 0; i < cena->qtd_retas; i++) {
    if (cena->retas[i].selecionado) {
      Ponto centro = centroReta(&cena->retas[i]);

      Matriz3x3 M = matrizComposta(S, centro);

      transformarReta(&cena->retas[i], M);
    }
  }

  for (i = 0; i < cena->qtd_poligonos; i++) {
    if (cena->poligonos[i].selecionado) {
      int j;

      Ponto centro = calcularCentro(cena->poligonos[i].vertices,
                                    cena->poligonos[i].qtd_vertices);

      Matriz3x3 M = matrizComposta(S, centro);

      transformarPoligono(&cena->poligonos[i], M);
    }
  }
}
