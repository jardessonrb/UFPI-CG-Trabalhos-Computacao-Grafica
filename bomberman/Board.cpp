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
    // Define a cor do cubo
    glColor3f(0.0, 0.0, 1.0); // Azul
    // Translada o cubo para a posição desejada
    glPushMatrix();
    glTranslatef(-275.0, 30.0, 0.0); // Move o cubo para a posição (-300, -0.5, 300) com largura e altura de 50
    glutSolidCube(50.0); // Desenha um cubo com 50 unidades de lado
    glPopMatrix();
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
    for (int z = 0; z < this->comprimento; z++) {
        for (int x = 0; x < this->tamanho_bloco; x++) {
            this->coordenada_chao_board[z][x] = 1;
            this->coordenada_chao_board[z][x + (this->largura - this->tamanho_bloco)] = 1;
        }
    }

    for (int x = this->tamanho_bloco; x < (this->largura - this->tamanho_bloco); x++) {
        for (int z = 0; z < this->tamanho_bloco; z++) {
            this->coordenada_chao_board[x][z] = 1;
            this->coordenada_chao_board[x][z + (this->comprimento - this->tamanho_bloco)] = 1;
        }
    }
}

void Board::desenhar_cenario() {
    desenhar_chao();
    desenhar_linhas_no_chao();
    iniciar_matriz();
}