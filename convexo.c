#include "convexo.h"
#include <stdio.h>

// Produto Vetorial 2D para determinar a orientação de três pontos
static float produtoEscalarObliquo(Ponto a, Ponto b, Ponto c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

// Algoritmo Gift Wrapping (Marcha de Jarvis)
static void aplicarJarvisMarch(Poligono *p) {
    int n = p->qtd_vertices;
    if (n < 3) return;

    Ponto fecho[MAX_VERTICES];
    int qtd_fecho = 0;

    // Encontrar o ponto mais à esquerda
    int esquerdo = 0;
    for (int i = 1; i < n; i++) {
        if (p->vertices[i].x < p->vertices[esquerdo].x) {
            esquerdo = i;
        } else if (p->vertices[i].x == p->vertices[esquerdo].x) {
            if (p->vertices[i].y < p->vertices[esquerdo].y) {
                esquerdo = i;
            }
        }
    }

    int p_atual = esquerdo;
    int q_proximo;

    do {
        if (qtd_fecho >= MAX_VERTICES) {
            printf("[AVISO] Limite máximo de vértices do fecho atingido.\n");
            break;
        }
        fecho[qtd_fecho++] = p->vertices[p_atual];
        q_proximo = (p_atual + 1) % n;

        for (int r = 0; r < n; r++) {
            if (produtoEscalarObliquo(p->vertices[p_atual], p->vertices[r], p->vertices[q_proximo]) > 0) {
                q_proximo = r;
            }
        }
        p_atual = q_proximo;

    } while (p_atual != esquerdo);

    // Atualiza o polígono com os vértices do fecho convexo
    p->qtd_vertices = qtd_fecho;
    for (int i = 0; i < qtd_fecho; i++) {
        p->vertices[i] = fecho[i];
    }
}


void converterParaConvexo(CenaGrafica *cena) {
    int conversoes = 0;

    for (int i = 0; i < cena->qtd_poligonos; i++) {
        if (cena->poligonos[i].selecionado) {
            aplicarJarvisMarch(&cena->poligonos[i]);
            conversoes++;
        }
    }

    if (conversoes == 0) {
        printf("[AVISO] Nenhum polígono selecionado para converter.\n");
    } else {
        printf("[SUCESSO] %d polígono(s) convertido(s) para convexo.\n", conversoes);
    }
}
