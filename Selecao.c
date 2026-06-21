#include "selecao.h"
#include <math.h>
#include <GL/glut.h>

#define TOLERANCIA 10.0f



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
    v.x = (float)mx;
    v.y = (float)my;
    return v;
}


#define CS_ESQUERDA 1 // 0001
#define CS_DIREITA  2 // 0010
#define CS_ABAIXO   4 // 0100
#define CS_ACIMA    8 // 1000

int calcularCodigoCS(float x, float y, float mx, float my, float tol) {
    int codigo = 0;
    if (x < mx - tol)      codigo |= CS_ESQUERDA;
    else if (x > mx + tol) codigo |= CS_DIREITA;
    
    if (y < my - tol)      codigo |= CS_ABAIXO;
    else if (y > my + tol) codigo |= CS_ACIMA;
    return codigo;
}

int cohenSutherlandIntercepta(Ponto p1, Ponto p2, Ponto clique, float tol) {
    float mx = clique.x;
    float my = clique.y;
    float x1 = p1.x, y1 = p1.y;
    float x2 = p2.x, y2 = p2.y;
    
    int code1 = calcularCodigoCS(x1, y1, mx, my, tol);
    int code2 = calcularCodigoCS(x2, y2, mx, my, tol);
    int aceito = 0;

    while (1) {
        if ((code1 == 0) && (code2 == 0)) {
            // Caso trivial: aceita (linha intercepta a caixa)
            aceito = 1;
            break;
        } else if (code1 & code2) {
            // Caso trivial: rejeita (linha totalmente fora da caixa)
            break;
        } else {
            // Caso nao trivial: calcular intersecao
            int code_fora = code1 ? code1 : code2;
            float x = 0, y = 0;

            if (code_fora & CS_ACIMA) {
                x = x1 + (x2 - x1) * ((my + tol) - y1) / (y2 - y1);
                y = my + tol;
            } else if (code_fora & CS_ABAIXO) {
                x = x1 + (x2 - x1) * ((my - tol) - y1) / (y2 - y1);
                y = my - tol;
            } else if (code_fora & CS_DIREITA) {
                y = y1 + (y2 - y1) * ((mx + tol) - x1) / (x2 - x1);
                x = mx + tol;
            } else if (code_fora & CS_ESQUERDA) {
                y = y1 + (y2 - y1) * ((mx - tol) - x1) / (x2 - x1);
                x = mx - tol;
            }

            if (code_fora == code1) {
                x1 = x;
                y1 = y;
                code1 = calcularCodigoCS(x1, y1, mx, my, tol);
            } else {
                x2 = x;
                y2 = y;
                code2 = calcularCodigoCS(x2, y2, mx, my, tol);
            }
        }
    }
    return aceito;
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
    if (clique.x <= ponto->posicao.x + TOLERANCIA && clique.x >= ponto->posicao.x - TOLERANCIA) {
        if (clique.y <= ponto->posicao.y + TOLERANCIA && clique.y >= ponto->posicao.y - TOLERANCIA) {
            return 1;
        }
    }
    return 0;
}

int clicouNaReta(Reta *reta, Ponto clique){
    return cohenSutherlandIntercepta(reta->p1, reta->p2, clique, TOLERANCIA);
}


int clicouNoPoligono(Poligono *poligono, Ponto clique){
    for( int i = 0; i < poligono -> qtd_vertices; i++){
        int j = ( i + 1) % poligono -> qtd_vertices;
        if(cohenSutherlandIntercepta(poligono->vertices[i], poligono->vertices[j], clique, TOLERANCIA)){
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

void excluirObjetoSelecionado(CenaGrafica *cena, float mouseX, float mouseY) {
    Ponto clique = windowParaGL(mouseX, mouseY);

    for (int i = 0; i < cena->qtd_pontos; i++) {
        if (clicouNoPonto(&cena->pontos[i], clique)) {
            for (int j = i; j < cena->qtd_pontos - 1; j++) {
                cena->pontos[j] = cena->pontos[j + 1];
            }
            cena->qtd_pontos--;
            return;
        }
    }

    for (int i = 0; i < cena->qtd_retas; i++) {
        if (clicouNaReta(&cena->retas[i], clique)) {
            for (int j = i; j < cena->qtd_retas - 1; j++) {
                cena->retas[j] = cena->retas[j + 1];
            }
            cena->qtd_retas--;
            return;
        }
    }

    for (int i = 0; i < cena->qtd_poligonos; i++) {
        if (clicouNoPoligono(&cena->poligonos[i], clique)) {
            for (int j = i; j < cena->qtd_poligonos - 1; j++) {
                cena->poligonos[j] = cena->poligonos[j + 1];
            }
            cena->qtd_poligonos--;
            return;
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

