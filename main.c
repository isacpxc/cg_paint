#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Estruturas.h"

// ============================================================================
// VARIAVEL GLOBAL - Contexto do Paint
// ============================================================================
ContextoPaint ctx;

// Largura e altura iniciais da janela.
#define LARGURA_JANELA 800
#define ALTURA_JANELA 600

#define ARQUIVO_PADRAO "cena_cg_paint.bin"
#define INTERVALO_TIMER_MS 16
#define PASSO_TRANSLACAO 10.0f
#define PASSO_ROTACAO 10.0f
#define FATOR_ESCALA_MAIOR 1.10f
#define FATOR_ESCALA_MENOR 0.90f
#define PASSO_CISALHAMENTO 0.10f

// Paleta de cores disponiveis.
static float paleta[][3] = {
    {1.0f, 0.0f, 0.0f}, // 0 - Vermelho
    {0.0f, 1.0f, 0.0f}, // 1 - Verde
    {0.0f, 0.0f, 1.0f}, // 2 - Azul
    {1.0f, 1.0f, 0.0f}, // 3 - Amarelo
    {1.0f, 0.0f, 1.0f}, // 4 - Magenta
    {0.0f, 1.0f, 1.0f}, // 5 - Ciano
    {1.0f, 1.0f, 1.0f}, // 6 - Branco
    {1.0f, 0.5f, 0.0f}, // 7 - Laranja
    {0.0f, 0.0f, 0.0f}  // 8 - Preto
};

static const char* nomes_cores[] = {
    "VERMELHO", "VERDE", "AZUL", "AMARELO", "MAGENTA", "CIANO", "BRANCO", "LARANJA", "PRETO"
};

static const int total_cores = (int)(sizeof(paleta) / sizeof(paleta[0]));
static int indice_cor = 0;
static int mouse_esquerdo_pressionado = 0;
static Ponto ultimo_mouse = {0.0f, 0.0f};

// ============================================================================
// FUNCOES AUXILIARES DE ESTADO / UI
// ============================================================================

static void copiarCor(float destino[3], const float origem[3]) {
    destino[0] = origem[0];
    destino[1] = origem[1];
    destino[2] = origem[2];
}

static void aplicarCorAtualPorIndice(int indice) {
    if (indice < 0) indice = total_cores - 1;
    if (indice >= total_cores) indice = 0;

    indice_cor = indice;
    copiarCor(ctx.ui.cor_atual, paleta[indice_cor]);
    printf(">> Cor atual: %s\n", nomes_cores[indice_cor]);
}

static void cancelarOperacaoTemporaria(void) {
    ctx.ui.tem_p1_temp = 0;
    ctx.ui.qtd_vertices_temp = 0;
    ctx.ui.construindo_poligono = 0;
}

static const char* nomeFerramenta(ModoFerramenta ferramenta) {
    switch (ferramenta) {
        case MODO_PONTO:    return "PONTO";
        case MODO_RETA:     return "RETA";
        case MODO_POLIGONO: return "POLIGONO";
        case MODO_SELECAO:  return "SELECAO";
        case MODO_BORRACHA: return "BORRACHA";
        default:            return "DESCONHECIDA";
    }
}

static void definirFerramenta(ModoFerramenta ferramenta) {
    cancelarOperacaoTemporaria();
    ctx.ui.ferramenta_atual = ferramenta;

    printf(">> Ferramenta: %s\n", nomeFerramenta(ferramenta));
    if (ferramenta == MODO_RETA) {
        printf("   Use dois cliques: primeiro para P1, segundo para P2.\n");
    } else if (ferramenta == MODO_POLIGONO) {
        printf("   Esquerdo adiciona vertice; Enter ou direito finaliza; Backspace remove ultimo vertice.\n");
    }

    glutPostRedisplay();
}

static void ajustarEspessura(float delta) {
    ctx.ui.espessura_atual += delta;

    if (ctx.ui.espessura_atual < 1.0f) ctx.ui.espessura_atual = 1.0f;
    if (ctx.ui.espessura_atual > 50.0f) ctx.ui.espessura_atual = 50.0f;

    printf(">> Espessura: %.0f\n", ctx.ui.espessura_atual);
    glutPostRedisplay();
}

static Ponto converterMouseParaMundo(int x, int y) {
    Ponto p;
    p.x = (float)x;
    p.y = (float)(glutGet(GLUT_WINDOW_HEIGHT) - y);
    return p;
}

