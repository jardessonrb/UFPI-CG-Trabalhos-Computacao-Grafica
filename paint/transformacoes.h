#ifndef transformacoes_h
#define transformacoes_h
#include <utility>

using namespace std;

pair<int, int> translacao(int coordenadaX, int coordenadaY, double valorTX, double valorTy) {
    coordenadaX = (int)(valorTX + coordenadaX);
    coordenadaY = (int)(valorTy + coordenadaY);

    return make_pair(coordenadaX, coordenadaY);
}

pair<int, int> escala(int coordenadaX, int coordenadaY, double valorEscala) {
    coordenadaX = (int)(valorEscala * coordenadaX);
    coordenadaY = (int)(valorEscala * coordenadaY);

    return make_pair(coordenadaX, coordenadaY);
}

pair<int, int> rotacao(int coordenadaX, int coordenadaY, double cosseno, double seno) {
    int novoValorX = (int)((coordenadaX * cosseno) - (coordenadaY * seno));
    int novoValorY = (int)((coordenadaX * seno) + (coordenadaY * cosseno));

    return make_pair(novoValorX, novoValorY);
}

pair<int, int> cisalhamento(int coordenadaX, int coordenadaY, double valorCisalhamento) {
    coordenadaX = (int)(coordenadaX + (coordenadaY * valorCisalhamento));

    return make_pair(coordenadaX, coordenadaY);
}

pair<int, int> reflexao(int coordenadaX, int coordenadaY, char eixo, int maxX = 700, int maxY = 600) {
    int xRefletido = coordenadaX;
    int yRefletido = coordenadaY;

    if (eixo == 'Y' || eixo == 'y') {
        xRefletido = maxX - coordenadaX;
    }

    if (eixo == 'X' || eixo == 'x') {
        yRefletido = maxY - coordenadaY;
    }

    return std::make_pair(xRefletido, yRefletido);
}




#endif

