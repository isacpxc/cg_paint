// trnasformacoes.c
//responsável: Kallyne Lopes Taveira
// modulo de criação das funções de tranformações geométricas

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Estruturas.h"


// =============================================================================
// aplicaMatriz
// -----------------------------------------------------------------------------
// Aplica uma matriz de transformação homogênea 3x3 a um ponto 2D.
//
// Esta função é utilizada internamente pelas transformações geométricas.
//
// Parâmetros:
//   p - ponto a ser transformado
//   m - matriz de transformação
//
// Retorno:
//   Novo ponto após a aplicação da matriz.
// =============================================================================

static Ponto aplicaMatriz(Ponto p, Matriz3x3 m)
{
    Ponto resultado;

    resultado.x = m.m[0][0] * p.x +
                m.m[0][1] * p.y +
                m.m[0][2];

    resultado.y = m.m[1][0] * p.x +
                m.m[1][1] * p.y +
                m.m[1][2];

    return resultado;
}
// =============================================================================
// multiplicaMatriz
// -----------------------------------------------------------------------------
// Realiza a multiplicação entre duas matrizes homogêneas 3x3.
//
// Esta função é utilizada para construir transformações compostas,
// combinando duas matrizes de transformação em uma única matriz.
//
// Parâmetros:
//   A - primeira matriz
//   B - segunda matriz
//
// Retorno:
//   Matriz resultante da multiplicação A x B.
// =============================================================================
static Matriz3x3 multiplicaMatriz(Matriz3x3 A, Matriz3x3 B)
{
    Matriz3x3 R;
    int i,j,k;

    for(i=0;i<3;i++)
    {
        for(j=0;j<3;j++)
        {
            R.m[i][j]=0;

            for(k=0;k<3;k++)
                R.m[i][j]+=A.m[i][k]*B.m[k][j];
        }
    }

    return R;
}

// =============================================================================
// matrizTranslacao
// -----------------------------------------------------------------------------
// Cria uma matriz homogênea de translação.
//
// Esta função é utilizada para deslocar objetos ou construir
// transformações compostas.
//
// Parâmetros:
//   tx - deslocamento no eixo X
//   ty - deslocamento no eixo Y
//
// Retorno:
//   Matriz homogênea de translação.
// =============================================================================
static Matriz3x3 matrizTranslacao(float tx,float ty)
{
    Matriz3x3 T = {{
        {1,0,tx},
        {0,1,ty},
        {0,0,1}
    }};

    return T;
}


// =============================================================================
// matrizComposta
// -----------------------------------------------------------------------------
// Constrói uma transformação composta em torno do centro de um objeto.
//
// A transformação é composta por três etapas:
//
//   1) Translada o objeto para a origem;
//   2) Aplica a transformação desejada;
//   3) Retorna o objeto para sua posição original.
//
// Matematicamente:
//
//      M = T(C) · M · T(-C)
//
// onde:
//   T(-C) -> leva o centro para a origem;
//   M     -> transformação principal;
//   T(C)  -> retorna o objeto ao centro.
//
// Parâmetros:
//   M      - matriz da transformação principal
//   centro - centro geométrico do objeto
//
// Retorno:
//   Matriz composta correspondente.
// =============================================================================

static Matriz3x3 matrizComposta(Matriz3x3 M, Ponto centro)
{
    Matriz3x3 T1 = matrizTranslacao(-centro.x,-centro.y);
    Matriz3x3 T2 = matrizTranslacao( centro.x, centro.y);

    Matriz3x3 Aux;

    Aux = multiplicaMatriz(M,T1);

    return multiplicaMatriz(T2,Aux);
}

// =============================================================================
// transformarReta
// -----------------------------------------------------------------------------
// Aplica uma matriz de transformação aos dois extremos de uma reta.
//
// Função auxiliar utilizada pelas operações geométricas.
//
// Parâmetros:
//   r - reta a ser transformada
//   m - matriz de transformação
//
// Retorno:
//   Nenhum.
// =============================================================================

static void transformarReta(Reta *r, Matriz3x3 m){

    r->p1 = aplicaMatriz(r->p1, m);
    r->p2 = aplicaMatriz(r->p2, m);
}

// =============================================================================
// transformarPoligono
// -----------------------------------------------------------------------------
// Aplica uma matriz de transformação a todos os vértices de um polígono.
//
// Função auxiliar utilizada pelas operações geométricas.
//
// Parâmetros:
//   p - polígono a ser transformado
//   m - matriz de transformação
//
// Retorno:
//   Nenhum.
// =============================================================================

static void transformarPoligono(Poligono *p, Matriz3x3 m){
    int i;

    for(i=0; i< p->qtd_vertices; i++){
        p->vertices[i] = aplicaMatriz(p->vertices[i], m);
    }
}

