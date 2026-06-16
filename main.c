#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Estruturas.h"

// ============================================================================
// VARIAVEL GLOBAL - Contexto do Paint
// ============================================================================
ContextoPaint ctx;

// Largura e altura da janela
#define LARGURA_JANELA 800
#define ALTURA_JANELA  600

// Paleta de cores disponiveis
float paleta[][3] = {
    {1.0f, 0.0f, 0.0f},   // 0 - Vermelho
    {0.0f, 1.0f, 0.0f},   // 1 - Verde
    {0.0f, 0.0f, 1.0f},   // 2 - Azul
    {1.0f, 1.0f, 0.0f},   // 3 - Amarelo
    {1.0f, 0.0f, 1.0f},   // 4 - Magenta
    {0.0f, 1.0f, 1.0f},   // 5 - Ciano
    {1.0f, 1.0f, 1.0f},   // 6 - Branco
    {1.0f, 0.5f, 0.0f},   // 7 - Laranja
};
const char* nomes_cores[] = {
    "VERMELHO", "VERDE", "AZUL", "AMARELO", "MAGENTA", "CIANO", "BRANCO", "LARANJA"
};
int total_cores = 8;
int indice_cor = 0;

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    for (int i = 0; i < ctx.cena.qtd_pontos; i++) {
        PontoDesenho *p = &ctx.cena.pontos[i];
        glPointSize(p->tamanho);
        glColor3f(p->cor[0], p->cor[1], p->cor[2]);
        glBegin(GL_POINTS);
            glVertex2f(p->posicao.x, p->posicao.y);
        glEnd();
    }

    for (int i = 0; i < ctx.cena.qtd_retas; i++) {
        Reta *r = &ctx.cena.retas[i];
        glLineWidth(r->espessura);
        glColor3f(r->cor[0], r->cor[1], r->cor[2]);
        glBegin(GL_LINES);
            glVertex2f(r->p1.x, r->p1.y);
            glVertex2f(r->p2.x, r->p2.y);
        glEnd();
    }

    for (int i = 0; i < ctx.cena.qtd_poligonos; i++) {
        Poligono *pol = &ctx.cena.poligonos[i];

        if (pol->preenchido) {
            glColor3f(pol->cor_preenchimento[0], pol->cor_preenchimento[1], pol->cor_preenchimento[2]);
            glBegin(GL_POLYGON);
            for (int j = 0; j < pol->qtd_vertices; j++) {
                glVertex2f(pol->vertices[j].x, pol->vertices[j].y);
            }
            glEnd();
        }

        glLineWidth(pol->espessura_contorno);
        glColor3f(pol->cor_contorno[0], pol->cor_contorno[1], pol->cor_contorno[2]);
        glBegin(GL_LINE_LOOP);
        for (int j = 0; j < pol->qtd_vertices; j++) {
            glVertex2f(pol->vertices[j].x, pol->vertices[j].y);
        }
        glEnd();
    }

    glColor3f(ctx.ui.cor_atual[0], ctx.ui.cor_atual[1], ctx.ui.cor_atual[2]);
    glBegin(GL_QUADS);
        glVertex2f(10, 10);
        glVertex2f(30, 10);
        glVertex2f(30, 30);
        glVertex2f(10, 30);
    glEnd();

    glColor3f(1.0f, 1.0f, 1.0f);
    glLineWidth(1.0f);
    glBegin(GL_LINE_LOOP);
        glVertex2f(10, 10);
        glVertex2f(30, 10);
        glVertex2f(30, 30);
        glVertex2f(10, 30);
    glEnd();

    const char* modo_texto = "";
    switch (ctx.ui.ferramenta_atual) {
        case MODO_PONTO:    modo_texto = "[PONTO]";    break;
        case MODO_RETA:     modo_texto = "[RETA]";     break;
        case MODO_POLIGONO: modo_texto = "[POLIGONO]"; break;
        case MODO_SELECAO:  modo_texto = "[SELECAO]";  break;
        case MODO_BORRACHA: modo_texto = "[BORRACHA]"; break;
    }
    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2f(40, 16);
    for (const char* c = modo_texto; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *c);
    }

    glutSwapBuffers();
}

