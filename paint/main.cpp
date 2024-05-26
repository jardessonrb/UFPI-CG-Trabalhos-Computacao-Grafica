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
#include "transformacoes.h"

using namespace std;

#define ESC 27

bool isAplicarEscala = false, isAplicarTranslacao = false,
isAplicarCisalhamento = false, isAplicarReflexao = false, isAplicaoRotacao = false, isAplicarPintura = false;
char eixo = '#';
float corAtual[3] = { 0.0f, 0.5f, 0.5f };

double valorCosseno = 0.7, valorSeno = 0.7;

int espessuraLinha = 1;
bool isPrimeiroClick = false;
bool poliganoCompleto = false;
int contadorClicks = 0;
int larguraJanela = 700;
int alturaJanela = 700;
int mouseCoordenadaAtualX;
int mouseCoordenadaAtualY;

int coordenadaPrimeiroClickX;
int coordenadaPrimeiroClickY;
int coordenadaSegundoClickX;
int coordenadaSegundoClickY;

int dx[] = { 0, 1, 0, -1 };
int dy[] = { -1, 0, 1, 0 };

enum tipo_forma {
    LINHA = 1,
    TRIANGULO = 2,
    RETANGULO = 3,
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

    bool operator==(const Vertice& other) const {
        return coordenadaX == other.coordenadaX && coordenadaY == other.coordenadaY;
    }
};


struct Forma {
    int posicao;
    int tipoForma;
    bool isPintada = false;
    float cor[3] = {};
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

    void definirCor(float r, float g, float b) {
        isPintada = true;
        cor[0] = r;
        cor[1] = g;
        cor[2] = b;
    }

    vector<Vertice> getVertices() {
        return vertices;
    }

    void aplicarEscala(double valorEscala) {
        for (Vertice& vertice : this->vertices) {
            pair<int, int> novoValorVertice = escala(vertice.coordenadaX, vertice.coordenadaY, valorEscala);
            vertice.coordenadaX = novoValorVertice.first;
            vertice.coordenadaY = novoValorVertice.second;
        }
    }

    void aplicarTranslacao(double valorTx, double valorTy) {
        for (Vertice& vertice : this->vertices) {
            pair<int, int> novoValorVertice = translacao(vertice.coordenadaX, vertice.coordenadaY, valorTx, valorTy);
            vertice.coordenadaX = novoValorVertice.first;
            vertice.coordenadaY = novoValorVertice.second;
        }
    }

    void aplicarCisalhamento(double valorCisalhamento) {
        for (Vertice& vertice : this->vertices) {
            pair<int, int> novoValorVertice = cisalhamento(vertice.coordenadaX, vertice.coordenadaY, valorCisalhamento);
            vertice.coordenadaX = novoValorVertice.first;
            vertice.coordenadaY = novoValorVertice.second;
        }
    }

    void aplicarRotacao() {
        for (Vertice& vertice : this->vertices) {
            pair<int, int> novoValorVertice = rotacao(vertice.coordenadaX, vertice.coordenadaY, valorCosseno, valorSeno);
            vertice.coordenadaX = novoValorVertice.first;
            vertice.coordenadaY = novoValorVertice.second;
        }
    }

    void aplicarReflexao() {
        for (Vertice& vertice : this->vertices) {
            pair<int, int> novoValorVertice = reflexao(vertice.coordenadaX, vertice.coordenadaY, eixo);
            vertice.coordenadaX = novoValorVertice.first;
            vertice.coordenadaY = novoValorVertice.second;
        }
    }

    double minDistanciaAOrigem(int x, int y) {
        return sqrt(x * x + y * y);
    }

    pair<int, int> calcularMenorVertice() {
        double minDistancia = this->minDistanciaAOrigem(this->vertices[0].coordenadaX, this->vertices[0].coordenadaY);
        int x = this->vertices[0].coordenadaX, y = this->vertices[0].coordenadaY;
        for (Vertice v : this->vertices) {
            double distanciaAtual = this->minDistanciaAOrigem(v.coordenadaX, v.coordenadaY);

            if (minDistancia > distanciaAtual) {
                minDistancia = distanciaAtual;
                x = v.coordenadaX;
                y = v.coordenadaY;
            }
        }

        return make_pair(x, y);
    }
};

class Paint {

public:
    vector<Forma> formas;
    vector<Vertice> figuraEmConstrucao;

    void addForma(Forma forma) {
        formas.push_back(forma);
    }

    vector<Forma> getFormas() {
        return this->formas;
    }

    void addVerticeFiguraAtual(Vertice vertice) {
        this->figuraEmConstrucao.push_back(vertice);
    }

    vector<Vertice> getFiguraAtual() {
        return this->figuraEmConstrucao;
    }