// =============================================================================
// centroReta
// -----------------------------------------------------------------------------
// Calcula o ponto médio de uma reta.
//
// Utilizado como centro de referência para operações de rotação e escala.
//
// Parâmetros:
//   r - reta analisada
//
// Retorno:
//   Ponto correspondente ao centro da reta.
// =============================================================================

static Ponto centroReta(Reta *r){
    Ponto c;

    c.x = (r->p1.x + r->p2.x)/ 2.0f;
    c.y = (r->p1.y + r->p2.y)/ 2.0f;

    return c;
}

// =============================================================================
// calcularCentro
// -----------------------------------------------------------------------------
// Calcula o centro geométrico (centroide) de um conjunto de vértices.
//
// Utilizado principalmente nas operações de rotação e escala de polígonos,
// garantindo que a transformação ocorra em torno do próprio objeto.
//
// Parâmetros:
//   vertices - vetor contendo os vértices do polígono
//   qtd      - quantidade de vértices presentes no vetor
//
// Retorno:
//   Estrutura Ponto contendo as coordenadas do centro calculado.
// =============================================================================

Ponto calcularCentro(Ponto vertices[], int qtd){
    Ponto centro;
    int i;

    centro.x = 0.0f;
    centro.y = 0.0f;

    if(qtd <= 0)
        return centro;

    for(i = 0; i < qtd; i++)
    {
        centro.x += vertices[i].x;
        centro.y += vertices[i].y;
    }

    centro.x /= qtd;
    centro.y /= qtd;

    return centro;
}

// =============================================================================
// transladarObjeto
// -----------------------------------------------------------------------------
// Aplica uma transformação de translação a todos os objetos selecionados.
//
// A translação desloca os objetos nas direções X e Y sem alterar sua forma,
// orientação ou tamanho.
//
// Parâmetros:
//   cena - ponteiro para a CenaGrafica
//   tx   - deslocamento no eixo X
//   ty   - deslocamento no eixo Y
//
// Retorno:
//   Nenhum.
// =============================================================================

void transladarObjeto(CenaGrafica *cena, float tx, float ty){
   int i;
   Matriz3x3 T = {{{1,0,tx},
                {0,1,ty},
                {0,0,1}}};

    for(i=0; i < cena->qtd_pontos; i++){
        if(cena->pontos[i].selecionado){
            cena->pontos[i].posicao =
            aplicaMatriz(cena->pontos[i].posicao, T);
        }
    }

    for(i=0; i< cena->qtd_retas; i++){
        if(cena->retas[i].selecionado){
            transformarReta(&cena->retas[i], T);
        }
    }

    for(i=0; i< cena->qtd_poligonos; i++){
        if(cena->poligonos[i].selecionado){
            transformarPoligono(&cena->poligonos[i], T);
        }
    }
}

// =============================================================================
// refletirObjeto
// -----------------------------------------------------------------------------
// Aplica uma reflexão aos objetos selecionados.
//
// A reflexão pode ocorrer em relação ao eixo X, eixo Y ou à origem,
// dependendo do valor recebido no parâmetro eixo.
//
// Parâmetros:
//   cena - ponteiro para a CenaGrafica
//   eixo - tipo de reflexão (REFLEXAO_X, REFLEXAO_Y ou REFLEXAO_ORIGEM)
//
// Retorno:
//   Nenhum.
// =============================================================================

void refletirObjeto(CenaGrafica *cena, EixoReflexao eixo){

    int i;
    Matriz3x3 R;

    switch(eixo)
    {
    case REFLEXAO_X:

         R = (Matriz3x3){{
             {1,0,0},
             {0,-1,0},
             {0,0,1}}};
        break;

    case REFLEXAO_Y:

         R = (Matriz3x3){{
            {-1,0,0},
            {0,1,0},
            {0,0,1}}};
        break;

    default:

         R = (Matriz3x3){{
            {-1,0,0},
            {0,-1,0},
            {0,0,1}}};
        break;
    }

    for(i=0; i < cena->qtd_pontos; i++){
        if(cena->pontos[i].selecionado){
            cena->pontos[i].posicao =
            aplicaMatriz(cena->pontos[i].posicao, R);
        }
    }

    for(i = 0; i < cena->qtd_retas; i++){
    if(cena->retas[i].selecionado){

        Ponto centro = centroReta(&cena->retas[i]);

        Matriz3x3 M = matrizComposta(R, centro);

        transformarReta(&cena->retas[i], M);
    }
}

    for(i = 0; i < cena->qtd_poligonos; i++){
    if(cena->poligonos[i].selecionado){

        Ponto centro = calcularCentro(
            cena->poligonos[i].vertices,
            cena->poligonos[i].qtd_vertices
        );

        Matriz3x3 M = matrizComposta(R, centro);

        transformarPoligono(&cena->poligonos[i], M);
        }
    }
}
// =============================================================================
// cisalharObjeto
// -----------------------------------------------------------------------------
// Aplica uma transformação de cisalhamento aos objetos selecionados.
//
// O cisalhamento inclina a geometria do objeto sem alterar sua área,
// utilizando fatores independentes para os eixos X e Y.
//
// Parâmetros:
//   cena - ponteiro para a CenaGrafica
//   shx  - fator de cisalhamento horizontal
//   shy  - fator de cisalhamento vertical
//
// Retorno:
//   Nenhum.
// =============================================================================

