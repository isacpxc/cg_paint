#ifndef ESTRUTURAS_H
#define ESTRUTURAS_H

#define MAX_VERTICES 200
#define MAX_OBJETOS 1000

#ifdef _WIN32
    #include <windows.h>
#endif
#include <GL/glut.h>

// ============================================================================
// 1. ENUMS
// ============================================================================

// Ferramentas disponiveis (Lapis, Reta, Poligono, Selecao, Borracha)
typedef enum {
    MODO_PONTO,      // Equivalente ao "Lapis" para criar pontos
    MODO_RETA,       // Ferramenta de criar retas
    MODO_POLIGONO,   // Ferramenta de criar poligonos
    MODO_SELECAO,    // Ferramenta de selecionar objetos
    MODO_BORRACHA    // Ferramenta de exclusao (clicou, apagou)
} ModoFerramenta;

// Tipo de objeto (util para funcoes genericas de selecao/exclusao)
typedef enum {
    TIPO_PONTO,
    TIPO_RETA,
    TIPO_POLIGONO
} TipoObjeto;

// Eixo de reflexao
typedef enum {
    REFLEXAO_X,      // Reflexao em relacao ao eixo X
    REFLEXAO_Y,      // Reflexao em relacao ao eixo Y
    REFLEXAO_ORIGEM  // Reflexao em relacao a origem
} EixoReflexao;

// ============================================================================
// 2. ESTRUTURAS GEOMETRICAS
// ============================================================================

// Coordenada pura (usada internamente pelas outras structs)
typedef struct {
    float x;
    float y;
} Ponto;

// Matriz 3x3 para operacoes de transformacao em coordenadas homogeneas 2D
typedef struct {
    float m[3][3];
} Matriz3x3;

// Ponto desenhavel na tela (com propriedades visuais)
typedef struct {
    Ponto posicao;
    float cor[3];       // Cor RGB [R, G, B]
    float tamanho;      // Tamanho do ponto (glPointSize)
    int selecionado;    // 0 = nao selecionado, 1 = selecionado
} PontoDesenho;

// Segmento de reta (com propriedades visuais)
typedef struct {
    Ponto p1;
    Ponto p2;
    float cor[3];       // Cor RGB [R, G, B]
    float espessura;    // Espessura da linha (glLineWidth)
    int selecionado;    // 0 = nao selecionado, 1 = selecionado
} Reta;

// Poligono (com propriedades visuais)
typedef struct {
    Ponto vertices[MAX_VERTICES];
    int qtd_vertices;
    float cor_contorno[3];      // Cor RGB do contorno
    float cor_preenchimento[3]; // Cor RGB do preenchimento
    int preenchido;             // 0 = so contorno, 1 = preenchido
    float espessura_contorno;   // Espessura das arestas
    int selecionado;            // 0 = nao selecionado, 1 = selecionado
} Poligono;

// ============================================================================
// 3. CENA GRAFICA (contem todos os objetos desenhados)
// ============================================================================

typedef struct {
    PontoDesenho pontos[MAX_OBJETOS];
    int qtd_pontos;

    Reta retas[MAX_OBJETOS];
    int qtd_retas;

    Poligono poligonos[MAX_OBJETOS];
    int qtd_poligonos;
} CenaGrafica;

// ============================================================================
// 4. ESTADO DA ANIMACAO
// ============================================================================

typedef struct {
    int ativa;          // 0 = Animacao pausada, 1 = Rodando
    float tempo;        // Contador de tempo ou angulo para o loop da animacao
    float velocidade;   // Velocidade do movimento
} EstadoAnimacao;

// ============================================================================
// 5. ESTADO DA INTERFACE / APLICACAO
// ============================================================================

typedef struct {
    ModoFerramenta ferramenta_atual; // Qual botao esta ativo (Lapis, Borracha, etc)
    EstadoAnimacao animacao;         // Estado atual da animacao
    float cor_atual[3];              // Cor selecionada para os proximos desenhos [R, G, B]
    float espessura_atual;           // Espessura selecionada para proximos desenhos

    // Estado temporario para construcao de poligono
    Ponto vertices_temp[MAX_VERTICES]; // Vertices sendo adicionados pelo usuario
    int qtd_vertices_temp;             // Quantos vertices ja foram clicados
    int construindo_poligono;          // 1 = esta no meio de criar um poligono

    // Estado temporario para construcao de reta
    Ponto p1_temp;                     // Primeiro ponto da reta sendo criada
    int tem_p1_temp;                   // 1 = ja clicou o primeiro ponto da reta
} EstadoInterface;

// ============================================================================
// 6. CONTEXTO GLOBAL DO PAINT (une tudo em um unico lugar)
// ============================================================================

typedef struct {
    CenaGrafica cena;
    EstadoInterface ui;  // "interface" e palavra reservada no Windows (rpcdce.h)
} ContextoPaint;

// ============================================================================
// 7. PROTOTIPOS DE FUNCOES
// ============================================================================

// --- Inicializacao ---
void inicializarContexto(ContextoPaint *ctx);

// --- Criacao de objetos ---
int adicionarPonto(CenaGrafica *cena, float x, float y, float cor[3], float tamanho);
void adicionarReta(CenaGrafica *cena, Ponto p1, Ponto p2, float cor[3], float espessura);
void adicionarPoligono(CenaGrafica *cena, Ponto vertices[], int qtd_vertices,
                       float cor_contorno[3], float cor_preenchimento[3],
                       int preenchido, float espessura_contorno);

// --- Desenho (renderizacao OpenGL) ---
void desenharCena(CenaGrafica *cena);
void desenharPonto(PontoDesenho *p);
void desenharReta(Reta *r);
void desenharPoligono(Poligono *p);

// --- Selecao/Exclusao ---
void selecionarObjeto(CenaGrafica *cena, float mouseX, float mouseY);
void excluirObjetosSelecionados(CenaGrafica *cena);
void desselecionarTodos(CenaGrafica *cena);

// --- Transformacoes geometricas (exigidas pelo trabalho) ---
// Translacao: arrastar e soltar
void transladarObjeto(CenaGrafica *cena, float tx, float ty);

// Rotacao: em relacao ao centro do objeto (ponto em relacao a origem)
void rotacionarObjeto(CenaGrafica *cena, float angulo);

// Escala: em relacao ao centro do objeto (nao existe para ponto)
void escalarObjeto(CenaGrafica *cena, float sx, float sy);

// Reflexao: em relacao a um eixo
void refletirObjeto(CenaGrafica *cena, EixoReflexao eixo);

// Cisalhamento
void cisalharObjeto(CenaGrafica *cena, float shx, float shy);

// --- Salvar/Carregar arquivo ---
void salvarCena(CenaGrafica *cena, const char *nomeArquivo);
void carregarCena(CenaGrafica *cena, const char *nomeArquivo);

// --- Utilidades geometricas ---
Ponto calcularCentro(Ponto vertices[], int qtd);
float distanciaPontos(Ponto a, Ponto b);

#endif
