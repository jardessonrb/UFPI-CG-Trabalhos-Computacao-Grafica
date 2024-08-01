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
    // for (int i = 0; i < this->largura; i++)
    // {
    //     for (int j = 0; j < this->comprimento; j++)
    //     {
    //         this->coordenada_chao_board[i][j] = 0;
    //     }
    // }

    this->cgQuadrado = cg;
    this->coordenada_caixotes = std::vector<std::vector<int>>(16, std::vector(2, 0));
    this->coordenada_caixotes = {
        { 140, 40 },
        { 20, 200 },
        { 20, 160 },
        { 60, 20 },
        { 100, 20 },
        { 140, 20 },
        { 200, 20 },
        { 140, 160 },
        { 140, 180 },
        { 140, 200 },
        { 140, 220 },
        { 140, 240 },
        { 140, 260 },
        { 240, 140 },
        { 240, 180 },
        { 240, 220 },
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

        this->marcarMatriz(0, z);
        this->marcarMatriz(this->comprimento - this->tamanho_bloco, z);
    }

    // Lado frente e limite
    for (int x = 0; x < this->comprimento; x += this->tamanho_bloco)
    {
        this->cgQuadrado.desenhaQuadrado(x, 0, this->tamanho_bloco);
        this->cgQuadrado.desenhaQuadrado(x, largura - this->tamanho_bloco, this->tamanho_bloco);

        this->marcarMatriz(x, 0);
        this->marcarMatriz(x, largura - this->tamanho_bloco);

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
                    this->marcarMatriz(x, z_linha);
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
        this->cgQuadrado.desenhaQuadrado(this->coordenada_caixotes[i][0], this->coordenada_caixotes[i][1], this->tamanho_bloco);
        this->marcarMatriz(this->coordenada_caixotes[i][0], this->coordenada_caixotes[i][1]);
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

void Board::marcarMatriz(int x, int z) {
    for (int i = x; i < (x + this->tamanho_bloco); i++)
    {
        for (int j = z; j < (z + this->tamanho_bloco); j++)
        {
            if (i >= 0 && i < this->largura && j >= 0 && j < this->comprimento) {
                this->coordenada_chao_board[i][j] = 1;
            }
        }
    }
}

void Board::desenharPersonagem(int x, int z, int tamanho) {
    this->cgQuadrado.desenhaQuadrado(x, z, tamanho);
}

std::vector<std::vector<int>>& Board::getCoordenadas() {
    return this->coordenada_chao_board;
}

void Board::soltarBomba(int x, int z, int tamanho) {

    // glColor3f(0.67, 0.35, 0.12);
    this->cgQuadrado.desenhaQuadrado(x, z, tamanho);
}

void Board::desenhar_cenario() {
    desenhar_chao();
    desenhar_linhas_no_chao();
    iniciar_matriz();
    desenhar_obstaculos();
}