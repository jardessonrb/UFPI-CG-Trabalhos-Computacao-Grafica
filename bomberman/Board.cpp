#include "board.h"

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

Board::~Board() {
}

Board::Board() {
}

Board::Board(CGQuadrado cg) {
    this->coordenada_chao_board = std::vector<std::vector<int>>(300, std::vector(300, 0));
    this->baloes = std::vector<Balao>();

    this->cgQuadrado = cg;
    this->coordenada_caixotes = std::vector<std::vector<int>>(16, std::vector(3, 0));
    this->coordenada_caixotes = {
        { 140, 40, 1},
        { 20, 200, 1},
        { 20, 160, 1},
        { 60, 20, 1},
        { 100, 20, 1},
        { 140, 20, 1},
        { 200, 20, 1},
        { 140, 160, 1},
        { 140, 180, 1},
        { 140, 200, 1},
        { 140, 220, 1},
        { 140, 240, 1},
        { 140, 260, 1},
        { 240, 140, 1},
        { 240, 180, 1},
        { 240, 220, 1},
    };
}

Board::Board(CGQuadrado cg, std::vector<Balao> baloes) {
    this->coordenada_chao_board = std::vector<std::vector<int>>(300, std::vector(300, 0));
    this->baloes = baloes;
    this->cgQuadrado = cg;
    this->coordenada_caixotes = std::vector<std::vector<int>>(16, std::vector(3, 0));
    this->coordenada_caixotes = {
        { 140, 40, 1},
        { 20, 200, 1},
        { 20, 160, 1},
        { 60, 20, 1},
        { 100, 20, 1},
        { 140, 20, 1},
        { 200, 20, 1},
        { 140, 160, 1},
        { 140, 180, 1},
        { 140, 200, 1},
        { 140, 220, 1},
        { 140, 240, 1},
        { 140, 260, 1},
        { 240, 140, 1},
        { 240, 180, 1},
        { 240, 220, 1},
    };
}

bool Board::isParede(int x, int z) {
    return false;
}

void Board::desenhar_chao() {
    glColor3f(0.0f, 0.0f, 0.0f); // Define a cor preta
    glBegin(GL_QUADS); // Inicia o desenho de um quadrilátero
    glVertex3f(0.0f, altura_y, 300.0f); // Primeiro vértice
    glVertex3f(300.0f, altura_y, 300.0f);  // Segundo vértice
    glVertex3f(300.0f, altura_y, 0.0f);   // Terceiro vértice
    glVertex3f(0.0f, altura_y, 0.0f);  // Quarto vértice
    glEnd(); // Termina o desenho do quadrilátero
}

void Board::desenhar_obstaculos() {
    glColor3f(0.0, 0.0, 0.5);

    //Lado esquerdo e direito
    for (int z = 0; z < this->comprimento; z += this->tamanho_bloco)
    {
        this->cgQuadrado.desenhaQuadrado(0, z, this->tamanho_bloco);
        this->cgQuadrado.desenhaQuadrado(this->comprimento - this->tamanho_bloco, z, this->tamanho_bloco);

        this->marcar_matriz(0, z, 1);
        this->marcar_matriz(this->comprimento - this->tamanho_bloco, z, 1);
    }

    // Lado frente e limite
    for (int x = 0; x < this->comprimento; x += this->tamanho_bloco)
    {
        this->cgQuadrado.desenhaQuadrado(x, 0, this->tamanho_bloco);
        this->cgQuadrado.desenhaQuadrado(x, largura - this->tamanho_bloco, this->tamanho_bloco);

        this->marcar_matriz(x, 0, 1);
        this->marcar_matriz(x, largura - this->tamanho_bloco, 1);
    }

    //Quadrados Alternados
    bool desenha = true;
    for (int x = (2 * this->tamanho_bloco), z = (2 * this->tamanho_bloco); x < comprimento; x += this->tamanho_bloco)
    {
        if (desenha) {
            bool desenha_linha = true;
            for (int z_linha = z; z_linha < comprimento; z_linha += this->tamanho_bloco)
            {
                if (desenha_linha) {
                    this->cgQuadrado.desenhaQuadrado(x, z_linha, this->tamanho_bloco);
                    this->marcar_matriz(x, z_linha, 1);
                }

                desenha_linha = !desenha_linha;
            }
        }
        desenha = !desenha;
    }
    //Desenha cubos
    glColor3f(0.5, 0.4, 0.0);
    for (int i = 0; i < this->coordenada_caixotes.size(); i++)
    {
        if (this->coordenada_caixotes[i][2]) {
            this->cgQuadrado.desenhaQuadrado(this->coordenada_caixotes[i][0], this->coordenada_caixotes[i][1], this->tamanho_bloco);
            this->marcar_matriz(this->coordenada_caixotes[i][0], this->coordenada_caixotes[i][1], 2);
        }
    }

}

void Board::apagar_caixote(int x, int z) {
    for (int i = 0; i < this->coordenada_caixotes.size(); i++)
    {
        if (this->coordenada_caixotes[i][0] == x && this->coordenada_caixotes[i][1] == z) {
            this->coordenada_caixotes[i][2] = 0;
            this->marcar_matriz(this->coordenada_caixotes[i][0], this->coordenada_caixotes[i][1], 0);
        }
    }

}

