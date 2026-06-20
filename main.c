#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Estruturas.h"

// Prototipo da funcao de animacao
void atualizarPassoAnimacao(ContextoPaint *ctx);

// ============================================================================
// VARIAVEIS GLOBAIS
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
int animacao_ligada = 0;

// Modos de animacao suportados
#define ANIM_DIREITA  0
#define ANIM_ESQUERDA 1
#define ANIM_CIMA     2
#define ANIM_BAIXO    3
#define ANIM_CIRCULAR 4

int modo_animacao = ANIM_DIREITA;
float angulo_circular = 0.0f;

// ============================================================================
// LOOP DA ANIMACAO
// ============================================================================
void timerAnimacao(int valor) {
    if (animacao_ligada) {
        atualizarPassoAnimacao(&ctx);
        glutPostRedisplay();
    }
    glutTimerFunc(16, timerAnimacao, 0);
}

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

    if (ctx.ui.ferramenta_atual == MODO_RETA && ctx.ui.tem_p1_temp) {
        glPointSize(10.0f);
        glColor3f(ctx.ui.cor_atual[0], ctx.ui.cor_atual[1], ctx.ui.cor_atual[2]);
        glBegin(GL_POINTS);
            glVertex2f(ctx.ui.p1_temp.x, ctx.ui.p1_temp.y);
        glEnd();
    }

    if (ctx.ui.ferramenta_atual == MODO_POLIGONO && ctx.ui.qtd_vertices_temp > 0) {
        glPointSize(8.0f);
        glColor3f(ctx.ui.cor_atual[0], ctx.ui.cor_atual[1], ctx.ui.cor_atual[2]);
        glBegin(GL_POINTS);
        for (int i = 0; i < ctx.ui.qtd_vertices_temp; i++) {
            glVertex2f(ctx.ui.vertices_temp[i].x, ctx.ui.vertices_temp[i].y);
        }
        glEnd();

        if (ctx.ui.qtd_vertices_temp > 1) {
            glLineWidth(ctx.ui.espessura_atual > 0.0f ? ctx.ui.espessura_atual : 2.0f);
            glBegin(GL_LINE_STRIP);
            for (int i = 0; i < ctx.ui.qtd_vertices_temp; i++) {
                glVertex2f(ctx.ui.vertices_temp[i].x, ctx.ui.vertices_temp[i].y);
            }
            glEnd();
        }
    }

    const char* modo_texto = "";
    char poligono_texto[32];
    switch (ctx.ui.ferramenta_atual) {
        case MODO_PONTO:    modo_texto = "[PONTO]";    break;
        case MODO_RETA:     modo_texto = ctx.ui.tem_p1_temp ? "[RETA: clique P2]" : "[RETA]"; break;
        case MODO_POLIGONO:
            if (ctx.ui.construindo_poligono) {
                sprintf(poligono_texto, "[POLIGONO: %d vertices]", ctx.ui.qtd_vertices_temp);
                modo_texto = poligono_texto;
            } else {
                modo_texto = "[POLIGONO]";
            }
            break;
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
                if (!ctx.ui.tem_p1_temp) {
                    ctx.ui.p1_temp.x = mx;
                    ctx.ui.p1_temp.y = my;
                    ctx.ui.tem_p1_temp = 1;
                    printf("[RETA] P1 em (%.0f, %.0f) - clique P2 para finalizar\n", mx, my);
                } else {
                    Ponto p2 = {mx, my};
                    if (adicionarReta(&ctx.cena, ctx.ui.p1_temp, p2, ctx.ui.cor_atual, ctx.ui.espessura_atual)) {
                        printf("Reta #%d de (%.0f, %.0f) ate (%.0f, %.0f)\n",
                               ctx.cena.qtd_retas,
                               ctx.ui.p1_temp.x, ctx.ui.p1_temp.y, mx, my);
                    }
                    ctx.ui.tem_p1_temp = 0;
                }
                break;

            case MODO_POLIGONO:
                if (ctx.ui.qtd_vertices_temp < MAX_VERTICES) {
                    ctx.ui.vertices_temp[ctx.ui.qtd_vertices_temp].x = mx;
                    ctx.ui.vertices_temp[ctx.ui.qtd_vertices_temp].y = my;
                    ctx.ui.qtd_vertices_temp++;
                    ctx.ui.construindo_poligono = 1;
                    printf("[POLIGONO] Vertice %d em (%.0f, %.0f) - direito para finalizar\n",
                           ctx.ui.qtd_vertices_temp, mx, my);
                } else {
                    printf("[AVISO] Maximo de vertices atingido (%d).\n", MAX_VERTICES);
                }
                break;

            case MODO_SELECAO:
                printf("[SELECAO] Clique em (%.0f, %.0f) - ainda nao implementado\n", mx, my);
                break;

            case MODO_BORRACHA:
                printf("[BORRACHA] Clique em (%.0f, %.0f) - ainda nao implementado\n", mx, my);
                break;
        }
        glutPostRedisplay();
    }

    if (botao == GLUT_RIGHT_BUTTON && ctx.ui.ferramenta_atual == MODO_POLIGONO
        && ctx.ui.construindo_poligono) {
        if (ctx.ui.qtd_vertices_temp >= 3) {
            if (adicionarPoligono(&ctx.cena, ctx.ui.vertices_temp, ctx.ui.qtd_vertices_temp,
                                  ctx.ui.cor_atual, ctx.ui.cor_atual, 0, ctx.ui.espessura_atual)) {
                printf("Poligono #%d criado com %d vertices\n",
                       ctx.cena.qtd_poligonos, ctx.ui.qtd_vertices_temp);
            }
        } else {
            printf("[AVISO] Precisa de pelo menos 3 vertices (tem %d). Poligono cancelado.\n",
                   ctx.ui.qtd_vertices_temp);
        }
        ctx.ui.qtd_vertices_temp = 0;
        ctx.ui.construindo_poligono = 0;
        glutPostRedisplay();
    }
}

