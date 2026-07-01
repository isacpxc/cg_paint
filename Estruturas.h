#ifndef ESTRUTURAS_H
#define ESTRUTURAS_H

#define MAX_VERTICES 200
#define MAX_OBJETOS 20000

#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/glut.h>

typedef enum {
  MODO_PONTO,
  MODO_RETA,
  MODO_POLIGONO,
  MODO_SELECAO,
  MODO_BORRACHA,
  MODO_LAPIS
} ModoFerramenta;

typedef enum { TIPO_PONTO, TIPO_RETA, TIPO_POLIGONO } TipoObjeto;

typedef enum { REFLEXAO_X, REFLEXAO_Y, REFLEXAO_ORIGEM } EixoReflexao;

typedef struct {
  float x;
  float y;
} Ponto;

typedef struct {
  float m[3][3];
} Matriz3x3;

typedef struct {
  Ponto posicao;
  float cor[3];
  float tamanho;
  int selecionado;
} PontoDesenho;

typedef struct {
  Ponto p1;
  Ponto p2;
  float cor[3];
  float espessura;
  int selecionado;
} Reta;

typedef struct {
  Ponto vertices[MAX_VERTICES];
  int qtd_vertices;
  float cor_contorno[3];
  float cor_preenchimento[3];
  int preenchido;
  float espessura_contorno;
  int selecionado;
} Poligono;

typedef struct {
  PontoDesenho pontos[MAX_OBJETOS];
  int qtd_pontos;

  Reta retas[MAX_OBJETOS];
  int qtd_retas;

  Poligono poligonos[MAX_OBJETOS];
  int qtd_poligonos;
} CenaGrafica;

typedef struct {
  int ativa;
  float tempo;
  float velocidade;
} EstadoAnimacao;

typedef struct {
  ModoFerramenta ferramenta_atual;
  EstadoAnimacao animacao;
  float cor_atual[3];
  float espessura_atual;

  Ponto vertices_temp[MAX_VERTICES];
  int qtd_vertices_temp;
  int construindo_poligono;

  Ponto p1_temp;
  int tem_p1_temp;
} EstadoInterface;

typedef struct {
  CenaGrafica cena;
  EstadoInterface ui;
} ContextoPaint;

void inicializarContexto(ContextoPaint *ctx);

int adicionarPonto(CenaGrafica *cena, float x, float y, float cor[3],
                   float tamanho);
int adicionarReta(CenaGrafica *cena, Ponto p1, Ponto p2, float cor[3],
                  float espessura);
int adicionarPoligono(CenaGrafica *cena, Ponto vertices[], int qtd_vertices,
                      float cor_contorno[3], float cor_preenchimento[3],
                      int preenchido, float espessura_contorno);

void desenharCena(CenaGrafica *cena);
void desenharPonto(PontoDesenho *p);
void desenharReta(Reta *r);
void desenharPoligono(Poligono *p);

void selecionarObjeto(CenaGrafica *cena, float mouseX, float mouseY);
void excluirObjetoSelecionado(CenaGrafica *cena, float mouseX, float mouseY);
void excluirObjetosSelecionados(CenaGrafica *cena);
void desselecionarTodos(CenaGrafica *cena);

void transladarObjeto(CenaGrafica *cena, float tx, float ty);

void rotacionarObjeto(CenaGrafica *cena, float angulo);

void escalarObjeto(CenaGrafica *cena, float sx, float sy);

void refletirObjeto(CenaGrafica *cena, EixoReflexao eixo);

void cisalharObjeto(CenaGrafica *cena, float shx, float shy);

int salvarCena(CenaGrafica *cena, const char *nomeArquivo);
int carregarCena(CenaGrafica *cena, const char *nomeArquivo);

Ponto calcularCentro(Ponto vertices[], int qtd);
float distanciaPontos(Ponto a, Ponto b);

#endif
