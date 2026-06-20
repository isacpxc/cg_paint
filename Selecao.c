#include "Estruturas.h"
#include <stdio.h>
#include <math.h>
#include <GL/glut.h>

#define TOLERANCIA 10.0f

int larguraJanela = 800;
int alturaJanela = 600;
ContextoPaint contexto;


void desenharCena(CenaGrafica *cena){
    for( int i = 0; i < cena -> qtd_pontos; i++){
        PontoDesenho *p = &cena -> pontos[i];

    if(p -> selecionado){
        glColor3f(1.0f, 1.0f, 0.0f);
        glPointSize(p -> tamanho + 4.0f);
    } else{
        glColor3f(p -> cor[0], p -> cor[1], p -> cor[2]);
        glPointSize(p -> tamanho + 4.0f);
    }

        glBegin(GL_POINTS);
           glVertex2f(p -> posicao.x, p -> posicao.y);
        glEnd();
    }

    for( int i = 0; i < cena -> qtd_retas; i++){
        Reta *r = &cena -> retas[i];

        if( r -> selecionado){
            glColor3f(1.0f, 1.0f, 0.0f);
            glLineWidth(r -> espessura + 2.0f);
        } else {
            glColor3f(r -> cor[0], r -> cor[1], r -> cor[2]);
            glLineWidth(r -> espessura);
        }

        glBegin(GL_LINES);
            glVertex2f(r -> p1.x, r -> p1.y);
            glVertex2f(r -> p2.x, r -> p2.y);
        glEnd();
    }

    for( int i = 0; i < cena -> qtd_poligonos; i++){
        Poligono *p = &cena -> poligonos[i];

        if(p -> preenchido){
            glColor3f(1.0f, 1.0f, 0.0f);
        } else{
            glColor3f(p -> cor_preenchimento[0], p -> cor_preenchimento[1],  p -> cor_preenchimento[2]);
        }

        glBegin(GL_POLYGON);
           for( int j = 0; j < p -> qtd_vertices; j++){
               glVertex2f(p -> vertices[j].x, p -> vertices[j].y);
           }

           glEnd();

        if(p -> selecionado){
            glColor3f(1.0f, 1.0f, 0.0f);
            glLineWidth(p -> espessura_contorno + 2.0f);
        } else{
            glColor3f(p -> cor_contorno[0], p -> cor_contorno[1], p -> cor_contorno[2]);
            glLineWidth(p -> espessura_contorno);
        }

        glBegin(GL_LINE_LOOP);
           for(int j = 0; j < p -> qtd_vertices; j++){
               glVertex2f(p -> vertices[j].x, p -> vertices[j].y);
           }
           glEnd();
    }
}


Ponto windowParaGL(int mx, int my){
    Ponto v;
    v.x = (2.0f * mx / larguraJanela) - 1.0f;
    v.y = 1.0f - (2.0f * my / alturaJanela);
    return v;
}


float distanciaPixels(Ponto a, Ponto b){
    float dx = (a.x - b.x) * larguraJanela / 2.0f;
    float dy = (a.y - b.y) * alturaJanela / 2.0f;
    return sqrtf(dx * dx + dy * dy);
}

float distPontosSegmentos(Ponto p, Ponto a, Ponto b){
    float ax = (a.x + 1.0f) * larguraJanela / 2.0f;
    float ay = (1.0f - a.y) * alturaJanela / 2.0f;
    float bx = (b.x + 1.0f) * larguraJanela / 2.0f;
    float by = (1.0f - b.y) * alturaJanela / 2.0f;
    float px = (p.x + 1.0f) * larguraJanela / 2.0f;
    float py = (1.0f - p.y) * alturaJanela / 2.0f;

    float dx = bx - ax;
    float dy = by - ay;
    float len2 = dx * dx + dy * dy;

    if(len2 == 0.0f){
       return sqrtf((px - ax) * (px - ax) + (py - ay) * (py - ay));
    }

    float t =((px - ax) * dx + (py - ay) *dy) / len2;
    if(t < 0.0f) t = 0.0f;
    if( t > 1.0f) t = 1.0f;

    float projx = ax + t * dx;
    float projy = ay + t * dy;

    return sqrtf((px - projx) * (px - projx) + (py - projy) * (py - projy));
}