static void desenharTexto(float x, float y, const char *texto) {
    glRasterPos2f(x, y);
    for (const char* c = texto; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *c);
    }
}

static void desenharAmostraCor(void) {
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
}

static void desenharHUD(void) {
    char texto[160];

    desenharAmostraCor();

    snprintf(texto, sizeof(texto), "[%s] Cor: %s | Esp: %.0f | P:%d R:%d Pol:%d%s",
             nomeFerramenta(ctx.ui.ferramenta_atual),
             nomes_cores[indice_cor],
             ctx.ui.espessura_atual,
             ctx.cena.qtd_pontos,
             ctx.cena.qtd_retas,
             ctx.cena.qtd_poligonos,
             ctx.ui.animacao.ativa ? " | ANIMANDO" : "");

    glColor3f(1.0f, 1.0f, 1.0f);
    desenharTexto(40, 16, texto);

    desenharTexto(10, (float)glutGet(GLUT_WINDOW_HEIGHT) - 18,
                  "1=Ponto 2=Reta 3=Poligono 4=Selecao 5=Borracha | C=cor +/- esp | S=salvar O=abrir H=ajuda");
}

static void imprimirAjudaConsole(void) {
    printf("=============================================\n");
    printf(" CG PAINT - Atalhos e Eventos\n");
    printf("=============================================\n");
    printf(" FERRAMENTAS:\n");
    printf("  1 = Ponto\n");
    printf("  2 = Reta (2 cliques: P1 e P2)\n");
    printf("  3 = Poligono (esquerdo=vertice, Enter/direito=finalizar)\n");
    printf("  4 = Selecao\n");
    printf("  5 = Borracha\n");
    printf("\n DESENHO / ESTADO:\n");
    printf("  C = proxima cor\n");
    printf("  Shift+C = cor anterior\n");
    printf("  +/- = aumentar/diminuir espessura\n");
    printf("  L = limpar tela\n");
    printf("  Backspace = remover ultimo vertice do poligono em construcao\n");
    printf("  Enter = finalizar poligono\n");
    printf("  ESC = cancelar operacao temporaria; se nao houver nada, sair\n");
    printf("\n SELECAO / TRANSFORMACOES:\n");
    printf("  D = desselecionar todos\n");
    printf("  Delete/Backspace fora de poligono = excluir selecionados\n");
    printf("  Setas = transladar selecionados\n");
    printf("  [ e ] = rotacionar selecionados\n");
    printf("  , e . = diminuir/aumentar escala\n");
    printf("  X/Y/Z = refletir em X/Y/origem\n");
    printf("  J/K = cisalhamento negativo/positivo em X\n");
    printf("\n ARQUIVO / ANIMACAO:\n");
    printf("  S = salvar em '%s'\n", ARQUIVO_PADRAO);
    printf("  O = carregar de '%s'\n", ARQUIVO_PADRAO);
    printf("  Espaco ou A = ligar/desligar animacao\n");
    printf("  F1 = mostrar esta ajuda\n");
    printf("  F5 = salvar | F9 = carregar\n");
    printf("=============================================\n\n");
}

// ============================================================================
// INICIALIZACAO
// ============================================================================

void inicializarContexto(ContextoPaint *contexto) {
    memset(contexto, 0, sizeof(ContextoPaint));

    contexto->ui.ferramenta_atual = MODO_PONTO;
    contexto->ui.espessura_atual = 8.0f;
    contexto->ui.animacao.ativa = 0;
    contexto->ui.animacao.tempo = 0.0f;
    contexto->ui.animacao.velocidade = 1.0f;

    copiarCor(contexto->ui.cor_atual, paleta[0]);
    indice_cor = 0;
}

static void inicializarOpenGL(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(LARGURA_JANELA, ALTURA_JANELA);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("CG Paint - Trabalho de Computacao Grafica");

    glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
    glDisable(GL_DEPTH_TEST);
}

// ============================================================================
// DISPLAY / RENDERIZACAO
// ============================================================================

static void renderizarPonto(PontoDesenho *p) {
    if (p->selecionado) {
        glPointSize(p->tamanho + 6.0f);
        glColor3f(1.0f, 1.0f, 1.0f);
        glBegin(GL_POINTS);
            glVertex2f(p->posicao.x, p->posicao.y);
        glEnd();
    }

    glPointSize(p->tamanho);
    glColor3f(p->cor[0], p->cor[1], p->cor[2]);
    glBegin(GL_POINTS);
        glVertex2f(p->posicao.x, p->posicao.y);
    glEnd();
}

