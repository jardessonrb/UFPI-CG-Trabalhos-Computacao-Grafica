#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#include <bits/stdc++.h>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <forward_list>
#include "glut_text.h"
#include "bresenham.h"
#include "rasterizacao_circulo.h"

using namespace std;

#define ESC 27

bool isPrimeiroClick = false;
int contadorClicks = 0;
int larguraJanela = 700;
int alturaJanela = 700;
int mouseCoordenadaAtualX;
int mouseCoordenadaAtualY;

int coordenadaPrimeiroClickX;
int coordenadaPrimeiroClickY;
int coordenadaSegundoClickX;
int coordenadaSegundoClickY;

enum tipo_forma {
    LINHA = 1,
    TRIANGULO = 2,
    RETA = 3,
    POLIGANO = 4,
    CIRCULO = 5
};


struct Vertice {
    int coordenadaX;
    int coordenadaY;

    Vertice(int x, int y) {
        this->coordenadaX = x;
        this->coordenadaY = y;
    }
};


struct Forma {
    int posicao;
    int tipoForma;
    vector<Vertice> vertices;

    Forma(int posicao, tipo_forma tipoForma, vector<Vertice> vertices) {
        this->posicao = posicao;
        this->tipoForma = tipoForma;
        this->vertices = vertices;
    }

    void addVertice(int x, int y) {
        vertices.push_back({ x, y });
    }

    Forma(int posicao, tipo_forma tipoForma, vector<pair<int, int>> verticesPair) {
        this->posicao = posicao;
        this->tipoForma = tipoForma;

        vector<Vertice> vertices;
        for (auto vertice : verticesPair) {
            vertices.emplace_back(vertice.first, vertice.second);
        }
        this->vertices = vertices;
    }

    vector<Vertice> getVertices() const {
        return vertices;
    }
};

class Paint {

public:
    vector<Forma> formas;

    void addForma(Forma forma) {
        formas.push_back(forma);
    }

    vector<Forma> getFormas() {
        return this->formas;
    }
};

Paint paint;
tipo_forma formaAtual = LINHA;
vector<Vertice> verticesFormaCorrent;

void salvarFigura(tipo_forma forma);
void init(void);
void reshape(int w, int h);
void display(void);
void keyboard(unsigned char key, int x, int y);
void movimentacaoMouseComClick(int button, int state, int x, int y);
void movimentoMousePassivo(int x, int y);
void drawPixel(int x, int y);
void drawFormas();
// void retaImediata(double x1, double y1, double x2, double y2);
// void bresenham(int x1, int y1, int x2, int y2);
// void menu_popup(int value);



int main(int argc, char** argv)
{
    glutInit(&argc, argv); // Passagens de parametro C para o glut
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); //Selecao do Modo do Display e do Sistema de cor
    glutInitWindowSize(larguraJanela, alturaJanela);  // Tamanho da janela do OpenGL
    glutInitWindowPosition(100, 100); //Posicao inicial da janela do OpenGL
    glutCreateWindow("Computacao Grafica: Paint"); // Da nome para uma janela OpenGL
    init();


    glutKeyboardFunc(keyboard);
    glutMouseFunc(movimentacaoMouseComClick);
    glutPassiveMotionFunc(movimentoMousePassivo);

    glutReshapeFunc(reshape);
    glutDisplayFunc(display);

    glutMainLoop();
    return EXIT_SUCCESS;
}

void init(void) {
    glClearColor(1.0, 1.0, 1.0, 1.0); //Limpa a tela com a cor branca;
}

void movimentacaoMouseComClick(int button, int state, int x, int y) {
    switch (button) {
    case GLUT_LEFT_BUTTON:
        switch (formaAtual)
        {
        case LINHA:
            if (state == GLUT_DOWN) {
                verticesFormaCorrent.emplace_back(x, alturaJanela - y - 1);
                contadorClicks++;
                if (isPrimeiroClick) {
                    isPrimeiroClick = !isPrimeiroClick;
                    salvarFigura(LINHA);
                    contadorClicks = 0;
                    glutPostRedisplay();
                }
                else {
                    drawPixel(x, y);
                    isPrimeiroClick = !isPrimeiroClick;
                }


            }
            break;
        case CIRCULO:
            if (state == GLUT_DOWN) {
                verticesFormaCorrent.emplace_back(x, alturaJanela - y - 1);
                contadorClicks++;
                if (isPrimeiroClick) {
                    isPrimeiroClick = !isPrimeiroClick;
                    salvarFigura(CIRCULO);
                    contadorClicks = 0;
                    glutPostRedisplay();
                }
                else {
                    drawPixel(x, y);
                    isPrimeiroClick = !isPrimeiroClick;
                }
                break;
            }

        case TRIANGULO:
            if (state == GLUT_DOWN) {
                verticesFormaCorrent.emplace_back(x, alturaJanela - y - 1);
                contadorClicks++;
                if (contadorClicks == 3) {
                    salvarFigura(TRIANGULO);
                    contadorClicks = 0;
                    glutPostRedisplay();
                }
                break;
            }

        default:
            break;
        }
        break;

    default:
        break;
    }
}