void teclado(unsigned char tecla, int x, int y) {
    switch (tecla) {
        // Ferramentas base
        case '1':
            ctx.ui.ferramenta_atual = MODO_PONTO;
            ctx.ui.tem_p1_temp = 0;
            printf(">> Ferramenta: PONTO\n");
            break;
        case '2':
            ctx.ui.ferramenta_atual = MODO_RETA;
            ctx.ui.tem_p1_temp = 0;
            printf(">> Ferramenta: RETA (clique 2 pontos)\n");
            break;
        case '3':
            ctx.ui.ferramenta_atual = MODO_POLIGONO;
            ctx.ui.tem_p1_temp = 0;
            ctx.ui.qtd_vertices_temp = 0;
            ctx.ui.construindo_poligono = 0;
            printf(">> Ferramenta: POLIGONO (esquerdo=vertice, direito=finalizar)\n");
            break;
        case '4':
            ctx.ui.ferramenta_atual = MODO_SELECAO;
            ctx.ui.tem_p1_temp = 0;
            printf(">> Ferramenta: SELECAO\n");
            break;
        case '5':
            ctx.ui.ferramenta_atual = MODO_BORRACHA;
            ctx.ui.tem_p1_temp = 0;
            printf(">> Ferramenta: BORRACHA\n");
            break;

        // UI
        case 'c': case 'C':
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
        case 'l': case 'L':
            memset(&ctx.cena, 0, sizeof(CenaGrafica));
            printf(">> Tela limpa!\n");
            glutPostRedisplay();
            break;

        // Controles de Animacao
        case 'p': case 'P':
            animacao_ligada = !animacao_ligada;
            printf(">> Animacao %s!\n", animacao_ligada ? "LIGADA" : "DESLIGADA");
            break;
        case 'w': case 'W':
            modo_animacao = ANIM_CIMA;
            printf(">> Direcao da animacao: CIMA\n");
            break;
        case 's': case 'S':
            modo_animacao = ANIM_BAIXO;
            printf(">> Direcao da animacao: BAIXO\n");
            break;
        case 'a': case 'A':
            modo_animacao = ANIM_ESQUERDA;
            printf(">> Direcao da animacao: ESQUERDA\n");
            break;
        case 'd': case 'D':
            modo_animacao = ANIM_DIREITA;
            printf(">> Direcao da animacao: DIREITA\n");
            break;
        case 'm': case 'M':
            modo_animacao = ANIM_CIRCULAR;
            printf(">> Modo de animacao: CIRCULAR\n");
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

    // ============================================================================
    // MENU DO CONSOLE ATUALIZADO
    // ============================================================================
    printf("==================================================\n");
    printf("         CG PAINT - Computacao Grafica            \n");
    printf("==================================================\n");
    printf("  FERRAMENTAS (teclado):\n");
    printf("    1 = Ponto\n");
    printf("    2 = Reta       (2 cliques: P1 e P2)\n");
    printf("    3 = Poligono   (esquerdo=vertice, direito=finalizar)\n");
    printf("    4 = Selecao    (nao implementado)\n");
    printf("    5 = Borracha   (nao implementado)\n");
    printf("--------------------------------------------------\n");
    printf("  CONTROLES GERAIS:\n");
    printf("    C     = Trocar cor\n");
    printf("    +/-   = Aumentar / diminuir espessura\n");
    printf("    L     = Limpar tela\n");
    printf("    ESC   = Sair\n");
    printf("--------------------------------------------------\n");
    printf("  CONTROLES DE ANIMACAO:\n");
    printf("    P     = Ativar / Pausar animacao\n");
    printf("    W     = Mover para CIMA\n");
    printf("    S     = Mover para BAIXO\n");
    printf("    A     = Mover para ESQUERDA\n");
    printf("    D     = Mover para DIREITA\n");
    printf("    M     = Ativar movimento CIRCULAR (Orbita)\n");
    printf("==================================================\n\n");

    // Inicializa o motor de animacao
    glutTimerFunc(16, timerAnimacao, 0);

    glutMainLoop();
    return 0;
}