static void renderizarReta(Reta *r) {
    if (r->selecionado) {
        glLineWidth(r->espessura + 4.0f);
        glColor3f(1.0f, 1.0f, 1.0f);
        glBegin(GL_LINES);
            glVertex2f(r->p1.x, r->p1.y);
            glVertex2f(r->p2.x, r->p2.y);
        glEnd();
    }

    glLineWidth(r->espessura);
    glColor3f(r->cor[0], r->cor[1], r->cor[2]);
    glBegin(GL_LINES);
        glVertex2f(r->p1.x, r->p1.y);
        glVertex2f(r->p2.x, r->p2.y);
    glEnd();
}

static void renderizarPoligono(Poligono *pol) {
    if (pol->preenchido) {
        glColor3f(pol->cor_preenchimento[0], pol->cor_preenchimento[1], pol->cor_preenchimento[2]);
        glBegin(GL_POLYGON);
            for (int j = 0; j < pol->qtd_vertices; j++) {
                glVertex2f(pol->vertices[j].x, pol->vertices[j].y);
            }
        glEnd();
    }

    if (pol->selecionado) {
        glLineWidth(pol->espessura_contorno + 4.0f);
        glColor3f(1.0f, 1.0f, 1.0f);
        glBegin(GL_LINE_LOOP);
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

static void renderizarTemporarios(void) {
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
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    for (int i = 0; i < ctx.cena.qtd_pontos; i++) {
        renderizarPonto(&ctx.cena.pontos[i]);
    }

    for (int i = 0; i < ctx.cena.qtd_retas; i++) {
        renderizarReta(&ctx.cena.retas[i]);
    }

    for (int i = 0; i < ctx.cena.qtd_poligonos; i++) {
        renderizarPoligono(&ctx.cena.poligonos[i]);
    }

    renderizarTemporarios();
    desenharHUD();

    glutSwapBuffers();
}

// ============================================================================
// MOUSE
// ============================================================================

static void finalizarPoligonoAtual(void) {
    if (!ctx.ui.construindo_poligono) {
        return;
    }

    if (ctx.ui.qtd_vertices_temp >= 3) {
        if (adicionarPoligono(&ctx.cena,
                              ctx.ui.vertices_temp,
                              ctx.ui.qtd_vertices_temp,
                              ctx.ui.cor_atual,
                              ctx.ui.cor_atual,
                              0,
                              ctx.ui.espessura_atual)) {
            printf("Poligono #%d criado com %d vertices\n",
                   ctx.cena.qtd_poligonos,
                   ctx.ui.qtd_vertices_temp);
        }
    } else {
        printf("[AVISO] Precisa de pelo menos 3 vertices (tem %d). Poligono cancelado.\n",
               ctx.ui.qtd_vertices_temp);
    }

    ctx.ui.qtd_vertices_temp = 0;
    ctx.ui.construindo_poligono = 0;
    glutPostRedisplay();
}

static void removerUltimoVerticeTemporario(void) {
    if (ctx.ui.ferramenta_atual != MODO_POLIGONO || ctx.ui.qtd_vertices_temp <= 0) {
        excluirObjetosSelecionados(&ctx.cena);
        glutPostRedisplay();
        return;
    }

    ctx.ui.qtd_vertices_temp--;
    if (ctx.ui.qtd_vertices_temp == 0) {
        ctx.ui.construindo_poligono = 0;
    }

    printf(">> Ultimo vertice removido. Vertices temporarios: %d\n", ctx.ui.qtd_vertices_temp);
    glutPostRedisplay();
}

void mouse(int botao, int estado, int x, int y) {
    Ponto m = converterMouseParaMundo(x, y);

    if (botao == GLUT_LEFT_BUTTON) {
        mouse_esquerdo_pressionado = (estado == GLUT_DOWN);
        ultimo_mouse = m;
    }

    if (estado != GLUT_DOWN) {
        return;
    }

    if (botao == GLUT_LEFT_BUTTON) {
        switch (ctx.ui.ferramenta_atual) {
            case MODO_PONTO:
                if (adicionarPonto(&ctx.cena, m.x, m.y, ctx.ui.cor_atual, ctx.ui.espessura_atual)) {
                    printf("Ponto #%d em (%.0f, %.0f)\n", ctx.cena.qtd_pontos, m.x, m.y);
                }
                break;

            case MODO_RETA:
                if (!ctx.ui.tem_p1_temp) {
                    ctx.ui.p1_temp = m;
                    ctx.ui.tem_p1_temp = 1;
                    printf("[RETA] P1 em (%.0f, %.0f) - clique P2 para finalizar\n", m.x, m.y);
                } else {
                    if (adicionarReta(&ctx.cena, ctx.ui.p1_temp, m, ctx.ui.cor_atual, ctx.ui.espessura_atual)) {
                        printf("Reta #%d de (%.0f, %.0f) ate (%.0f, %.0f)\n",
                               ctx.cena.qtd_retas,
                               ctx.ui.p1_temp.x,
                               ctx.ui.p1_temp.y,
                               m.x,
                               m.y);
                    }
                    ctx.ui.tem_p1_temp = 0;
                }
                break;

            case MODO_POLIGONO:
                if (ctx.ui.qtd_vertices_temp < MAX_VERTICES) {
                    ctx.ui.vertices_temp[ctx.ui.qtd_vertices_temp] = m;
                    ctx.ui.qtd_vertices_temp++;
                    ctx.ui.construindo_poligono = 1;
                    printf("[POLIGONO] Vertice %d em (%.0f, %.0f) - Enter/direito para finalizar\n",
                           ctx.ui.qtd_vertices_temp,
                           m.x,
                           m.y);
                } else {
                    printf("[AVISO] Maximo de vertices atingido (%d).\n", MAX_VERTICES);
                }
                break;

            case MODO_SELECAO:
                selecionarObjeto(&ctx.cena, m.x, m.y);
                printf("[SELECAO] Clique em (%.0f, %.0f). Arraste para transladar selecionados.\n", m.x, m.y);
                break;

            case MODO_BORRACHA:
                selecionarObjeto(&ctx.cena, m.x, m.y);
                excluirObjetosSelecionados(&ctx.cena);
                printf("[BORRACHA] Tentativa de apagar objeto em (%.0f, %.0f).\n", m.x, m.y);
                break;
        }

        glutPostRedisplay();
    }

    if (botao == GLUT_RIGHT_BUTTON && ctx.ui.ferramenta_atual == MODO_POLIGONO) {
        finalizarPoligonoAtual();
    }
}

void movimentoMouse(int x, int y) {
    if (!mouse_esquerdo_pressionado || ctx.ui.ferramenta_atual != MODO_SELECAO) {
        return;
    }

    Ponto atual = converterMouseParaMundo(x, y);
    float dx = atual.x - ultimo_mouse.x;
    float dy = atual.y - ultimo_mouse.y;

    if (dx != 0.0f || dy != 0.0f) {
        transladarObjeto(&ctx.cena, dx, dy);
        ultimo_mouse = atual;
        glutPostRedisplay();
    }
}

// ============================================================================
// TECLADO / ATALHOS
// ============================================================================

static int existeOperacaoTemporaria(void) {
    return ctx.ui.tem_p1_temp || ctx.ui.construindo_poligono || ctx.ui.qtd_vertices_temp > 0;
}

static void limparTela(void) {
    memset(&ctx.cena, 0, sizeof(CenaGrafica));
    cancelarOperacaoTemporaria();
    printf(">> Tela limpa!\n");
    glutPostRedisplay();
}

static void salvarArquivoPadrao(void) {
    salvarCena(&ctx.cena, ARQUIVO_PADRAO);
}

static void carregarArquivoPadrao(void) {
    if (carregarCena(&ctx.cena, ARQUIVO_PADRAO)) {
        cancelarOperacaoTemporaria();
        glutPostRedisplay();
    }
}

static void alternarAnimacao(void) {
    ctx.ui.animacao.ativa = !ctx.ui.animacao.ativa;
    printf(">> Animacao: %s\n", ctx.ui.animacao.ativa ? "ATIVA" : "PAUSADA");
    glutPostRedisplay();
}

void teclado(unsigned char tecla, int x, int y) {
    (void)x;
    (void)y;

    switch (tecla) {
        case '1': definirFerramenta(MODO_PONTO); break;
        case '2': definirFerramenta(MODO_RETA); break;
        case '3': definirFerramenta(MODO_POLIGONO); break;
        case '4': definirFerramenta(MODO_SELECAO); break;
        case '5': definirFerramenta(MODO_BORRACHA); break;

        case 'c': aplicarCorAtualPorIndice(indice_cor + 1); glutPostRedisplay(); break;
        case 'C': aplicarCorAtualPorIndice(indice_cor - 1); glutPostRedisplay(); break;

        case '+': case '=': ajustarEspessura(2.0f); break;
        case '-': case '_': ajustarEspessura(-2.0f); break;

        case 'l': case 'L': limparTela(); break;
        case 's': case 'S': salvarArquivoPadrao(); break;
        case 'o': case 'O': carregarArquivoPadrao(); break;
        case 'd': case 'D':
            desselecionarTodos(&ctx.cena);
            printf(">> Todos os objetos foram desselecionados.\n");
            glutPostRedisplay();
            break;

        case 8:   // Backspace
        case 127: // Delete em alguns teclados/terminais
            removerUltimoVerticeTemporario();
            break;

        case 13: // Enter
            finalizarPoligonoAtual();
            break;

        case '[':
            rotacionarObjeto(&ctx.cena, -PASSO_ROTACAO);
            glutPostRedisplay();
            break;
        case ']':
            rotacionarObjeto(&ctx.cena, PASSO_ROTACAO);
            glutPostRedisplay();
            break;
        case ',':
            escalarObjeto(&ctx.cena, FATOR_ESCALA_MENOR, FATOR_ESCALA_MENOR);
            glutPostRedisplay();
            break;
        case '.':
            escalarObjeto(&ctx.cena, FATOR_ESCALA_MAIOR, FATOR_ESCALA_MAIOR);
            glutPostRedisplay();
            break;
        case 'x': case 'X':
            refletirObjeto(&ctx.cena, REFLEXAO_X);
            glutPostRedisplay();
            break;
        case 'y': case 'Y':
            refletirObjeto(&ctx.cena, REFLEXAO_Y);
            glutPostRedisplay();
            break;
        case 'z': case 'Z':
            refletirObjeto(&ctx.cena, REFLEXAO_ORIGEM);
            glutPostRedisplay();
            break;
        case 'j': case 'J':
            cisalharObjeto(&ctx.cena, -PASSO_CISALHAMENTO, 0.0f);
            glutPostRedisplay();
            break;
        case 'k': case 'K':
            cisalharObjeto(&ctx.cena, PASSO_CISALHAMENTO, 0.0f);
            glutPostRedisplay();
            break;

        case ' ':
        case 'a': case 'A':
            alternarAnimacao();
            break;

        case 'h': case 'H':
            imprimirAjudaConsole();
            break;

        case 27: // ESC
            if (existeOperacaoTemporaria()) {
                cancelarOperacaoTemporaria();
                printf(">> Operacao temporaria cancelada.\n");
                glutPostRedisplay();
            } else {
                printf(">> Encerrando CG Paint.\n");
                exit(0);
            }
            break;

        default:
            printf(">> Atalho nao mapeado: '%c' (%d). Pressione H para ajuda.\n", tecla, tecla);
            break;
    }
}

void tecladoEspecial(int tecla, int x, int y) {
    (void)x;
    (void)y;

    switch (tecla) {
        case GLUT_KEY_LEFT:
            transladarObjeto(&ctx.cena, -PASSO_TRANSLACAO, 0.0f);
            break;
        case GLUT_KEY_RIGHT:
            transladarObjeto(&ctx.cena, PASSO_TRANSLACAO, 0.0f);
            break;
        case GLUT_KEY_UP:
            transladarObjeto(&ctx.cena, 0.0f, PASSO_TRANSLACAO);
            break;
        case GLUT_KEY_DOWN:
            transladarObjeto(&ctx.cena, 0.0f, -PASSO_TRANSLACAO);
            break;
        case GLUT_KEY_PAGE_UP:
            rotacionarObjeto(&ctx.cena, PASSO_ROTACAO);
            break;
        case GLUT_KEY_PAGE_DOWN:
            rotacionarObjeto(&ctx.cena, -PASSO_ROTACAO);
            break;
        case GLUT_KEY_HOME:
            escalarObjeto(&ctx.cena, FATOR_ESCALA_MAIOR, FATOR_ESCALA_MAIOR);
            break;
        case GLUT_KEY_END:
            escalarObjeto(&ctx.cena, FATOR_ESCALA_MENOR, FATOR_ESCALA_MENOR);
            break;
        case GLUT_KEY_F1:
            imprimirAjudaConsole();
            break;
        case GLUT_KEY_F5:
            salvarArquivoPadrao();
            break;
        case GLUT_KEY_F9:
            carregarArquivoPadrao();
            break;
        default:
            printf(">> Tecla especial nao mapeada: %d. Pressione F1 para ajuda.\n", tecla);
            break;
    }

    glutPostRedisplay();
}

// ============================================================================
// TIMER / ANIMACAO
// ============================================================================

void timer(int valor) {
    (void)valor;

    if (ctx.ui.animacao.ativa) {
        ctx.ui.animacao.tempo += 0.016f * ctx.ui.animacao.velocidade;
        rotacionarObjeto(&ctx.cena, ctx.ui.animacao.velocidade);
        glutPostRedisplay();
    }

    glutTimerFunc(INTERVALO_TIMER_MS, timer, 0);
}

// ============================================================================
// JANELA / PROJECAO
// ============================================================================

void redimensionar(int w, int h) {
    if (h <= 0) h = 1;

    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Mantem o mundo 2D na mesma escala do mouse: origem embaixo/esquerda.
    gluOrtho2D(0, w, 0, h);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

static void registrarCallbacks(void) {
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutMotionFunc(movimentoMouse);
    glutKeyboardFunc(teclado);
    glutSpecialFunc(tecladoEspecial);
    glutReshapeFunc(redimensionar);
    glutTimerFunc(INTERVALO_TIMER_MS, timer, 0);
}

// ============================================================================
// FALLBACKS TEMPORARIOS PARA MODULOS AINDA NAO IMPLEMENTADOS
// ============================================================================
// Estas funcoes weak permitem compilar/testar o main.c na branch dev atual.
// Quando outro arquivo .c implementar a mesma funcao, o GCC/MinGW usa a versao
// real do colega e ignora esta versao fraca.

#if defined(__GNUC__)
#define CG_PAINT_WEAK __attribute__((weak))
#else
#define CG_PAINT_WEAK
#endif

static float distancia2(Ponto a, Ponto b) {
    float dx = a.x - b.x;
    float dy = a.y - b.y;
    return dx * dx + dy * dy;
}

static float distancia2PontoSegmento(Ponto p, Ponto a, Ponto b) {
    float vx = b.x - a.x;
    float vy = b.y - a.y;
    float wx = p.x - a.x;
    float wy = p.y - a.y;
    float c1 = vx * wx + vy * wy;

    if (c1 <= 0.0f) {
        return distancia2(p, a);
    }

    float c2 = vx * vx + vy * vy;
    if (c2 <= c1) {
        return distancia2(p, b);
    }

    float t = c1 / c2;
    Ponto proj = {a.x + t * vx, a.y + t * vy};
    return distancia2(p, proj);
}

static int pontoDentroPoligono(Ponto p, Poligono *pol) {
    int dentro = 0;
    int j = pol->qtd_vertices - 1;

    for (int i = 0; i < pol->qtd_vertices; i++) {
        Ponto pi = pol->vertices[i];
        Ponto pj = pol->vertices[j];

        if (((pi.y > p.y) != (pj.y > p.y)) &&
            (p.x < (pj.x - pi.x) * (p.y - pi.y) / (pj.y - pi.y + 0.0001f) + pi.x)) {
            dentro = !dentro;
        }

        j = i;
    }

    return dentro;
}

CG_PAINT_WEAK void desselecionarTodos(CenaGrafica *cena) {
    for (int i = 0; i < cena->qtd_pontos; i++) cena->pontos[i].selecionado = 0;
    for (int i = 0; i < cena->qtd_retas; i++) cena->retas[i].selecionado = 0;
    for (int i = 0; i < cena->qtd_poligonos; i++) cena->poligonos[i].selecionado = 0;
}

CG_PAINT_WEAK void selecionarObjeto(CenaGrafica *cena, float mouseX, float mouseY) {
    Ponto mouse = {mouseX, mouseY};
    float melhor = 12.0f * 12.0f;
    TipoObjeto tipo = TIPO_PONTO;
    int indice = -1;

    desselecionarTodos(cena);

    for (int i = 0; i < cena->qtd_pontos; i++) {
        float d = distancia2(mouse, cena->pontos[i].posicao);
        if (d < melhor) {
            melhor = d;
            tipo = TIPO_PONTO;
            indice = i;
        }
    }

    for (int i = 0; i < cena->qtd_retas; i++) {
        float limite = cena->retas[i].espessura + 8.0f;
        float d = distancia2PontoSegmento(mouse, cena->retas[i].p1, cena->retas[i].p2);
        if (d < limite * limite && d < melhor) {
            melhor = d;
            tipo = TIPO_RETA;
            indice = i;
        }
    }

    for (int i = 0; i < cena->qtd_poligonos; i++) {
        int achou = pontoDentroPoligono(mouse, &cena->poligonos[i]);
        for (int j = 0; !achou && j < cena->poligonos[i].qtd_vertices; j++) {
            Ponto a = cena->poligonos[i].vertices[j];
            Ponto b = cena->poligonos[i].vertices[(j + 1) % cena->poligonos[i].qtd_vertices];
            if (distancia2PontoSegmento(mouse, a, b) <= 12.0f * 12.0f) {
                achou = 1;
            }
        }

        if (achou) {
            tipo = TIPO_POLIGONO;
            indice = i;
            break;
        }
    }

    if (indice >= 0) {
        if (tipo == TIPO_PONTO) cena->pontos[indice].selecionado = 1;
        if (tipo == TIPO_RETA) cena->retas[indice].selecionado = 1;
        if (tipo == TIPO_POLIGONO) cena->poligonos[indice].selecionado = 1;
    }
}

CG_PAINT_WEAK void excluirObjetosSelecionados(CenaGrafica *cena) {
    int destino = 0;
    for (int i = 0; i < cena->qtd_pontos; i++) {
        if (!cena->pontos[i].selecionado) cena->pontos[destino++] = cena->pontos[i];
    }
    cena->qtd_pontos = destino;

    destino = 0;
    for (int i = 0; i < cena->qtd_retas; i++) {
        if (!cena->retas[i].selecionado) cena->retas[destino++] = cena->retas[i];
    }
    cena->qtd_retas = destino;

    destino = 0;
    for (int i = 0; i < cena->qtd_poligonos; i++) {
        if (!cena->poligonos[i].selecionado) cena->poligonos[destino++] = cena->poligonos[i];
    }
    cena->qtd_poligonos = destino;
}

CG_PAINT_WEAK void transladarObjeto(CenaGrafica *cena, float tx, float ty) {
    for (int i = 0; i < cena->qtd_pontos; i++) {
        if (cena->pontos[i].selecionado) {
            cena->pontos[i].posicao.x += tx;
            cena->pontos[i].posicao.y += ty;
        }
    }

    for (int i = 0; i < cena->qtd_retas; i++) {
        if (cena->retas[i].selecionado) {
            cena->retas[i].p1.x += tx;
            cena->retas[i].p1.y += ty;
            cena->retas[i].p2.x += tx;
            cena->retas[i].p2.y += ty;
        }
    }

    for (int i = 0; i < cena->qtd_poligonos; i++) {
        if (cena->poligonos[i].selecionado) {
            for (int j = 0; j < cena->poligonos[i].qtd_vertices; j++) {
                cena->poligonos[i].vertices[j].x += tx;
                cena->poligonos[i].vertices[j].y += ty;
            }
        }
    }
}

CG_PAINT_WEAK void rotacionarObjeto(CenaGrafica *cena, float angulo) {
    (void)cena;
    (void)angulo;
    // Implementacao real deve ficar no modulo Transformacoes.c.
}

CG_PAINT_WEAK void escalarObjeto(CenaGrafica *cena, float sx, float sy) {
    (void)cena;
    (void)sx;
    (void)sy;
    // Implementacao real deve ficar no modulo Transformacoes.c.
}

CG_PAINT_WEAK void refletirObjeto(CenaGrafica *cena, EixoReflexao eixo) {
    (void)cena;
    (void)eixo;
    // Implementacao real deve ficar no modulo Transformacoes.c.
}

CG_PAINT_WEAK void cisalharObjeto(CenaGrafica *cena, float shx, float shy) {
    (void)cena;
    (void)shx;
    (void)shy;
    // Implementacao real deve ficar no modulo Transformacoes.c.
}

// ============================================================================
// MAIN
// ============================================================================

int main(int argc, char** argv) {
    inicializarContexto(&ctx);
    inicializarOpenGL(argc, argv);
    registrarCallbacks();
    imprimirAjudaConsole();

    glutMainLoop();
    return 0;
}
