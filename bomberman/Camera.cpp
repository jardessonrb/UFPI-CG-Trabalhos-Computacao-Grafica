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
#include <cstdio>
#include "camera.h"
#include "vector3d.h"
#include <vector>

Camera::Camera() {}

Camera::Camera(vector3d pos) {
    m_pos = pos;
    ponto_camera = vector3d(0, 0, 0);
    up_camera = vector3d(0, 0, 0);

    m_dir = vector3d(0, 0, -1);
    m_left = vector3d(-1, 0, 0);
    m_up = vector3d(0, 1, 0);
    m_velocidade = vector3d(0, 0, 0);

    this->direcoes = std::vector<vector3d>();
    this->direcoes.push_back(vector3d(0, 0, -1));
    this->direcoes.push_back(vector3d(1, 0, 0));
    this->direcoes.push_back(vector3d(0, 0, 1));
    this->direcoes.push_back(vector3d(-1, 0, 0));
}

Camera::Camera(vector3d pos, vector3d ponto, vector3d up) {
    m_pos = pos;
    ponto_camera = ponto;
    up_camera = up;

    m_dir = vector3d(0, 0, -1);
    m_left = vector3d(-1, 0, 0);
    m_up = vector3d(0, 1, 0);
    m_velocidade = vector3d(0, 0, 0);
    this->direcoes = std::vector<vector3d>();
    this->direcoes.push_back(vector3d(0, 0, -1));
    this->direcoes.push_back(vector3d(1, 0, 0));
    this->direcoes.push_back(vector3d(0, 0, 1));
    this->direcoes.push_back(vector3d(-1, 0, 0));
}

Camera::~Camera() {
}

void Camera::ativar() {
    gluLookAt(m_pos.x, m_pos.y, m_pos.z, ponto_camera.x, ponto_camera.y, ponto_camera.z, up_camera.x, up_camera.y, up_camera.z);
}

void Camera::frente() {
    m_velocidade = m_dir * escala_velocidade;
    m_pos = m_velocidade + m_pos;

    ponto_camera.x = m_pos.x;
    ponto_camera.z = 0;
}

void Camera::tras() {
    m_velocidade = m_dir * (-escala_velocidade);
    m_pos = m_velocidade + m_pos;

    ponto_camera.x = m_pos.x;
    ponto_camera.z = 300;
}

void Camera::esquerda() {
    m_velocidade = m_left * escala_velocidade;
    m_pos = m_pos + m_velocidade;


    ponto_camera.x = 0;
    ponto_camera.z = m_pos.z;
}

void Camera::direita() {
    m_velocidade = m_left * (-escala_velocidade);
    m_pos = m_pos + m_velocidade;

    ponto_camera.x = 300;
    ponto_camera.z = m_pos.z;
}

float Camera::convertParaRadianos(float angulo) {

    return (angulo * valor_PI) / 180.0f;
}

void Camera::atualizarYaw(float dYaw) {
    valor_yaw += dYaw;
}
void Camera::atualizarYaw() {
    float angulo = convertParaRadianos(valor_yaw);
    m_dir.x = sin(angulo);
    m_dir.z = -cos(angulo);
    m_dir.normalizar();
}

void Camera::ativarVisaoCima() {
    gluLookAt(150, 300, 550, // Posição da câmera (x, y, z)
        150, 0, 0,    // Ponto para onde a câmera está olhando (x, y, z)
        0.0, 0.0, -3.0); //UP da camera

    // gluLookAt(150, 350, 150, // Posição da câmera (x, y, z)
    //     150, 0, 150,    // Ponto para onde a câmera está olhando (x, y, z)
    //     0.0, 0.0, 1.0); //UP da camera
}


void Camera::direita(std::vector<std::vector<int>>& coordenadas) {
    this->index_direcao = this->indexCalculado(DIREITA);

    ajustarCamera();
}

void Camera::frente(std::vector<std::vector<int>>& coordenadas) {
    this->index_direcao = this->indexCalculado(DIREITA);

    ajustarCamera();
}
void Camera::esquerda(std::vector<std::vector<int>>& coordenadas) {
    this->index_direcao = this->indexCalculado(ESQUERDA);

    ajustarCamera();
}
void Camera::tras(std::vector<std::vector<int>>& coordenadas) {
    this->index_direcao = this->indexCalculado(COSTA);

    ajustarCamera();
}

void Camera::ajustarCamera() {
    if (this->index_direcao == 0) {
        ponto_camera.x = m_pos.x;
        ponto_camera.z = 0;
    }

    if (this->index_direcao == 1) {
        ponto_camera.x = 300;
        ponto_camera.z = m_pos.z;
    }

    if (this->index_direcao == 2) {
        ponto_camera.x = m_pos.x;
        ponto_camera.z = 300;
    }

    if (this->index_direcao == 3) {
        ponto_camera.x = 0;
        ponto_camera.z = m_pos.z;
    }
}


std::vector<vector3d> Camera::andar(std::vector<std::vector<int>>& coordenadas) {
    vector3d direcao = this->direcoes[this->index_direcao];
    printf("[%f][%f][%f] index %d \n", direcao.x, direcao.y, direcao.z, this->index_direcao);
    m_velocidade = direcao * escala_velocidade;

    vector3d pos_temp = m_velocidade + m_pos;
    if (!contatoCenario(coordenadas, pos_temp.x, pos_temp.z)) {
        m_pos = pos_temp;
    }

    std::vector<vector3d> coordenadas_atualizadas;
    coordenadas_atualizadas.push_back(m_pos);
    coordenadas_atualizadas.push_back(ponto_camera);
    printf("Camera: %f, %f , %f \n", m_pos.x, m_pos.y, m_pos.z);
    return coordenadas_atualizadas;
}

bool Camera::contatoCenario(std::vector<std::vector<int>>& coordenadas, int x, int z) {
    int x_linha = x + 5;
    int z_linha = z + 5;

    if (x < 0 || x >= 300 || x_linha < 0 || x_linha >= 300) {
        return true;
    }

    if (z < 0 || z >= 300 || z_linha < 0 || z_linha >= 300) {
        return true;
    }

    if (coordenadas[x][z] == 1 || coordenadas[x][z_linha] == 1 || coordenadas[x_linha][z] == 1 || coordenadas[x_linha][z_linha] == 1) {
        // printf("\nxz[%d][%d] <=> xz_linha[%d][%d] \n", x, z, x_linha, z_linha);
        // printf("[%d] <=> [%d] <=> [%d] <=> [%d]", coordenadas[x][z], coordenadas[x][z_linha], coordenadas[x_linha][z], coordenadas[x_linha][z_linha]);
        return true;
    }

    return false;
}


int Camera::indexCalculado(Sentido sentido) {
    int novo_index = this->index_direcao;
    if (sentido == FRENTE) {
        novo_index + 0;
    }

    if (sentido == DIREITA) {
        if (novo_index + 1 > 3) {
            novo_index = 0;
        }
        else {
            novo_index += 1;
        }
    }

    if (sentido == COSTA) {
        if (novo_index + 2 > 3) {
            int temp = (novo_index - 2);
            novo_index = (temp < 0 ? temp * (-1) : temp);
        }
        else {
            novo_index += 2;
        }
    }

    if (sentido == ESQUERDA) {
        if (novo_index - 1 < 0) {
            novo_index = 3;
        }
        else {
            novo_index -= 1;
        }
    }

    return novo_index;
}