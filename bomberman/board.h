#ifndef BOARD_H
#define BOARD_H
#include "cg/CGQuadrado.h"
#include <vector>

class Board {

private:
    int largura = 300;
    int comprimento = 300;
    int tamanho_bloco = 20;
    int altura_y = -0.5f;
    std::vector<std::vector<int>> coordenada_chao_board;
    CGQuadrado cgQuadrado;
    void iniciar_matriz();
    std::vector<std::vector<int>> coordenada_caixotes;

public:
    ~Board();
    Board();
    Board(CGQuadrado cgQuadrado);
    bool isParede(int x, int z);
    void desenhar_chao();
    void desenhar_obstaculos();
    void desenhar_linhas_no_chao();
    void desenhar_cenario();
    void marcarMatriz(int x, int z);
    void desenharPersonagem(int x, int z, int tamanho);
    std::vector<std::vector<int>>& getCoordenadas();
    void soltarBomba(int x, int z, int tamanho);
};


#endif // BOARD_H