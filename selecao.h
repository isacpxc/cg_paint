#ifndef SELECAO_H
#define SELECAO_H

#include "Estruturas.h"

void desenharCena(CenaGrafica *cena);
void selecionarObjeto(CenaGrafica *cena, float mouseX, float mouseY);
void excluirObjetoSelecionado(CenaGrafica *cena, float mouseX, float mouseY);
void desselecionarTodos(CenaGrafica *cena);

#endif

