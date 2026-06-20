#include <stdio.h>
#include <math.h>
#include "Estruturas.h"
#define ANIM_DIREITA  0
#define ANIM_ESQUERDA 1
#define ANIM_CIMA     2
#define ANIM_BAIXO    3
#define ANIM_CIRCULAR 4

extern int modo_animacao;
extern float angulo_circular;

void atualizarPassoAnimacao(ContextoPaint *ctx) {
    float vel = 2.0f;
    float dx = 0.0f;
    float dy = 0.0f;

    // Determina o deslocamento baseado no modo atual
    if (modo_animacao == ANIM_CIRCULAR) {
        angulo_circular += 0.03f;
        if (angulo_circular > 2.0f * 3.14159f) {
            angulo_circular = 0.0f;
        }
        dx = cosf(angulo_circular) * 1.5f;
        dy = sinf(angulo_circular) * 1.5f;
    } else {
        if (modo_animacao == ANIM_DIREITA)   dx = vel;
        if (modo_animacao == ANIM_ESQUERDA)  dx = -vel;
        if (modo_animacao == ANIM_CIMA)      dy = vel;
        if (modo_animacao == ANIM_BAIXO)     dy = -vel;
    }

    // =========================================================================
    // ANIMANDO PONTOS
    // =========================================================================
    for (int i = 0; i < ctx->cena.qtd_pontos; i++) {
        ctx->cena.pontos[i].posicao.x += dx;
        ctx->cena.pontos[i].posicao.y += dy;
        if (ctx->cena.pontos[i].posicao.x > 800.0f) ctx->cena.pontos[i].posicao.x = 0.0f;
        if (ctx->cena.pontos[i].posicao.x < 0.0f)   ctx->cena.pontos[i].posicao.x = 800.0f;
        if (ctx->cena.pontos[i].posicao.y > 600.0f) ctx->cena.pontos[i].posicao.y = 0.0f;
        if (ctx->cena.pontos[i].posicao.y < 0.0f)   ctx->cena.pontos[i].posicao.y = 600.0f;
    }

    // =========================================================================
    // ANIMANDO RETAS
    // =========================================================================
    for (int i = 0; i < ctx->cena.qtd_retas; i++) {
        ctx->cena.retas[i].p1.x += dx;
        ctx->cena.retas[i].p1.y += dy;
        ctx->cena.retas[i].p2.x += dx;
        ctx->cena.retas[i].p2.y += dy;
        if (ctx->cena.retas[i].p1.x > 800.0f) {
            float tam = ctx->cena.retas[i].p2.x - ctx->cena.retas[i].p1.x;
            ctx->cena.retas[i].p1.x = 0.0f; ctx->cena.retas[i].p2.x = tam;
        }
        if (ctx->cena.retas[i].p1.x < 0.0f) {
            float tam = ctx->cena.retas[i].p2.x - ctx->cena.retas[i].p1.x;
            ctx->cena.retas[i].p1.x = 800.0f; ctx->cena.retas[i].p2.x = 800.0f + tam;
        }
        if (ctx->cena.retas[i].p1.y > 600.0f) {
            float tam = ctx->cena.retas[i].p2.y - ctx->cena.retas[i].p1.y;
            ctx->cena.retas[i].p1.y = 0.0f; ctx->cena.retas[i].p2.y = tam;
        }
        if (ctx->cena.retas[i].p1.y < 0.0f) {
            float tam = ctx->cena.retas[i].p2.y - ctx->cena.retas[i].p1.y;
            ctx->cena.retas[i].p1.y = 600.0f; ctx->cena.retas[i].p2.y = 600.0f + tam;
        }
    }

    // =========================================================================
    // ANIMANDO POLIGONOS
    // =========================================================================
    for (int i = 0; i < ctx->cena.qtd_poligonos; i++) {
        if (ctx->cena.poligonos[i].qtd_vertices == 0) continue;

        // Move todos os vertices juntos usando o deslocamento calculado
        for (int j = 0; j < ctx->cena.poligonos[i].qtd_vertices; j++) {
            ctx->cena.poligonos[i].vertices[j].x += dx;
            ctx->cena.poligonos[i].vertices[j].y += dy;
        }

        // Pega o ponto de referencia (vertice 0) para checar limites de tela
        float refX = ctx->cena.poligonos[i].vertices[0].x;
        float refY = ctx->cena.poligonos[i].vertices[0].y;

        if (refX > 800.0f || refX < 0.0f || refY > 600.0f || refY < 0.0f) {
            for (int j = 0; j < ctx->cena.poligonos[i].qtd_vertices; j++) {
                if (refX > 800.0f) ctx->cena.poligonos[i].vertices[j].x -= 800.0f;
                if (refX < 0.0f)   ctx->cena.poligonos[i].vertices[j].x += 800.0f;
                if (refY > 600.0f) ctx->cena.poligonos[i].vertices[j].y -= 600.0f;
                if (refY < 0.0f)   ctx->cena.poligonos[i].vertices[j].y += 600.0f;
            }
        }
    }
}
