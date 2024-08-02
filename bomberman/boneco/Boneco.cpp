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
#include "Boneco.h"
#include "../cg/CGQuadrado.h"

Boneco::Boneco(vector3d pos) {
    m_pos = pos;
    ponto_camera = vector3d(0, 1, 0);
    up_camera = vector3d(0, 0, 0);

    m_dir = vector3d(0, 0, -1);
    m_left = vector3d(-1, 0, 0);
    m_up = vector3d(0, 1, 0);
    m_velocidade = vector3d(0, 0, 0);
}

Boneco::Boneco(vector3d pos, vector3d ponto, vector3d up) {
    m_pos = pos;
    ponto_camera = ponto;
    up_camera = up;

    m_dir = vector3d(0, 0, -1);
    m_left = vector3d(-1, 0, 0);
    m_up = vector3d(0, 1, 0);
    m_velocidade = vector3d(0, 0, 0);
}

Boneco::~Boneco() {
}

Boneco::Boneco() {
}

void Boneco::ativar() {
    gluLookAt(m_pos.x, m_pos.y, m_pos.z, ponto_camera.x, ponto_camera.y, ponto_camera.z, up_camera.x, up_camera.y, up_camera.z);
}

void Boneco::frente() {
    m_velocidade = m_dir * escala_velocidade;
    m_pos = m_velocidade + m_pos;

    ponto_camera.x = m_pos.x;
    ponto_camera.z = 0;

    // printf("%f %f %f \n", m_pos.x, m_pos.y, m_pos.z);
}

void Boneco::tras() {
    m_velocidade = m_dir * (-escala_velocidade);
    m_pos = m_velocidade + m_pos;

    ponto_camera.x = m_pos.x;
    ponto_camera.z = 300;

    // printf("%f %f %f \n", m_pos.x, m_pos.y, m_pos.z);
}

void Boneco::esquerda() {
    m_velocidade = m_left * escala_velocidade;
    m_pos = m_pos + m_velocidade;


    ponto_camera.x = 0;
    ponto_camera.z = m_pos.z;

    // printf("%f %f %f \n", m_pos.x, m_pos.y, m_pos.z);
}

void Boneco::direita() {
    m_velocidade = m_left * (-escala_velocidade);
    m_pos = m_pos + m_velocidade;

    ponto_camera.x = 300;
    ponto_camera.z = m_pos.z;

    // printf("%f %f %f \n", m_pos.x, m_pos.y, m_pos.z);
}

float Boneco::convertParaRadianos(float angulo) {

    return (angulo * valor_PI) / 180.0f;
}


void Boneco::ativarVisaoCima() {
    gluLookAt(150, 370, 150, // Posição da câmera (x, y, z)
        150, -0.5, 150,    // Ponto para onde a câmera está olhando (x, y, z)
        0.0, 0.0, 1.0); //UP da Boneco
}

vector3d Boneco::getPos() {
    return this->m_pos;
}


vector3d Boneco::getPosTem() {
    return this->m_pos_temp;
}

void Boneco::commitPos() {}

void Boneco::frente(std::vector<std::vector<int>>& coordenadas) {
    m_velocidade = m_dir * escala_velocidade;
    vector3d pos_temp = m_velocidade + m_pos;

    if (!contatoCenario(coordenadas, pos_temp.x, pos_temp.z)) {
        m_pos = pos_temp;
        ponto_camera.x = m_pos.x;
        ponto_camera.z = 0;
    }

}

void Boneco::direita(std::vector<std::vector<int>>& coordenadas) {
    m_velocidade = m_left * (-escala_velocidade);
    vector3d pos_temp = m_pos + m_velocidade;

    if (!contatoCenario(coordenadas, pos_temp.x, pos_temp.z)) {
        m_pos = pos_temp;
        ponto_camera.x = 300;
        ponto_camera.z = m_pos.z;
    }
}

void Boneco::esquerda(std::vector<std::vector<int>>& coordenadas) {
    m_velocidade = m_left * escala_velocidade;
    vector3d pos_temp = m_pos + m_velocidade;

    if (!contatoCenario(coordenadas, pos_temp.x, pos_temp.z)) {
        m_pos = pos_temp;
        ponto_camera.x = 0;
        ponto_camera.z = m_pos.z;
    }

}

void Boneco::tras(std::vector<std::vector<int>>& coordenadas) {
    m_velocidade = m_dir * (-escala_velocidade);
    vector3d pos_temp = m_velocidade + m_pos;
    if (!contatoCenario(coordenadas, pos_temp.x, pos_temp.z)) {
        m_pos = pos_temp;
        ponto_camera.x = m_pos.x;
        ponto_camera.z = 300;
    }
}

bool Boneco::contatoCenario(std::vector<std::vector<int>>& coordenadas, int x, int z) {
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

void Boneco::soltarBomba() {
    CGQuadrado cgquadrado(-0.5);
    glColor3f(1.0, 1.0, 0.0);
    cgquadrado.desenhaQuadrado(this->m_pos.x, this->m_pos.z, 3);
    // printf("soltando bomba em [%d][%d]", this->m_pos.x, this->m_pos.z);
}

void Boneco::atualizarCoordenada(std::vector<vector3d>& coordenadas) {
    m_pos = coordenadas[0];

    // printf("Boneco: %f, %f , %f \n", m_pos.x, m_pos.y, m_pos.z);
    ponto_camera = coordenadas[1];
}