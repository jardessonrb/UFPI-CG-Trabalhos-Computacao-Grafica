#ifndef CGQUADRADO_H
#define CGQUADRADO_H

#define N_VERTICES 8
#define N_FACES 12


class CGQuadrado {

private:
    float alturaY;

public:
    CGQuadrado(float alturaY);
    CGQuadrado();
    ~CGQuadrado();
    void desenhaQuadrado(float x, float z, float tamanho);
    void desenhaObstaculos(int comprimento, int largura, float tamanho);

};


#endif // CGQUADRADO_H