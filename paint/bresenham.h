#ifndef bresenham_h
#define bresenham_h
#include <bits/stdc++.h>

using namespace std;

vector<pair<int, int>> calcularVertices(int valorD, int incrementoE, int incrementoNE, bool isSimetrico, bool isDeclive, int x1, int y1, int x2) {
    vector<pair<int, int>> vertices;

    int di = 0;
    while (x1 < x2) {
        if (valorD <= 0) {
            valorD += incrementoE;
        }
        else {
            valorD += incrementoNE;
            y1++;
        }

        x1++;

        int valorCorrenteX = x1;
        int valorCorrenteY = y1;

        if (isDeclive) {
            valorCorrenteX = y1;
            valorCorrenteY = x1;
        }

        if (isSimetrico) {
            valorCorrenteY = valorCorrenteY * (-1);
        }

        vertices.emplace_back(valorCorrenteX, valorCorrenteY);

    }

    return vertices;
}


int mod(int valor) {
    return valor < 0 ? valor * (-1) : valor;
}

vector<pair<int, int>> calcularBresenhamPrimeiroOctante(int x1, int y1, int x2, int y2) {
    int deltaX, deltaY, valorD, incrementoE, incrementoNE;
    bool isSimetrico = false, isDeclive = false;

    deltaX = x2 - x1;
    deltaY = y2 - y1;

    int multDelta = deltaX * deltaY;

    if (multDelta < 0) {
        y1 = y1 * (-1);
        y2 = y2 * (-1);

        deltaY = deltaY * (-1);
        isSimetrico = true;
    }

    int moduloDeltaX = mod(deltaX);
    int moduloDeltaY = mod(deltaY);

    if (moduloDeltaX < moduloDeltaY) {
        int tempCoord1 = y1;
        y1 = x1;
        x1 = tempCoord1;

        int tempCoord2 = y2;
        y2 = x2;
        x2 = tempCoord2;

        int tempDelta = deltaY;
        deltaY = deltaX;
        deltaX = tempDelta;

        isDeclive = true;
    }

    if (x1 > x2) {
        int tempCoordX = x1;
        int tempCoordY = y1;

        x1 = x2;
        y1 = y2;

        x2 = tempCoordX;
        y2 = tempCoordY;

        deltaX = deltaX * (-1);
        deltaY = deltaY * (-1);
    }

    deltaY = y2 - y1;
    deltaX = x2 - x1;
    valorD = (2 * deltaY) - deltaX;
    incrementoE = 2 * deltaY;
    incrementoNE = 2 * (deltaY - deltaX);

    return calcularVertices(valorD, incrementoE, incrementoNE, isSimetrico, isDeclive, x1, y1, x2);
}

#endif