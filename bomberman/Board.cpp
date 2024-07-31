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
    glColor3f(0.0, 0.0, 1.0);
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
    // // printf("Booneco: [%d][%d] - ", x, z);
    // for (int i = x; i < (x + tamanho); i++)
    // {
    //     for (int j = z; j < (z + tamanho); j++)
    //     {
    //         if (i >= 0 && i < this->largura && j >= 0 && j < this->comprimento) {
    //             if (this->coordenada_chao_board[i][j] == 1) {
    //                 // printf("[%d][%d] triscou na parede \n", i, j);
    //                 return;
    //             }
    //         }
    //     }

    // }

    // printf("[%d][%d] passou sem triscar", x, z);
    this->cgQuadrado.desenhaQuadrado(x, z, tamanho);
}

std::vector<std::vector<int>>& Board::getCoordenadas() {
    return this->coordenada_chao_board;
}

void Board::soltarBomba(int x, int z, int tamanho) {
    // printf("[%d] [%d] => %d", x, z, tamanho);
    glColor3f(1.0, 1.0, 0.0);
    this->cgQuadrado.desenhaQuadrado(x, z, tamanho);
}

void Board::desenhar_cenario() {
    desenhar_chao();
    desenhar_linhas_no_chao();
    iniciar_matriz();
    desenhar_obstaculos();
}