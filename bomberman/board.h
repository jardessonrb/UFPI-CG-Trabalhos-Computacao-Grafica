#ifndef BOARD_H
#define BOARD_H
#include "cg/CGQuadrado.h"
#include <vector>
#include "balao/Balao.h"
#include "boneco/Boneco.h"
#include "Camera.h"
#include "tocar/tocar.h"
#include "thread"

class Board {

private:
    int largura = 300;
    int comprimento = 300;
    int tamanho_bloco = 20;
    int altura_y = -0.5f;
    std::vector<std::vector<int>> coordenada_chao_board;
    CGQuadrado cgQuadrado;
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
    Boneco boneco;
    Camera camera;
    bool isCameraPrimeiraPessoa = false;
    int motivo_fim_jogo = 0;
    int quantidade_pontos = 0;

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
    void add_boneco(Boneco balao);
    void add_camera(Camera camera);
    void alternar_camera();
    void ativar_camera();
    int get_status_jogo();
    int get_motivo_morte();
    int get_quantidade_pontos();
    void detectar_contato_com_balao();

    void evento_keyboard(char keyboard);
};


#endif // BOARD_H