    void cleanFiguraAtual() {
        this->figuraEmConstrucao.clear();
    }

    void aplicarEscala(double valorEscala) {
        for (Forma& forma : this->formas) {
            auto [x, y] = forma.calcularMenorVertice();
            forma.aplicarTranslacao(-x, -y);
            forma.aplicarEscala(valorEscala);
            forma.aplicarTranslacao(x, y);
        }
        isAplicarEscala = false;
    }

    void aplicarTranslacao(double valorTx, double valorTy) {
        for (Forma& forma : this->formas) {
            forma.aplicarTranslacao(valorTx, valorTy);
        }
        isAplicarTranslacao = false;
    }

    void aplicarCisalhamento(double valorCisalhamento) {
        for (Forma& forma : this->formas) {
            auto [x, y] = forma.calcularMenorVertice();
            forma.aplicarTranslacao(-x, -y);
            forma.aplicarCisalhamento(valorCisalhamento);
            forma.aplicarTranslacao(x, y);
        }
        isAplicarCisalhamento = false;
    }

    void aplicarRotacao() {
        for (Forma& forma : this->formas) {
            int x, y;
            Vertice vertice = forma.vertices[0];
            x = vertice.coordenadaX;
            y = vertice.coordenadaY;

            forma.aplicarTranslacao(-x, -y);
            forma.aplicarRotacao();
            forma.aplicarTranslacao(x, y);
        }
        isAplicaoRotacao = false;
    }

    void aplicarReflexao() {
        for (Forma& forma : this->formas) {
            forma.aplicarReflexao();
        }
        isAplicarReflexao = false;
    }
};

Paint paint;
tipo_forma formaAtual = LINHA;
vector<Vertice> verticesFormaCorrent;

