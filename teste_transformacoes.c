#include <stdio.h>
#include <stdlib.h>
#include <GL/freeglut.h>
#include "Estruturas.h"

CenaGrafica cena;

void inicializarCenaTeste()
{
    float preto[3] = {0.0f, 0.0f, 0.0f};
    float azul[3]  = {0.0f, 0.0f, 1.0f};

    cena.qtd_pontos = 0;
    cena.qtd_retas = 0;
    cena.qtd_poligonos = 0;

    // Ponto
    adicionarPonto(&cena, -100, -100, preto, 5);
    cena.pontos[0].selecionado = 1;

    // Reta
    adicionarReta(
        &cena,
        (Ponto){-150, 50},
        (Ponto){-50, 50},
        preto,
        2
    );
    cena.retas[0].selecionado = 1;

    // Triângulo
    Ponto vertices[3] = {
        {50, 50},
        {150, 50},
        {100, 150}
    };

    adicionarPoligono(
        &cena,
        vertices,
        3,
        preto,
        azul,
        0,
        2
    );

    cena.poligonos[0].selecionado = 1;
}

void desenharCenaTeste()
{
    int i;

    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(0.0f, 0.0f, 0.0f);

    // Pontos
    for(i = 0; i < cena.qtd_pontos; i++)
    {
        glPointSize(cena.pontos[i].tamanho);

        glBegin(GL_POINTS);
        glVertex2f(
            cena.pontos[i].posicao.x,
            cena.pontos[i].posicao.y
        );
        glEnd();
    }

    // Retas
    for(i = 0; i < cena.qtd_retas; i++)
    {
        glLineWidth(cena.retas[i].espessura);

        glBegin(GL_LINES);
        glVertex2f(cena.retas[i].p1.x, cena.retas[i].p1.y);
        glVertex2f(cena.retas[i].p2.x, cena.retas[i].p2.y);
        glEnd();
    }

    // Polígonos
    for(i = 0; i < cena.qtd_poligonos; i++)
    {
        int j;

        glBegin(GL_LINE_LOOP);

        for(j = 0; j < cena.poligonos[i].qtd_vertices; j++)
        {
            glVertex2f(
                cena.poligonos[i].vertices[j].x,
                cena.poligonos[i].vertices[j].y
            );
        }

        glEnd();
    }

    glutSwapBuffers();
}

void teclado(unsigned char key, int x, int y)
{
    switch(key)
    {
        case 't':
            transladarObjeto(&cena, 20, 20);
            printf("Translacao\n");
            break;

        case 'r':
            rotacionarObjeto(&cena, 15);
            printf("Rotacao\n");
            break;

        case 'e':
            escalarObjeto(&cena, 1.2f, 1.2f);
            printf("Escala\n");
            break;

        case 'x':
            refletirObjeto(&cena, REFLEXAO_X);
            printf("Reflexao X\n");
            break;

        case 'y':
            refletirObjeto(&cena, REFLEXAO_Y);
            printf("Reflexao Y\n");
            break;

        case 'o':
            refletirObjeto(&cena, REFLEXAO_ORIGEM);
            printf("Reflexao Origem\n");
            break;

        case 'c':
            cisalharObjeto(&cena, 0.2f, 0.0f);
            printf("Cisalhamento\n");
            break;

        case 27:
            exit(0);
    }

    glutPostRedisplay();
}

void init()
{
    glClearColor(1, 1, 1, 1);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluOrtho2D(-300, 300, -300, 300);
}

int main(int argc, char **argv)
{
    printf("\n===== TESTE DAS TRANSFORMACOES =====\n");
    printf("t -> Translacao\n");
    printf("r -> Rotacao\n");
    printf("e -> Escala\n");
    printf("x -> Reflexao X\n");
    printf("y -> Reflexao Y\n");
    printf("o -> Reflexao Origem\n");
    printf("c -> Cisalhamento\n");
    printf("ESC -> Sair\n\n");

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

    glutInitWindowSize(800, 600);
    glutCreateWindow("Teste Transformacoes");

    init();
    inicializarCenaTeste();

    glutDisplayFunc(desenharCenaTeste);
    glutKeyboardFunc(teclado);

    glutMainLoop();

    return 0;
}
