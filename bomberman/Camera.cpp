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

Camera::Camera(vector3d pos) {
    m_pos = pos;
    ponto_camera = vector3d(0, 0, 0);
    up_camera = vector3d(0, 0, 0);

    m_dir = vector3d(0, 0, -1);
    m_left = vector3d(-1, 0, 0);
    m_up = vector3d(0, 1, 0);
    m_velocidade = vector3d(0, 0, 0);
}

Camera::Camera(vector3d pos, vector3d ponto, vector3d up) {
    m_pos = pos;
    ponto_camera = ponto;
    up_camera = up;

    m_dir = vector3d(0, 0, -1);
    m_left = vector3d(-1, 0, 0);
    m_up = vector3d(0, 1, 0);
    m_velocidade = vector3d(0, 0, 0);
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

    // printf("%f %f %f \n", m_pos.x, m_pos.y, m_pos.z);
}

void Camera::tras() {
    m_velocidade = m_dir * (-escala_velocidade);
    m_pos = m_velocidade + m_pos;

    ponto_camera.x = m_pos.x;
    ponto_camera.z = 300;

    // printf("%f %f %f \n", m_pos.x, m_pos.y, m_pos.z);
}

void Camera::esquerda() {
    m_velocidade = m_left * escala_velocidade;
    m_pos = m_pos + m_velocidade;


    ponto_camera.x = 0;
    ponto_camera.z = m_pos.z;

    // printf("%f %f %f \n", m_pos.x, m_pos.y, m_pos.z);
}

void Camera::direita() {
    m_velocidade = m_left * (-escala_velocidade);
    m_pos = m_pos + m_velocidade;

    ponto_camera.x = 300;
    ponto_camera.z = m_pos.z;

    // printf("%f %f %f \n", m_pos.x, m_pos.y, m_pos.z);
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
    gluLookAt(150, 200, 550, // Posição da câmera (x, y, z)
        150, 0, 0,    // Ponto para onde a câmera está olhando (x, y, z)
        0.0, 0.0, -2.0); //UP da camera

    // gluLookAt(150, 350, 150, // Posição da câmera (x, y, z)
    //     150, 0, 150,    // Ponto para onde a câmera está olhando (x, y, z)
    //     0.0, 0.0, 1.0); //UP da camera
}


// void Camera::alterarDirecao(Sentido sentidoEscolhido) {
//     if (sentidoEscolhido == sentido) {
//         return;
//     }

//     if (sentidoEscolhido == FRENTE) {
//         sentido = FRENTE;
//         ponto_camera.z = 0;
//         ponto_camera.x = m_pos.x;

//         m_dir = this->pos_frente;
//     }

//     if (sentidoEscolhido == DIREITA) {
//         sentido = DIREITA;
//         ponto_camera.x = 300;
//         ponto_camera.z = m_pos.z;

//         m_dir = this->pos_direita;
//     }

//     if (sentidoEscolhido == COSTA) {
//         sentido = COSTA;
//         ponto_camera.x = m_pos.x;
//         ponto_camera.z = 300;

//         m_dir = this->pos_costas;
//     }

//     if (sentidoEscolhido == ESQUERDA) {
//         sentido = ESQUERDA;
//         ponto_camera.x = 0;
//         ponto_camera.z = m_pos.z;

//         m_dir = this->pos_esquerda;
//     }
// }

// void Camera::virarDireita() {
//     if (sentido == FRENTE) {
//         sentido = DIREITA;
//         ponto_camera.x = 300;
//         m_dir = this->pos_direita;
//     }

//     if (sentido == DIREITA) {
//         sentido = COSTA;
//         ponto_camera.z = 300;
//         m_dir = this->pos_costas;
//     }

//     if (sentido == COSTA) {
//         sentido = ESQUERDA;

//         ponto_camera.x = 0;
//         m_dir = this->pos_esquerda;
//     }

//     if (sentido == ESQUERDA) {
//         sentido = FRENTE;
//         m_dir = this->pos_frente;
//         ponto_camera.z = 0;
//     }
// }

// void Camera::virarEsquerda() {
// if (sentido == FRENTE) {
//     sentido = ESQUERDA;
//     m_dir = this->pos_esquerda;
//     ponto_camera.x = 0;
// }

// if (sentido == ESQUERDA) {
//     sentido = COSTA;
//     m_dir = this->pos_costas;
//     ponto_camera.z = 300;
// }

// if (sentido == COSTA) {
//     sentido = DIREITA;
//     m_dir = this->pos_direita;
//     ponto_camera.x = 300;
// }

// if (sentido == DIREITA) {
//     sentido = FRENTE;
//     m_dir = this->pos_frente;
//     ponto_camera.z = 0;
// }
// }