void mouse(int botao, int estado, int x, int y) {
    if (estado != GLUT_DOWN) return;

    float mx = (float)x;
    float my = (float)(glutGet(GLUT_WINDOW_HEIGHT) - y);

    if (botao == GLUT_LEFT_BUTTON) {
        switch (ctx.ui.ferramenta_atual) {
            case MODO_PONTO:
                if (adicionarPonto(&ctx.cena, mx, my, ctx.ui.cor_atual, ctx.ui.espessura_atual)) {
                    printf("Ponto #%d em (%.0f, %.0f)\n", ctx.cena.qtd_pontos, mx, my);
                }
                break;

            case MODO_RETA:
                printf("[RETA] Clique em (%.0f, %.0f) - ainda nao implementado\n", mx, my);
                break;

            case MODO_POLIGONO:
                printf("[POLIGONO] Clique em (%.0f, %.0f) - ainda nao implementado\n", mx, my);
                break;

            case MODO_SELECAO:
                // TODO: selecionarObjeto (modulo do Felipe)
                printf("[SELECAO] Clique em (%.0f, %.0f) - ainda nao implementado\n", mx, my);
                break;

            case MODO_BORRACHA:
                // TODO: excluirObjeto (modulo do Felipe)
                printf("[BORRACHA] Clique em (%.0f, %.0f) - ainda nao implementado\n", mx, my);
                break;
        }
        glutPostRedisplay();
    }
}


void teclado(unsigned char tecla, int x, int y) {
    switch (tecla) {
        case '1':
            ctx.ui.ferramenta_atual = MODO_PONTO;
            printf(">> Ferramenta: PONTO\n");
            break;
        case '2':
            ctx.ui.ferramenta_atual = MODO_RETA;
            printf(">> Ferramenta: RETA\n");
            break;
        case '3':
            ctx.ui.ferramenta_atual = MODO_POLIGONO;
            printf(">> Ferramenta: POLIGONO\n");
            break;
        case '4':
            ctx.ui.ferramenta_atual = MODO_SELECAO;
            printf(">> Ferramenta: SELECAO\n");
            break;
        case '5':
            ctx.ui.ferramenta_atual = MODO_BORRACHA;
            printf(">> Ferramenta: BORRACHA\n");
            break;

        case 'c':
        case 'C':
            indice_cor = (indice_cor + 1) % total_cores;
            ctx.ui.cor_atual[0] = paleta[indice_cor][0];
            ctx.ui.cor_atual[1] = paleta[indice_cor][1];
            ctx.ui.cor_atual[2] = paleta[indice_cor][2];
            printf(">> Cor: %s\n", nomes_cores[indice_cor]);
            glutPostRedisplay();
            break;

        case '+':
            ctx.ui.espessura_atual += 2.0f;
            if (ctx.ui.espessura_atual > 50.0f) ctx.ui.espessura_atual = 50.0f;
            printf(">> Espessura: %.0f\n", ctx.ui.espessura_atual);
            break;
        case '-':
            ctx.ui.espessura_atual -= 2.0f;
            if (ctx.ui.espessura_atual < 2.0f) ctx.ui.espessura_atual = 2.0f;
            printf(">> Espessura: %.0f\n", ctx.ui.espessura_atual);
            break;

        case 'l':
        case 'L':
            memset(&ctx.cena, 0, sizeof(CenaGrafica));
            printf(">> Tela limpa!\n");
            glutPostRedisplay();
            break;

        case 27:
            exit(0);
            break;
    }
}


void redimensionar(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, w, 0, h);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int main(int argc, char** argv) {

    memset(&ctx, 0, sizeof(ContextoPaint));
    ctx.ui.ferramenta_atual = MODO_PONTO;
    ctx.ui.cor_atual[0] = 1.0f;
    ctx.ui.cor_atual[1] = 0.0f;
    ctx.ui.cor_atual[2] = 0.0f;
    ctx.ui.espessura_atual = 8.0f;
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(LARGURA_JANELA, ALTURA_JANELA);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("CG Paint - Trabalho de Computacao Grafica");

    glClearColor(0.15f, 0.15f, 0.15f, 1.0f);

    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutKeyboardFunc(teclado);
    glutReshapeFunc(redimensionar);
    printf("=============================================\n");
    printf("       CG PAINT - Computacao Grafica\n");
    printf("=============================================\n");
    printf("  FERRAMENTAS (teclado numerico):\n");
    printf("    1 = Ponto\n");
    printf("    2 = Reta       (nao implementado)\n");
    printf("    3 = Poligono   (nao implementado)\n");
    printf("    4 = Selecao    (nao implementado)\n");
    printf("    5 = Borracha   (nao implementado)\n");
    printf("  CONTROLES:\n");
    printf("    C     = trocar cor\n");
    printf("    +/-   = aumentar/diminuir espessura\n");
    printf("    L     = limpar tela\n");
    printf("    ESC   = sair\n");
    printf("=============================================\n\n");

    glutMainLoop();
    return 0;
}