void Board::detectar_explosao(int x, int z, int tamanho) {
    int x_linha = x + tamanho;
    int z_linha = z + tamanho;

    if (x < 0 || x > 300 || x_linha < 0 || x_linha > 300 || z < 0 || z > 300 || z_linha < 0 || z_linha > 300) {
        return;
    }

    for (int i = 0; i < this->coordenada_caixotes.size(); i++)
    {
        if (this->coordenada_chao_board[x][z] == 2) {
            // printf("%d - %d detectado", x, z);
            this->apagar_caixote(x, z);
        }

        if (this->coordenada_chao_board[x][z_linha] == 2) {
            // printf("%d - %d detectado", x, z_linha);
            this->apagar_caixote(x, z_linha);
        }

        if (this->coordenada_chao_board[x_linha][z] == 2) {
            // printf("%d - %d detectado", x_linha, z);
            this->apagar_caixote(x_linha, z);
        }

        if (this->coordenada_chao_board[x_linha][z_linha] == 2) {
            // printf("%d - %d detectado", x_linha, z_linha);
            this->apagar_caixote(x_linha, z_linha);
        }
    }
}

void Board::desenhar_linhas_no_chao() {
    int z_inicial = 0;

    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_LINES);
    for (int i = 0; i <= this->largura && z_inicial >= -this->largura; i += this->tamanho_bloco, z_inicial += this->tamanho_bloco)
    {

        //Linha comprimento
        glVertex3f(i, altura_y, 0);
        glVertex3f(i, altura_y, this->largura);

        //Linha largura
        glVertex3f(0, altura_y, z_inicial);
        glVertex3f(this->largura, altura_y, z_inicial);


    }
    glEnd();
}

void Board::iniciar_matriz() {

}

void Board::marcar_matriz(int x, int z, int peso) {
    for (int i = x; i < (x + this->tamanho_bloco); i++)
    {
        for (int j = z; j < (z + this->tamanho_bloco); j++)
        {
            if (i >= 0 && i < this->largura && j >= 0 && j < this->comprimento) {
                this->coordenada_chao_board[i][j] = peso;
            }
        }
    }
}

void Board::desenhar_personagem(int x, int z, int tamanho) {
    this->cgQuadrado.desenhaQuadrado(x, z, tamanho);
}

std::vector<std::vector<int>>& Board::getCoordenadas() {
    return this->coordenada_chao_board;
}

void Board::desenhar_bomba(int x, int z, int tamanho) {
    glColor3f(0.67, 0.35, 0.12);
    this->cgQuadrado.desenhaQuadrado(x, z, tamanho);
}

void Board::desenhar_cenario() {
    desenhar_chao();
    desenhar_linhas_no_chao();
    iniciar_matriz();
    desenhar_obstaculos();
    ativar_bomba();
    desenhar_baloes();
}

void Board::ativar_bomba() {
    if (this->isSoltarBomba && this->inicio_tempo_bomba > glutGet(GLUT_ELAPSED_TIME)) {
        this->desenhar_bomba(this->posicao_bomba.first, this->posicao_bomba.second, this->tamanho_bomba);
    }

    if (this->inicio_tempo_bomba < glutGet(GLUT_ELAPSED_TIME)) {
        this->isSoltarBomba = false;
        if (!this->isBombaEmOutoro && this->quantidade_bombas == 1) {
            this->isBombaEmOutoro = true;
            this->inicio_tempo_bomba_estouro = (glutGet(GLUT_ELAPSED_TIME) + 2000);
            this->quantidade_bombas = 0;
        }
    }

    if (this->isBombaEmOutoro && (this->inicio_tempo_bomba_estouro > glutGet(GLUT_ELAPSED_TIME))) {
        this->estourar_bomba(this->posicao_bomba.first, this->posicao_bomba.second, this->tamanho_bloco, this->potencia_bomba);
    }
    else if (this->inicio_tempo_bomba_estouro < glutGet(GLUT_ELAPSED_TIME)) {
        this->isBombaEmOutoro = false;
    }
}

void Board::soltar_bomba(int x, int z, int tamanho, int potencia) {
    this->potencia_bomba = potencia;
    this->tamanho_bomba = tamanho;
    if (!this->isSoltarBomba && !this->isBombaEmOutoro) {
        this->isSoltarBomba = true;
        this->inicio_tempo_bomba = (glutGet(GLUT_ELAPSED_TIME) + 3000);
        this->quantidade_bombas = 1;
        this->posicao_bomba = std::pair<float, float>(x, z);
    }
}

void Board::estourar_bomba(int x, int z, int tamanho, int potencia) {
    x = (x / this->tamanho_bloco) * this->tamanho_bloco;
    z = (z / this->tamanho_bloco) * this->tamanho_bloco;

    for (int i = 0, z_linha_mais = z, z_linha_menos = z; i < potencia; i++, z_linha_mais += tamanho, z_linha_menos -= tamanho)
    {
        this->desenhar_bomba(x, z_linha_mais, tamanho);
        this->desenhar_bomba(x, z_linha_menos, tamanho);

        this->detectar_explosao(x, z_linha_mais, tamanho);
        this->detectar_explosao(x, z_linha_menos, tamanho);
    }

    for (int i = 0, x_linha_mais = x, x_linha_menos = x; i < potencia; i++, x_linha_mais += tamanho, x_linha_menos -= tamanho)
    {
        this->desenhar_bomba(x_linha_mais, z, tamanho);
        this->desenhar_bomba(x_linha_menos, z, tamanho);

        this->detectar_explosao(x_linha_mais, z, tamanho);
        this->detectar_explosao(x_linha_menos, z, tamanho);
    }
}

void Board::desenhar_baloes() {
    glColor3f(0.0f, 0.8f, 0.0f);
    for (int i = 0; i < this->baloes.size(); i++)
    {
        vector3d posicao_balao = this->baloes[i].calcular_direcao(getCoordenadas());
        this->desenhar_personagem(posicao_balao.x, posicao_balao.z, 10);
    }

}

void Board::add_balao(Balao b) {
    this->baloes.push_back(b);
}