int pontoNoPoligono(Ponto p, Ponto *vertices, int n){
    int dentro = 0;

    for(int i = 0, j = n - 1; i < n; j = i++){
        float xi = vertices[i].x;
        float yi = vertices[i].y;
        float xj = vertices[j].x;
        float yj = vertices[j].y;

        int cruza = ((yi > p.y) != (yj > p.y)) && (p.x < (xj - xi) * (p.y - yi) / (yj - yi) + xi);
        if(cruza) dentro  = !dentro;
    }

    return dentro;
}

int clicouNoPonto(PontoDesenho *ponto, Ponto clique){
    return distanciaPixels(clique, ponto -> posicao) <= TOLERANCIA;
}

int clicouNaReta(Reta *reta, Ponto clique){
    return distPontosSegmentos(clique, reta -> p1, reta -> p2) <= TOLERANCIA;
}


int clicouNoPoligono(Poligono *poligono, Ponto clique){
    for( int i = 0; i < poligono -> qtd_vertices; i++){
        int j = ( i + 1) % poligono -> qtd_vertices;
        if(distPontosSegmentos(clique, poligono -> vertices[i], poligono -> vertices[j]) <= TOLERANCIA){
            return 1;
        }
    }

    if (poligono->preenchido) {
        return pontoNoPoligono(clique, poligono->vertices, poligono->qtd_vertices);
    }
    
    return 0;
    
}


void selecionarObjeto(CenaGrafica *cena, float mouseX, float mouseY){
    Ponto clique = windowParaGL(mouseX, mouseY);

    for(int i = 0; i < cena -> qtd_pontos; i++){
       if(clicouNoPonto(&cena -> pontos[i], clique)){
          cena -> pontos[i].selecionado = !cena -> pontos[i].selecionado;
       }
    }



    for(int i = 0; i < cena -> qtd_retas; i++){
       if(clicouNaReta(&cena -> retas[i], clique)){
          cena -> retas[i].selecionado = !cena -> retas[i].selecionado;
       }
    }

    for(int i = 0; i < cena -> qtd_poligonos; i++){
       if(clicouNoPoligono(&cena -> poligonos[i], clique)){
          cena -> poligonos[i].selecionado = !cena -> poligonos[i].selecionado;
       }
    } 
}

void excluirObjetosSelecionados(CenaGrafica *cena){
    int j = 0;
    for(int i = 0; i < cena -> qtd_pontos; i++){
        if(!cena -> pontos[i].selecionado){
            cena -> pontos[j] = cena -> pontos[i];
            j++;
        }
    }
    cena -> qtd_pontos = j;

    j = 0;
    for(int i = 0; i < cena -> qtd_retas; i++){
        if(!cena -> retas[i].selecionado){
            cena -> retas[j] = cena -> retas[i];
            j++;
        }
    }
    cena -> qtd_retas = j;

    j = 0;
    for(int i = 0; i < cena -> qtd_poligonos; i++){
        if(!cena -> poligonos[i].selecionado){
            cena -> poligonos[j] = cena -> poligonos[i];
            j++;
        }
    }
    cena -> qtd_poligonos = j;
}


void desselecionarTodos(CenaGrafica *cena){
    for(int i = 0; i < cena -> qtd_pontos; i++){
        cena -> pontos[i].selecionado = 0;
    }

    for(int i = 0; i < cena -> qtd_retas; i++){
        cena -> retas[i].selecionado = 0;
    }

    for(int i = 0; i < cena -> qtd_poligonos; i++){
        cena -> poligonos[i].selecionado = 0;
    }
}