void cisalharObjeto(CenaGrafica *cena, float shx, float shy){
    int i;

    Matriz3x3 C = {{{1,shx,0},
                {shy,1,0},
                {0,0,1}}};

    for(i=0; i < cena->qtd_pontos; i++){
        if(cena->pontos[i].selecionado){
            cena->pontos[i].posicao =
            aplicaMatriz(cena->pontos[i].posicao, C);
        }
    }

    for(i=0; i< cena->qtd_retas; i++){
        if(cena->retas[i].selecionado){

            Ponto centro = centroReta(&cena->retas[i]);
            Matriz3x3 M = matrizComposta(C, centro);
            transformarReta(&cena->retas[i], M);
        }

        }

    for(i=0; i< cena->qtd_poligonos; i++){
        if(cena->poligonos[i].selecionado){
            Ponto centro = calcularCentro(
            cena->poligonos[i].vertices,
            cena->poligonos[i].qtd_vertices
        );

            Matriz3x3 M = matrizComposta(C, centro);
            transformarPoligono(&cena->poligonos[i], M);
            }
        }
    }

// =============================================================================
// rotacionarObjeto
// -----------------------------------------------------------------------------
// Aplica uma rotação aos objetos selecionados.
//
// Pontos são rotacionados em torno da origem.
// Retas e polígonos são rotacionados em torno de seus respectivos centros.
//
// Parâmetros:
//   cena   - ponteiro para a CenaGrafica
//   angulo - ângulo da rotação em graus
//
// Retorno:
//   Nenhum.
// =============================================================================

void rotacionarObjeto(CenaGrafica *cena, float angulo){
    int i;

    float rad = angulo * M_PI / 180.0f;

    float cosseno = cos(rad);
    float seno = sin(rad);

    Matriz3x3 R = {{
    {cosseno, -seno, 0},
    {seno, cosseno, 0},
    {0, 0,1}}};

    //pontos
    for(i = 0; i < cena->qtd_pontos; i++){
    if(cena->pontos[i].selecionado){
        cena->pontos[i].posicao =
            aplicaMatriz(cena->pontos[i].posicao, R);
    }
}
    //retas
    for(i=0; i< cena->qtd_retas; i++){
        if(cena->retas[i].selecionado){
            Ponto centro = centroReta(&cena->retas[i]);

            Matriz3x3 M = matrizComposta(R, centro);

            transformarReta(&cena->retas[i], M);
        }
    }

    //poligonos
    for(i=0; i<cena->qtd_poligonos; i++){
        Ponto centro;

        if(cena->poligonos[i].selecionado){
        Ponto centro = calcularCentro(
        cena->poligonos[i].vertices,
        cena->poligonos[i].qtd_vertices
    );

        Matriz3x3 M = matrizComposta(R, centro);

        transformarPoligono(&cena->poligonos[i], M);
        }
    }
}

// =============================================================================
// escalarObjeto
// -----------------------------------------------------------------------------
// Aplica uma transformação de escala aos objetos selecionados.
//
// A escala é realizada em torno do centro geométrico do objeto,
// preservando sua posição geral na cena.
//
// Parâmetros:
//   cena - ponteiro para a CenaGrafica
//   sx   - fator de escala horizontal
//   sy   - fator de escala vertical
//
// Retorno:
//   Nenhum.
// =============================================================================

void escalarObjeto(CenaGrafica *cena, float sx, float sy){
    int i;

    for(i=0;i<cena->qtd_retas;i++){
        if(cena->retas[i].selecionado){
            Ponto c = centroReta(&cena->retas[i]);

            cena->retas[i].p1.x =
                c.x + (cena->retas[i].p1.x - c.x)*sx;

            cena->retas[i].p1.y =
                c.y + (cena->retas[i].p1.y - c.y)*sy;

            cena->retas[i].p2.x =
                c.x + (cena->retas[i].p2.x - c.x)*sx;

            cena->retas[i].p2.y =
                c.y + (cena->retas[i].p2.y - c.y)*sy;
        }
    }

    for(i=0;i<cena->qtd_poligonos;i++){
        if(cena->poligonos[i].selecionado){
            int j;

            Ponto c = calcularCentro(
                    cena->poligonos[i].vertices,
                    cena->poligonos[i].qtd_vertices
                );

            for(j=0;j<cena->poligonos[i].qtd_vertices;j++){
                cena->poligonos[i].vertices[j].x = c.x +
                    (cena->poligonos[i].vertices[j].x - c.x)*sx;

                cena->poligonos[i].vertices[j].y = c.y +
                    (cena->poligonos[i].vertices[j].y - c.y)*sy;
            }
        }
    }
}



