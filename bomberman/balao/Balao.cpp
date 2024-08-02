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
#include "../vector3d.h"
#include <cstdio>
#include "Balao.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <random>

Balao::Balao(vector3d pos) {
    m_pos = pos;
    ponto_camera = vector3d(0, 1, 0);
    up_camera = vector3d(0, 0, 0);

    m_dir = vector3d(0, 0, -1);
    m_left = vector3d(-1, 0, 0);
    m_up = vector3d(0, 1, 0);
    m_velocidade = vector3d(0, 0, 0);
}

Balao::Balao(vector3d pos, vector3d ponto, vector3d up) {
    m_pos = pos;
    ponto_camera = ponto;
    up_camera = up;

    m_dir = vector3d(0, 0, -1);
    m_left = vector3d(-1, 0, 0);
    m_up = vector3d(0, 1, 0);
    m_velocidade = vector3d(0, 0, 0);
}

Balao::~Balao() {
}

void Balao::frente() {
    m_velocidade = m_dir * escala_velocidade;
    m_pos = m_velocidade + m_pos;

    ponto_camera.x = m_pos.x;
    ponto_camera.z = 0;

}

void Balao::tras() {
    m_velocidade = m_dir * (-escala_velocidade);
    m_pos = m_velocidade + m_pos;

    ponto_camera.x = m_pos.x;
    ponto_camera.z = 300;
}

void Balao::esquerda() {
    m_velocidade = m_left * escala_velocidade;
    m_pos = m_pos + m_velocidade;


    ponto_camera.x = 0;
    ponto_camera.z = m_pos.z;
}

void Balao::direita() {
    m_velocidade = m_left * (-escala_velocidade);
    m_pos = m_pos + m_velocidade;

    ponto_camera.x = 300;
    ponto_camera.z = m_pos.z;
}

vector3d Balao::getPos() {
    return this->m_pos;
}

void Balao::frente(std::vector<std::vector<int>>& coordenadas) {
    m_velocidade = m_dir * escala_velocidade;
    vector3d pos_temp = m_velocidade + m_pos;

    if (!contatoCenario(coordenadas, pos_temp.x, pos_temp.z)) {
        m_pos = pos_temp;
        ponto_camera.x = m_pos.x;
        ponto_camera.z = 0;
    }

}

void Balao::direita(std::vector<std::vector<int>>& coordenadas) {
    m_velocidade = m_left * (-escala_velocidade);
    vector3d pos_temp = m_pos + m_velocidade;

    if (!contatoCenario(coordenadas, pos_temp.x, pos_temp.z)) {
        m_pos = pos_temp;
        ponto_camera.x = 300;
        ponto_camera.z = m_pos.z;
    }
}

void Balao::esquerda(std::vector<std::vector<int>>& coordenadas) {
    m_velocidade = m_left * escala_velocidade;
    vector3d pos_temp = m_pos + m_velocidade;

    if (!contatoCenario(coordenadas, pos_temp.x, pos_temp.z)) {
        m_pos = pos_temp;
        ponto_camera.x = 0;
        ponto_camera.z = m_pos.z;
    }

}

void Balao::tras(std::vector<std::vector<int>>& coordenadas) {
    m_velocidade = m_dir * (-escala_velocidade);
    vector3d pos_temp = m_velocidade + m_pos;
    if (!contatoCenario(coordenadas, pos_temp.x, pos_temp.z)) {
        m_pos = pos_temp;
        ponto_camera.x = m_pos.x;
        ponto_camera.z = 300;
    }
}

bool Balao::contatoCenario(std::vector<std::vector<int>>& coordenadas, int x, int z) {
    int x_linha = x + 5;
    int z_linha = z + 5;

    if (x < 0 || x > 300 || x_linha < 0 || x_linha > 300) {
        return true;
    }

    if (z < 0 || z > 300 || z_linha < 0 || z_linha > 300) {
        return true;
    }

    if (coordenadas[x][z] != 0 || coordenadas[x][z_linha] != 0 || coordenadas[x_linha][z] != 0 || coordenadas[x_linha][z_linha] != 0) {
        return true;
    }

    return false;
}

vector3d Balao::calcular_direcao(std::vector<std::vector<int>>& coordenadas) {
    static std::random_device rd;           // Obtém uma semente baseada em hardware
    static std::mt19937 gerador(rd());      // Inicializa o gerador com a semente
    std::uniform_int_distribution<int> distrib(0, 3); // Define a distribuição uniforme

    int numeroAleatorio = distrib(gerador);
    if (this->tempo_ultimo_movimento < glutGet(GLUT_ELAPSED_TIME)) {
        this->tempo_ultimo_movimento = (glutGet(GLUT_ELAPSED_TIME) + 500);
        if (numeroAleatorio == 0) {
            this->frente(coordenadas);
        }

        if (numeroAleatorio == 1) {
            this->direita(coordenadas);
        }

        if (numeroAleatorio == 2) {
            this->tras(coordenadas);
        }

        if (numeroAleatorio == 3) {
            this->esquerda(coordenadas);
        }
    }


    return this->m_pos;
}