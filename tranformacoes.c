#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Estruturas.h"


        // FUNÇÃO AUXILIARES

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

static void transformarReta(Reta *r, Matriz3x3 m){
    int i;

    r->p1 = aplicaMatriz(r->p1, m);
    r->p2 = aplicaMatriz(r->p2, m);
}

static void transformarPoligono(Poligono *p, Matriz3x3 m){
    int i;

    for(i=0; i< p->qtd_vertices; i++){
        p->vertices[i] = aplicaMatriz(p->vertices[i], m);
    }
}

static Ponto centroReta(Reta *r){
    Ponto c;

    c.x = (r->p1.x + r->p1.y)/ 2.0f;
    c.y = (r->p2.x + r->p2.y)/ 2.0f;

    return c;
}

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


        // TRANSLAÇÃO

void transladarObjeto(CenaGrafica *cena, float tx, float ty)
{
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

        // REFLEXÃO

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

    for(i=0; i< cena->qtd_retas; i++){
        if(cena->retas[i].selecionado){
            transformarReta(&cena->retas[i], R);
        }
    }

    for(i=0; i< cena->qtd_poligonos; i++){
        if(cena->poligonos[i].selecionado){
            transformarPoligono(&cena->poligonos[i], R);
            }
        }
    }


        // CISALHAMENTO

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
            transformarReta(&cena->retas[i], C);
        }
    }

    for(i=0; i< cena->qtd_poligonos; i++){
        if(cena->poligonos[i].selecionado){
            transformarPoligono(&cena->poligonos[i], C);
            }
        }
    }

        // ROTAÇÃO

void rotacionarObjeto(CenaGrafica *cena, float angulo){
    int i;

    float rad = angulo * M_PI / 180.0f;

    float cosseno = cos(rad);
    float seno = sin(rad);

    for(i=0; i< cena->qtd_pontos; i++){
        if(cena->pontos[i].selecionado){
            float x = cena->pontos[i].posicao.x;
            float y = cena->pontos[i].posicao.y;

            cena->pontos[i].posicao.x = x*cosseno - y*seno;
            cena->pontos[i].posicao.y = x*seno + y*cosseno;
        }
    }

    for(i=0; i< cena->qtd_retas; i++){
        if(cena->retas[i].selecionado){
            Ponto centro = centroReta(&cena->retas[i]);

            Ponto *pts[2] = {
                &cena->retas[i].p1,
                &cena->retas[i].p2
        };

    int k;
    for(k=0; k<2; k++){
        float x = pts[k]->x - centro.x;
        float y = pts[k]->y - centro.y;

        pts[k]->x = centro.x + x*cosseno -y*seno;
        pts[k]->y = centro.y + x*seno + y*cosseno;
        }
    }
}

    for(i=0; i<cena->qtd_poligonos; i++){
        Ponto centro;
        if(cena->poligonos[i].selecionado){
            centro = calcularCentro
            (cena->poligonos[i].vertices,
            cena->poligonos[i].qtd_vertices);
        }
    int j;
    for(j=0; j< cena->poligonos[i].qtd_vertices; j++){
        float x = cena->poligonos[i].vertices[j].x - centro.x;
        float y = cena->poligonos[i].vertices[j].y - centro.y;

        cena->poligonos[i].vertices[j].x = centro.x + x*cosseno - y*seno;
        cena->poligonos[i].vertices[j].y = centro.y + x*seno + y*cosseno;
            }
        }
    }

        // ESCALA

void escalarObjeto(CenaGrafica *cena, float sx, float sy){
    int i;

    for(i=0;i<cena->qtd_retas;i++)
    {
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

    for(i=0;i<cena->qtd_poligonos;i++)
    {
        if(cena->poligonos[i].selecionado)
        {
            int j;

            Ponto c = calcularCentro(
                    cena->poligonos[i].vertices,
                    cena->poligonos[i].qtd_vertices
                );

            for(j=0;j<cena->poligonos[i].qtd_vertices;j++)
            {
                cena->poligonos[i].vertices[j].x = c.x +
                    (cena->poligonos[i].vertices[j].x - c.x)*sx;

                cena->poligonos[i].vertices[j].y = c.y +
                    (cena->poligonos[i].vertices[j].y - c.y)*sy;
            }
        }
    }
}



