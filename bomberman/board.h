#ifndef BOARD_H
#define BOARD_H

class Board {
private:
    const int largura = 300;
    const int comprimento = 300;
    const int tamanho_bloco = 20;
    const int altura_y = -0.5f;
    int coordenada_chao_board[300][300];
    void iniciar_matriz();

public:
    ~Board();
    Board();
    bool isParede(int x, int z);
    void desenhar_chao();
    void desenhar_obstaculos();
    void desenhar_linhas_no_chao();
    void desenhar_cenario();
};


#endif // BOARD_H