void salvarTriangulo() {
    Vertice vertice1 = verticesFormaCorrent[0];
    Vertice vertice2 = verticesFormaCorrent[1];
    Vertice vertice3 = verticesFormaCorrent[2];

    vector<pair<int, int>> verticesPrimeiroLado = calcularBresenhamPrimeiroOctante(vertice1.coordenadaX, vertice1.coordenadaY, vertice2.coordenadaX, vertice2.coordenadaY);
    vector<pair<int, int>> verticesSegundoLado = calcularBresenhamPrimeiroOctante(vertice2.coordenadaX, vertice2.coordenadaY, vertice3.coordenadaX, vertice3.coordenadaY);
    vector<pair<int, int>> verticesTerceiroLado = calcularBresenhamPrimeiroOctante(vertice3.coordenadaX, vertice3.coordenadaY, vertice1.coordenadaX, vertice1.coordenadaY);

    drawPixel(vertice1.coordenadaX, vertice1.coordenadaY);
    drawPixel(vertice2.coordenadaX, vertice2.coordenadaY);
    drawPixel(vertice3.coordenadaX, vertice3.coordenadaY);

    for (pair<int, int> vertice2 : verticesSegundoLado) {
        verticesPrimeiroLado.push_back(vertice2);
    }

    for (pair<int, int> vertice3 : verticesTerceiroLado) {
        verticesPrimeiroLado.push_back(vertice3);
    }

    Forma forma(paint.formas.size(), TRIANGULO, verticesPrimeiroLado);
    paint.addForma(forma);
}

void salvarLinha() {
    Vertice vertice1 = verticesFormaCorrent[0];
    Vertice vertice2 = verticesFormaCorrent[1];
    vector<pair<int, int>> verticesPair = calcularBresenhamPrimeiroOctante(vertice1.coordenadaX, vertice1.coordenadaY, vertice2.coordenadaX, vertice2.coordenadaY);
    drawPixel(vertice1.coordenadaX, vertice1.coordenadaY);
    drawPixel(vertice2.coordenadaX, vertice2.coordenadaY);

    Forma forma(paint.formas.size(), LINHA, verticesPair);
    paint.addForma(forma);
}

void salvarCirculo() {
    Vertice vertice1 = verticesFormaCorrent[0];
    Vertice vertice2 = verticesFormaCorrent[1];
    vector<pair<int, int>> verticesPair = salvarCirculo(vertice1.coordenadaX, vertice1.coordenadaY, vertice2.coordenadaX, vertice2.coordenadaY);
    drawPixel(vertice1.coordenadaX, vertice1.coordenadaY);
    drawPixel(vertice2.coordenadaX, vertice2.coordenadaY);

    Forma forma(paint.formas.size(), CIRCULO, verticesPair);
    paint.addForma(forma);
}

void salvarFigura(tipo_forma tipoForma) {
    if (tipoForma == LINHA) {
        salvarLinha();
    }

    if (tipoForma == CIRCULO) {
        salvarCirculo();
    }

    if (tipoForma == TRIANGULO) {
        salvarTriangulo();
    }

    verticesFormaCorrent.clear();
}

void keyboard(unsigned char key, int x, int y) {
    int modifiers = glutGetModifiers();

    if (key == ESC) {
        exit(EXIT_SUCCESS);
    }

    if (key == 'l') {
        formaAtual = LINHA;
    }
    if (key == 'o') {
        formaAtual = CIRCULO;
    }
    if (key == 't') {
        formaAtual = TRIANGULO;
    }

}

void movimentoMousePassivo(int x, int y) {
    mouseCoordenadaAtualX = x;
    mouseCoordenadaAtualY = alturaJanela - y - 1;
    glutPostRedisplay();
}

void reshape(int w, int h)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, w, h);

    larguraJanela = w;
    alturaJanela = h;

    glOrtho(0, w, 0, h, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0, 0.0, 0.0);
    drawFormas();
    draw_text_stroke(0, 0, "(" + to_string(mouseCoordenadaAtualX) + "," + to_string(mouseCoordenadaAtualY) + ")", 0.2);
    glutSwapBuffers();
}

void drawFormas() {
    for (Forma forma : paint.getFormas()) {
        for (Vertice vertice : forma.getVertices()) {
            drawPixel(vertice.coordenadaX, vertice.coordenadaY);
        }
    }
}

void drawPixel(int x, int y) {
    glBegin(GL_POINTS);
    glVertex2f(x, y);
    glEnd();
}