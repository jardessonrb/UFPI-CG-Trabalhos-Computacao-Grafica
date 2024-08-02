#ifndef BOARD_H
#define BOARD_H
#include "cg/CGQuadrado.h"
#include <vector>
#include "balao/Balao.h"

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
    std::pair<float, float> posicao_bomba;
    unsigned int inicio_tempo_bomba = 0;
    unsigned int inicio_tempo_bomba_estouro = 0;
    bool isSoltarBomba = false;
    bool isBombaEmOutoro = false;
    int quantidade_bombas = 0;
    int potencia_bomba = 3;
    int tamanho_bomba = 20;
    std::vector<Balao> baloes;

public:
    ~Board();
    Board();
    Board(CGQuadrado cgQuadrado);
    Board(CGQuadrado cgQuadrado, std::vector<Balao> baloes);
    bool isParede(int x, int z);
    void desenhar_chao();
    void desenhar_obstaculos();
    void desenhar_linhas_no_chao();
    void desenhar_cenario();
    void marcar_matriz(int x, int z, int peso);
    void desenhar_personagem(int x, int z, int tamanho);
    void desenhar_baloes();
    std::vector<std::vector<int>>& getCoordenadas();
    void desenhar_bomba(int x, int z, int tamanho);
    void ativar_bomba();
    void estourar_bomba(int x, int z, int tamanho, int potencia);
    void soltar_bomba(int x, int z, int tamanho, int potencia);
    void detectar_explosao(int x, int z, int tamanho);
    void apagar_caixote(int x, int z);
    void add_balao(Balao balao);
};


#endif // BOARD_H