void reiniciarVariaveisAplicacoes();
void drawPreview(vector<pair<int, int>> vertices);
void salvarFigura(tipo_forma forma);
void init(void);
void reshape(int w, int h);
void display(void);
void keyboard(unsigned char key, int x, int y);
void movimentacaoMouseComClick(int button, int state, int x, int y);
void movimentoMousePassivo(int x, int y);
void drawPixel(int x, int y);
void drawPixelPintado(int x, int y, float cor[]);
void drawFormas();
void algoritmoFloodFill(int x, int y);
void desenharMenuOpcoes();
void adicionarTransformacoes();
void desenharQuadroDeCores();
void desenharQuadroOpcoes();
void mapearClickNaAreaOpcoes(int x, int y);
void desenharOpcoesEixoEPintura();

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
        if ((alturaJanela - y - 1) >= 600) {
            mapearClickNaAreaOpcoes(x, (alturaJanela - y - 1));
        }
        else {

            if (isAplicarPintura) {
                algoritmoFloodFill(x, alturaJanela - y - 1);
                isAplicarPintura = false;
                return;
            }

            switch (formaAtual)
            {
            case LINHA:
                if (state == GLUT_DOWN) {
                    drawPixel(x, alturaJanela - y - 1);
                    verticesFormaCorrent.emplace_back(x, alturaJanela - y - 1);
                    contadorClicks++;
                    if (isPrimeiroClick) {
                        isPrimeiroClick = !isPrimeiroClick;
                        salvarFigura(LINHA);
                        contadorClicks = 0;
                        glutPostRedisplay();
                    }
                    else {
                        isPrimeiroClick = !isPrimeiroClick;
                    }


                }
                break;
            case CIRCULO:
                if (state == GLUT_DOWN) {
                    drawPixel(x, alturaJanela - y - 1);
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
                    drawPixel(x, alturaJanela - y - 1);
                    verticesFormaCorrent.emplace_back(x, alturaJanela - y - 1);
                    contadorClicks++;
                    if (contadorClicks == 3) {
                        salvarFigura(TRIANGULO);
                        contadorClicks = 0;
                        glutPostRedisplay();
                    }
                    break;
                }

            case RETANGULO:
                if (state == GLUT_DOWN) {
                    drawPixel(x, alturaJanela - y - 1);
                    verticesFormaCorrent.emplace_back(x, alturaJanela - y - 1);
                    contadorClicks++;
                    if (contadorClicks == 2) {
                        salvarFigura(RETANGULO);
                        contadorClicks = 0;
                        glutPostRedisplay();
                    }
                    break;
                }

            case POLIGANO:
                if (state == GLUT_DOWN) {
                    int xAtual = x;
                    int yAtual = alturaJanela - y - 1;
                    drawPixel(xAtual, yAtual);
                    verticesFormaCorrent.emplace_back(xAtual, yAtual);
                    contadorClicks++;

                    if (contadorClicks > 1) {
                        auto [x1, y1] = verticesFormaCorrent[verticesFormaCorrent.size() - 2];
                        auto [x2, y2] = verticesFormaCorrent[verticesFormaCorrent.size() - 1];
                        vector<pair<int, int>> vertices = calcularBresenhamPrimeiroOctante(x1, y1, x2, y2);
                        for (auto v : vertices) {
                            Vertice vertice(v.first, v.second);
                            paint.addVerticeFiguraAtual(vertice);
                        }
                        glutPostRedisplay();
                    }

                    if (contadorClicks > 2) {
                        Vertice vertice = verticesFormaCorrent[0];
                        if ((vertice.coordenadaX == xAtual && vertice.coordenadaY == yAtual) or poliganoCompleto) {
                            salvarFigura(POLIGANO);
                            contadorClicks = 0;
                            poliganoCompleto = false;
                            glutPostRedisplay();
                        }
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
}

void rasterizarPoligano(Forma& forma) {
    vector<Vertice> vertices;
    for (int i = 1; i < forma.vertices.size(); i++)
    {
        Vertice anterior = forma.vertices[i - 1];
        Vertice atual = forma.vertices[i];
        vector<pair<int, int>> verticesBresenham = calcularBresenhamPrimeiroOctante(anterior.coordenadaX, anterior.coordenadaY, atual.coordenadaX, atual.coordenadaY);
        for (auto [x, y] : verticesBresenham) {
            drawPixel(x, y);
        }
    }

    Vertice primeiroVertice = forma.vertices[0];
    Vertice ultimoVertice = forma.vertices[forma.vertices.size() - 1];
    vector<pair<int, int>> verticesBresenham = calcularBresenhamPrimeiroOctante(ultimoVertice.coordenadaX, ultimoVertice.coordenadaY, primeiroVertice.coordenadaX, primeiroVertice.coordenadaY);
    for (auto [x, y] : verticesBresenham) {
        drawPixel(x, y);
    }
}

void rasterizarRetangulo(Forma& forma) {
    Vertice vertice1 = forma.vertices[0];
    Vertice vertice2 = forma.vertices[1];

    int x1 = vertice1.coordenadaX;
    int y1 = vertice1.coordenadaY;

    int x2 = vertice2.coordenadaX;
    int y2 = vertice2.coordenadaY;

    vector<pair<int, int>> vertices1 = calcularBresenhamPrimeiroOctante(x1, y1, x2, y1);
    vector<pair<int, int>> vertices2 = calcularBresenhamPrimeiroOctante(x2, y1, x2, y2);
    vector<pair<int, int>> vertices3 = calcularBresenhamPrimeiroOctante(x2, y2, x1, y2);
    vector<pair<int, int>> vertices4 = calcularBresenhamPrimeiroOctante(x1, y2, x1, y1);

    vector<vector<pair<int, int>>> vectorsVertices({ vertices1, vertices2, vertices3, vertices4 });
    vector<Vertice> vertices;

    for (auto [x, y] : vertices1)
        drawPixel(x, y);

    for (auto [x, y] : vertices2)
        drawPixel(x, y);

    for (auto [x, y] : vertices3)
        drawPixel(x, y);

    for (auto [x, y] : vertices4)
        drawPixel(x, y);

}

void rasterizarTriangulo(Forma& forma) {
    Vertice vertice1 = forma.vertices[0];
    Vertice vertice2 = forma.vertices[1];
    Vertice vertice3 = forma.vertices[2];

    vector<pair<int, int>> verticesPrimeiroLado = calcularBresenhamPrimeiroOctante(vertice1.coordenadaX, vertice1.coordenadaY, vertice2.coordenadaX, vertice2.coordenadaY);
    vector<pair<int, int>> verticesSegundoLado = calcularBresenhamPrimeiroOctante(vertice2.coordenadaX, vertice2.coordenadaY, vertice3.coordenadaX, vertice3.coordenadaY);
    vector<pair<int, int>> verticesTerceiroLado = calcularBresenhamPrimeiroOctante(vertice3.coordenadaX, vertice3.coordenadaY, vertice1.coordenadaX, vertice1.coordenadaY);

    drawPixel(vertice1.coordenadaX, vertice1.coordenadaY);
    drawPixel(vertice2.coordenadaX, vertice2.coordenadaY);
    drawPixel(vertice3.coordenadaX, vertice3.coordenadaY);

    for (auto [x, y] : verticesPrimeiroLado)
        drawPixel(x, y);

    for (auto [x, y] : verticesSegundoLado)
        drawPixel(x, y);

    for (auto [x, y] : verticesTerceiroLado)
        drawPixel(x, y);
}

void rasterizarLinha(Forma& forma) {
    Vertice vertice1 = forma.vertices[0];
    Vertice vertice2 = forma.vertices[1];
    vector<pair<int, int>> verticesPair = calcularBresenhamPrimeiroOctante(vertice1.coordenadaX, vertice1.coordenadaY, vertice2.coordenadaX, vertice2.coordenadaY);
    drawPixel(vertice1.coordenadaX, vertice1.coordenadaY);
    drawPixel(vertice2.coordenadaX, vertice2.coordenadaY);

    for (auto [x, y] : verticesPair)
    {
        drawPixel(x, y);
    }

}

void rasterizarCirculo(Forma& forma) {
    Vertice vertice1 = forma.vertices[0];
    Vertice vertice2 = forma.vertices[1];
    vector<pair<int, int>> verticesPair = salvarCirculo(vertice1.coordenadaX, vertice1.coordenadaY, vertice2.coordenadaX, vertice2.coordenadaY);
    drawPixel(vertice1.coordenadaX, vertice1.coordenadaY);
    drawPixel(vertice2.coordenadaX, vertice2.coordenadaY);

    for (auto [x, y] : verticesPair)
        drawPixel(x, y);
}

void salvarFigura(tipo_forma tipoForma) {
    Forma forma(paint.formas.size(), tipoForma, verticesFormaCorrent);
    paint.addForma(forma);

    verticesFormaCorrent.clear();
    paint.figuraEmConstrucao.clear();
}

void keyboard(unsigned char key, int x, int y) {
    int modifiers = glutGetModifiers();

    if (key == ESC) {
        exit(EXIT_SUCCESS);
    }

    // if (key == 'l') {
    //     formaAtual = LINHA;
    // }
    // if (key == 'c') {
    //     formaAtual = CIRCULO;
    // }
    // if (key == 't') {
    //     formaAtual = TRIANGULO;
    // }
    // if (key == 'r') {
    //     formaAtual = RETANGULO;
    // }
    // if (key == 'p') {
    //     formaAtual = POLIGANO;
    // }
    // if (key == '+') {
    //     espessuraLinha++;
    // }

    // if (key == '-') {
    //     if (espessuraLinha > 1) {
    //         espessuraLinha--;
    //     }
    // }

    // if (key == 'e') {
    //     isAplicarEscala = true;
    // }

    // if (key == 'j') {
    //     isAplicarTranslacao = true;
    // }

    // if (key == 'q') {
    //     isAplicarCisalhamento = true;
    // }

    // if (key == 'w' && eixo != '#') {
    //     isAplicarReflexao = true;
    // }

    // if (key == 'b') {
    //     isAplicaoRotacao = true;
    // }

    // if (key == 'f' && !poliganoCompleto && verticesFormaCorrent.size() > 2) {
    //     poliganoCompleto = true;
    // }

    // if (key == 'y' or key == 'x' or key == 'Y' or key == 'X') {
    //     eixo = key;
    // }

    // if (key == 'z') {
    //     isAplicarPintura = true;
    // }
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
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);


    glClearColor(1.0, 1.0, 1.0, 1.0);
    glColor3f(0.0, 0.0, 0.0);
    drawFormas();
    draw_text_stroke(0, 0, "(" + to_string(mouseCoordenadaAtualX) + "," + to_string(mouseCoordenadaAtualY) + ")", 0.2);

    glColor3f(0.85, 0.85, 0.85);
    glPointSize(1);
    glLineWidth(1);
    glRectf(0, alturaJanela, larguraJanela, alturaJanela - 100);

    desenharQuadroDeCores();

    desenharOpcoesEixoEPintura();

    desenharQuadroOpcoes();

    desenharMenuOpcoes();
    adicionarTransformacoes();

    glutSwapBuffers();
}

void drawFormas() {
    if (isAplicarEscala) {
        paint.aplicarEscala(2.0);
    }

    if (isAplicarTranslacao) {
        paint.aplicarTranslacao(20.0, 20.0);
    }

    if (isAplicarCisalhamento) {
        paint.aplicarCisalhamento(2);
    }

    if (isAplicaoRotacao) {
        paint.aplicarRotacao();
    }

    if (isAplicarReflexao) {
        paint.aplicarReflexao();
    }

    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0f, 0.0f, 0.0f);
    for (Forma& forma : paint.getFormas()) {
        if (forma.tipoForma == LINHA) {
            if (forma.isPintada) {
                for (Vertice vertice : forma.getVertices()) {
                    if (forma.isPintada) {
                        drawPixelPintado(vertice.coordenadaX, vertice.coordenadaY, forma.cor);
                    }
                    else {
                        drawPixel(vertice.coordenadaX, vertice.coordenadaY);
                    }
                }
            }
            rasterizarLinha(forma);
        }

        if (forma.tipoForma == TRIANGULO) {
            if (forma.isPintada) {
                for (Vertice vertice : forma.getVertices()) {
                    if (forma.isPintada) {
                        drawPixelPintado(vertice.coordenadaX, vertice.coordenadaY, forma.cor);
                    }
                    else {
                        drawPixel(vertice.coordenadaX, vertice.coordenadaY);
                    }
                }
            }
            rasterizarTriangulo(forma);
        }

        if (forma.tipoForma == RETANGULO) {
            if (forma.isPintada) {
                for (Vertice vertice : forma.getVertices()) {
                    if (forma.isPintada) {
                        drawPixelPintado(vertice.coordenadaX, vertice.coordenadaY, forma.cor);
                    }
                    else {
                        drawPixel(vertice.coordenadaX, vertice.coordenadaY);
                    }
                }
            }
            rasterizarRetangulo(forma);
        }

        if (forma.tipoForma == POLIGANO) {
            if (forma.isPintada) {
                for (Vertice vertice : forma.getVertices()) {
                    if (forma.isPintada) {
                        drawPixelPintado(vertice.coordenadaX, vertice.coordenadaY, forma.cor);
                    }
                    else {
                        drawPixel(vertice.coordenadaX, vertice.coordenadaY);
                    }
                }
            }
            rasterizarPoligano(forma);
        }

        if (forma.tipoForma == CIRCULO) {
            if (forma.isPintada) {
                for (Vertice vertice : forma.getVertices()) {
                    if (forma.isPintada) {
                        drawPixelPintado(vertice.coordenadaX, vertice.coordenadaY, forma.cor);
                    }
                    else {
                        drawPixel(vertice.coordenadaX, vertice.coordenadaY);
                    }
                }
            }
            rasterizarCirculo(forma);
        }
    }

    for (Vertice vertice : paint.getFiguraAtual()) {
        drawPixel(vertice.coordenadaX, vertice.coordenadaY);
    }
}

void drawPreview(vector<pair<int, int>> vertices) {
    glPointSize(5);
    glBegin(GL_POINTS);

    for (auto [x, y] : vertices) {
        glVertex2f(x, y);
    }

    glEnd();
}

void drawPixel(int x, int y) {
    glPointSize(espessuraLinha);
    glBegin(GL_POINTS);
    glVertex2f(x, y);
    glEnd();
    glFlush();
}

void drawPixelPintado(int x, int y, float cor[]) {
    glPointSize(espessuraLinha);
    glColor3f(cor[0], cor[1], cor[2]);
    glBegin(GL_POINTS);
    glVertex2f(x, y);
    glEnd();
    glFlush();
}

void reiniciarVariaveisAplicacoes() {
    isAplicarEscala = false;
    isAplicarTranslacao = false;
}

bool isPertencenteAForma(int x, int y, Forma& forma) {
    int n = forma.vertices.size();
    int count = 0, i, j;

    for (i = 0, j = n - 1; i < n; j = i++) {
        int yi = forma.vertices[i].coordenadaY;
        int yj = forma.vertices[j].coordenadaY;
        int xi = forma.vertices[i].coordenadaX;
        int xj = forma.vertices[j].coordenadaX;

        bool intersect = ((yi > y) != (yj > y)) &&
            (x < (xj - xi) * (y - yi) / (double)(yj - yi) + xi);
        if (intersect) {
            count++;
        }
    }

    return count % 2 == 1;
}

void pintarForma(int x, int y, Forma& forma) {
    vector<vector<bool>> visitados(larguraJanela, vector<bool>(alturaJanela, false));
    queue<Vertice> vertices;
    Vertice vertice(x, y);

    vertices.push(vertice);
    visitados[x][y] = true;

    forma.definirCor(corAtual[0], corAtual[1], corAtual[2]);
    while (!vertices.empty()) {
        Vertice atual = vertices.front();
        vertices.pop();
        visitados[atual.coordenadaX][atual.coordenadaY] = true;

        for (int i = 0; i < 4; i++)
        {
            x = atual.coordenadaX + dx[i];
            y = atual.coordenadaY + dy[i];

            if (x >= 0 && x < larguraJanela && y >= 0 && y < alturaJanela) {
                if (!visitados[x][y]) {
                    unsigned char pixelColor[3];
                    glReadPixels(x, y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixelColor);
                    if (pixelColor[0] != 0 && pixelColor[1] != 0 && pixelColor[2] != 0) {
                        visitados[x][y] = true;
                        Vertice novoVertice(x, y);
                        vertices.push(novoVertice);
                        forma.addVertice(x, y);
                    }

                }
            }
        }
    }
}

void algoritmoFloodFill(int x, int y) {
    for (Forma& forma : paint.formas) {
        if (isPertencenteAForma(x, y, forma)) {
            printf("[%d, %d] pertence a forma %d", x, y, forma.posicao);
            pintarForma(x, y, forma);
            return;
        }
    }
}

void desenharCirculo(int x1, int y1, int x2, int y2) {
    vector<std::pair<int, int>> vertices = salvarCirculo(x1, y1, x2, y2);
    for (auto [x, y] : vertices) {
        drawPixel(x, y);
    }
}


void desenharQuadroOpcoes() {
    //Quadrado de opções de formas e transformações
    glColor3f(0, 0, 0);
    glBegin(GL_LINE_LOOP);
    glVertex2f(10, alturaJanela - 10);
    glVertex2f(260, alturaJanela - 10);
    glVertex2f(260, alturaJanela - 90);
    glVertex2f(10, alturaJanela - 90);
    glEnd();

    //Linha do horizontal
    glBegin(GL_LINES);
    glVertex2f(10, alturaJanela - 50);
    glVertex2f(260, alturaJanela - 50);
    glEnd();

    //1° linha vertical
    glBegin(GL_LINES);
    glVertex2f(50, alturaJanela - 10);
    glVertex2f(50, alturaJanela - 90);
    glEnd();

    //2° linha vertical
    glBegin(GL_LINES);
    glVertex2f(100, alturaJanela - 10);
    glVertex2f(100, alturaJanela - 90);
    glEnd();

    //3° linha vertical
    glBegin(GL_LINES);
    glVertex2f(150, alturaJanela - 10);
    glVertex2f(150, alturaJanela - 90);
    glEnd();

    //4° linha vertical
    glBegin(GL_LINES);
    glVertex2f(200, alturaJanela - 10);
    glVertex2f(200, alturaJanela - 90);
    glEnd();

}

void desenharQuadroDeCores() {
    //Quadrado de cores
    glColor3f(0, 0, 0);
    glBegin(GL_LINE_LOOP);
    glVertex2f(350, alturaJanela - 10);
    glVertex2f(600, alturaJanela - 10);
    glVertex2f(600, alturaJanela - 90);
    glVertex2f(350, alturaJanela - 90);
    glEnd();

    //COR
    glColor3f(0.0f, 0.0f, 1.0f);
    glBegin(GL_QUADS);
    glVertex2f(350, alturaJanela - 10);
    glVertex2f(400, alturaJanela - 10);
    glVertex2f(400, alturaJanela - 50);
    glVertex2f(350, alturaJanela - 50);
    glEnd();

    //COR
    glColor3f(0.5f, 0.0f, 0.5f);
    glBegin(GL_QUADS);
    glVertex2f(400, alturaJanela - 10);
    glVertex2f(450, alturaJanela - 10);
    glVertex2f(450, alturaJanela - 50);
    glVertex2f(400, alturaJanela - 50);
    glEnd();

    //COR
    glColor3f(0.54f, 0.17f, 0.89f);
    glBegin(GL_QUADS);
    glVertex2f(450, alturaJanela - 10);
    glVertex2f(500, alturaJanela - 10);
    glVertex2f(500, alturaJanela - 50);
    glVertex2f(450, alturaJanela - 50);
    glEnd();

    glColor3f(1.0f, 1.0f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(500, alturaJanela - 10);
    glVertex2f(550, alturaJanela - 10);
    glVertex2f(550, alturaJanela - 50);
    glVertex2f(500, alturaJanela - 50);
    glEnd();

    glColor3f(0.54f, 0.17f, 0.89f);
    glBegin(GL_QUADS);
    glVertex2f(550, alturaJanela - 10);
    glVertex2f(600, alturaJanela - 10);
    glVertex2f(600, alturaJanela - 50);
    glVertex2f(550, alturaJanela - 50);
    glEnd();

    //Cores de baixo
    //COR
    glColor3f(1.0f, 0.5f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(350, alturaJanela - 50);
    glVertex2f(400, alturaJanela - 50);
    glVertex2f(400, alturaJanela - 90);
    glVertex2f(350, alturaJanela - 90);
    glEnd();

    //COR
    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(400, alturaJanela - 50);
    glVertex2f(450, alturaJanela - 50);
    glVertex2f(450, alturaJanela - 90);
    glVertex2f(400, alturaJanela - 90);
    glEnd();

    //COR
    glColor3f(1.0f, 0.0f, 1.0f);
    glBegin(GL_QUADS);
    glVertex2f(450, alturaJanela - 50);
    glVertex2f(500, alturaJanela - 50);
    glVertex2f(500, alturaJanela - 90);
    glVertex2f(450, alturaJanela - 90);
    glEnd();

    glColor3f(0.0f, 1.0f, 1.0f);
    glBegin(GL_QUADS);
    glVertex2f(500, alturaJanela - 50);
    glVertex2f(550, alturaJanela - 50);
    glVertex2f(550, alturaJanela - 90);
    glVertex2f(500, alturaJanela - 90);
    glEnd();

    glColor3f(0.0f, 0.5f, 0.5f);
    glBegin(GL_QUADS);
    glVertex2f(550, alturaJanela - 50);
    glVertex2f(600, alturaJanela - 50);
    glVertex2f(600, alturaJanela - 90);
    glVertex2f(550, alturaJanela - 90);
    glEnd();

    //Linha do Meio
    glBegin(GL_LINES);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex2f(350, alturaJanela - 50);
    glVertex2f(600, alturaJanela - 50);
    glEnd();

    glColor3f(0.0f, 0.0f, 0.0f);
    draw_text_stroke(620, alturaJanela - 30, "Cor Selecionada", 0.07);
    //Cor Atual selecionada
    glColor3f(corAtual[0], corAtual[1], corAtual[2]);
    glBegin(GL_QUADS);
    glVertex2f(630, alturaJanela - 50);
    glVertex2f(680, alturaJanela - 50);
    glVertex2f(680, alturaJanela - 90);
    glVertex2f(630, alturaJanela - 90);
    glEnd();
}

void desenharMenuOpcoes() {

    //Triangulo
    glColor3f(0, 0, 0);
    glBegin(GL_LINE_LOOP);
    glVertex2f(20, alturaJanela - 40);
    glVertex2f(40, alturaJanela - 40);
    glVertex2f(30, alturaJanela - 20);
    glEnd();

    //Linha
    glPointSize(5);
    glBegin(GL_LINES);
    glVertex2f(60, alturaJanela - 20);
    glVertex2f(80, alturaJanela - 40);
    glEnd();

    //Poligano
    glBegin(GL_LINE_LOOP);
    glVertex2f(110, alturaJanela - 30);
    glVertex2f(130, alturaJanela - 20);
    glVertex2f(140, alturaJanela - 30);
    glVertex2f(140, alturaJanela - 40);
    glVertex2f(110, alturaJanela - 30);
    glEnd();

    //Quadrado
    glBegin(GL_LINE_LOOP);
    glVertex2f(160, alturaJanela - 20);
    glVertex2f(190, alturaJanela - 20);
    glVertex2f(190, alturaJanela - 40);
    glVertex2f(160, alturaJanela - 40);
    glEnd();

    desenharCirculo(220, alturaJanela - 30, 230, alturaJanela - 30);
}

void desenharOpcoesEixoEPintura() {
    glColor3f(0, 0, 0);
    glBegin(GL_LINE_LOOP);
    glVertex2f(270, alturaJanela - 10);
    glVertex2f(340, alturaJanela - 10);
    glVertex2f(340, alturaJanela - 90);
    glVertex2f(270, alturaJanela - 90);
    glEnd();

    //1° linha vertical
    glBegin(GL_LINES);
    glVertex2f(270, alturaJanela - 35);
    glVertex2f(340, alturaJanela - 35);
    glEnd();

    //2° linha vertical
    glBegin(GL_LINES);
    glVertex2f(270, alturaJanela - 60);
    glVertex2f(340, alturaJanela - 60);
    glEnd();

    draw_text_stroke(280, alturaJanela - 30, "Eixo-X", 0.1);
    draw_text_stroke(280, alturaJanela - 50, "Eixo-Y", 0.1);
    draw_text_stroke(280, alturaJanela - 80, "Pintar", 0.1);
}

void adicionarTransformacoes() {

    draw_text_stroke(20, alturaJanela - 80, "TRS", 0.1);
    draw_text_stroke(60, alturaJanela - 80, "ECA", 0.1);
    draw_text_stroke(110, alturaJanela - 80, "CISA", 0.1);
    draw_text_stroke(160, alturaJanela - 80, "ROTA", 0.1);
    draw_text_stroke(210, alturaJanela - 80, "REFLX", 0.1);

}

void mapearClickNaAreaOpcoes(int x, int y) {
    printf("Clicou em [%d, %d]", x, y);
    printf("Esperado [%d, %d]", alturaJanela - 50, alturaJanela - 90);
    if (x >= 10 && x <= 50 && y <= (alturaJanela - 10) && y >= (alturaJanela - 50)) {
        formaAtual = TRIANGULO;
        printf("Clicou no triangulo");
    }

    if (x >= 51 && x <= 100 && y <= (alturaJanela - 10) && y >= (alturaJanela - 50)) {
        formaAtual = LINHA;
        printf("Clicou na Linha");
    }

    if (x >= 101 && x <= 150 && y <= (alturaJanela - 10) && y >= (alturaJanela - 50)) {
        formaAtual = POLIGANO;
        printf("Clicou na Poligano");
    }

    if (x >= 151 && x <= 200 && y <= (alturaJanela - 10) && y >= (alturaJanela - 50)) {
        formaAtual = RETANGULO;
        printf("Clicou na Retangulo");
    }

    if (x >= 201 && x <= 250 && y <= (alturaJanela - 10) && y >= (alturaJanela - 50)) {
        formaAtual = CIRCULO;
        printf("Clicou na Circulo");
    }

    if (x >= 10 && x <= 50 && y <= (alturaJanela - 50) && y >= (alturaJanela - 90)) {
        isAplicarTranslacao = true;
        printf("Clicou em translação");
    }

    if (x >= 51 && x <= 100 && y <= (alturaJanela - 50) && y >= (alturaJanela - 90)) {
        isAplicarEscala = true;
        printf("Clicou em Escala");
    }

    if (x >= 101 && x <= 150 && y <= (alturaJanela - 50) && y >= (alturaJanela - 90)) {
        isAplicarCisalhamento = true;
        printf("Clicou em Cisalhamento");
    }

    if (x >= 151 && x <= 200 && y <= (alturaJanela - 50) && y >= (alturaJanela - 90)) {
        isAplicaoRotacao = true;
        printf("Clicou em Rotação");
    }

    if (x >= 201 && x <= 250 && y <= (alturaJanela - 50) && y >= (alturaJanela - 90)) {
        isAplicarReflexao = true;
        printf("Clicou em Reflexão");
    }

    //Mapeamento para aplicar pintura.
    if (x >= 350 && x <= 400 && y <= (alturaJanela - 10) && y >= (alturaJanela - 50)) {
        corAtual[0] = 0.0f;
        corAtual[1] = 0.0f;
        corAtual[2] = 1.0f;
    }

    if (x >= 401 && x <= 450 && y <= (alturaJanela - 10) && y >= (alturaJanela - 50)) {
        corAtual[0] = 0.5f;
        corAtual[1] = 0.0f;
        corAtual[2] = 0.5f;
    }

    if (x >= 451 && x <= 500 && y <= (alturaJanela - 10) && y >= (alturaJanela - 50)) {
        corAtual[0] = 0.54f;
        corAtual[1] = 0.17f;
        corAtual[2] = 0.89f;
    }

    if (x >= 501 && x <= 550 && y <= (alturaJanela - 10) && y >= (alturaJanela - 50)) {
        corAtual[0] = 1.0f;
        corAtual[1] = 1.0f;
        corAtual[2] = 0.0f;
    }

    if (x >= 551 && x <= 600 && y <= (alturaJanela - 10) && y >= (alturaJanela - 50)) {
        corAtual[0] = 0.54f;
        corAtual[1] = 0.17f;
        corAtual[2] = 0.89f;
    }

    //Cores de baixo
    if (x >= 350 && x <= 400 && y <= (alturaJanela - 50) && y >= (alturaJanela - 90)) {
        corAtual[0] = 1.0f;
        corAtual[1] = 0.5f;
        corAtual[2] = 0.0f;
    }

    if (x >= 401 && x <= 450 && y <= (alturaJanela - 50) && y >= (alturaJanela - 90)) {
        corAtual[0] = 1.0f;
        corAtual[1] = 0.0f;
        corAtual[2] = 0.0f;
    }

    if (x >= 451 && x <= 500 && y <= (alturaJanela - 50) && y >= (alturaJanela - 90)) {
        corAtual[0] = 1.0f;
        corAtual[1] = 0.0f;
        corAtual[2] = 1.0f;
    }

    if (x >= 501 && x <= 550 && y <= (alturaJanela - 50) && y >= (alturaJanela - 90)) {
        corAtual[0] = 0.0f;
        corAtual[1] = 1.0f;
        corAtual[2] = 1.0f;
    }

    if (x >= 551 && x <= 600 && y <= (alturaJanela - 50) && y >= (alturaJanela - 90)) {
        corAtual[0] = 0.0f;
        corAtual[1] = 0.5f;
        corAtual[2] = 0.5f;
    }

    //Pintura e escolha dos Eixos
    if (x >= 270 && x <= 340 && y <= (alturaJanela - 10) && y >= (alturaJanela - 35)) {
        eixo = 'x';
        printf("Clicou para eixo X");
    }

    if (x >= 270 && x <= 340 && y <= (alturaJanela - 36) && y >= (alturaJanela - 60)) {
        eixo = 'y';
        printf("Clicou para eixo Y");
    }

    if (x >= 270 && x <= 340 && y <= (alturaJanela - 60) && y >= (alturaJanela - 90)) {
        isAplicarPintura = true;
        printf("Clicou para pintura");
    }
}