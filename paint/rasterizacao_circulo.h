#ifndef rasterizacao_circulo_h
#define rasterizacao_circulo_h
#include <vector>

using namespace std;

void salvarPontosSimetricos(int x, int y, int incrementoX, int incrementoY, vector<pair<int, int>>& vertices) {

    vertices.emplace_back(x + incrementoX, y + incrementoY);
    vertices.emplace_back(-x + incrementoX, y + incrementoY);
    vertices.emplace_back(x + incrementoX, -y + incrementoY);
    vertices.emplace_back(-x + incrementoX, -y + incrementoY);


    vertices.emplace_back(y + incrementoX, x + incrementoY);
    vertices.emplace_back(-y + incrementoX, x + incrementoY);
    vertices.emplace_back(y + incrementoX, -x + incrementoY);
    vertices.emplace_back(-y + incrementoX, -x + incrementoY);
}

vector<pair<int, int>> salvarCirculo(int x1, int y1, int x2, int y2) {
    vector<pair<int, int>> vertices;

    float raio = sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
    int incrementoX = x2;
    int incrementoY = y2;
    float x = 0, y = raio;

    float valorD = 1 - raio;
    float deltaE = 3;
    float deltaSE = -2 * raio + 5;

    while (y > x) {

        salvarPontosSimetricos(x, y, incrementoX, incrementoY, vertices);

        if (valorD < 0) {
            valorD += deltaE;
            deltaE += 2;
            deltaSE += 2;
        }
        else {
            valorD += deltaSE;
            deltaE += 2;
            deltaSE += 4;

            y--;
        }

        x++;
    }

    salvarPontosSimetricos(x, y, incrementoX, incrementoY, vertices);

    return vertices;
}


void desenharCirculo(int x1, int y1, int x2, int y2) {
    vector<std::pair<int, int>> vertices = salvarCirculo(x1, y1, x2, y2);
    for (auto [x, y] : vertices) {
        glPointSize(1);
        glBegin(GL_POINTS);
        glVertex2f(x, y);
        glEnd();
        glFlush();
    }
}

#endif