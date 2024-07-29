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

Boneco::Boneco(vector3d pos) {
    m_pos = pos;
    ponto_Boneco = vector3d(0, 0, 0);
    up_Boneco = vector3d(0, 0, 0);

    m_dir = vector3d(0, 0, -1);
    m_left = vector3d(-1, 0, 0);
    m_up = vector3d(0, 1, 0);
    m_velocidade = vector3d(0, 0, 0);
}

Boneco::Boneco(vector3d pos, vector3d ponto, vector3d up) {
    m_pos = pos;
    ponto_Boneco = ponto;
    up_Boneco = up;

    m_dir = vector3d(0, 0, -1);
    m_left = vector3d(-1, 0, 0);
    m_up = vector3d(0, 1, 0);
    m_velocidade = vector3d(0, 0, 0);
}

Boneco::~Boneco() {
}

void Boneco::ativar() {
    gluLookAt(m_pos.x, m_pos.y, m_pos.z, ponto_Boneco.x, ponto_Boneco.y, ponto_Boneco.z, up_Boneco.x, up_Boneco.y, up_Boneco.z);
}

void Boneco::frente() {
    m_velocidade = m_dir * escala_velocidade;
    m_pos = m_velocidade + m_pos;

    ponto_Boneco.x = m_pos.x;
    ponto_Boneco.z = 0;

    printf("%f %f %f \n", m_pos.x, m_pos.y, m_pos.z);
}

void Boneco::tras() {
    m_velocidade = m_dir * (-escala_velocidade);
    m_pos = m_velocidade + m_pos;

    ponto_Boneco.x = m_pos.x;
    ponto_Boneco.z = 300;

    printf("%f %f %f \n", m_pos.x, m_pos.y, m_pos.z);
}

void Boneco::esquerda() {
    m_velocidade = m_left * escala_velocidade;
    m_pos = m_pos + m_velocidade;


    ponto_Boneco.x = 0;
    ponto_Boneco.z = m_pos.z;

    printf("%f %f %f \n", m_pos.x, m_pos.y, m_pos.z);
}

void Boneco::direita() {
    m_velocidade = m_left * (-escala_velocidade);
    m_pos = m_pos + m_velocidade;

    ponto_Boneco.x = 300;
    ponto_Boneco.z = m_pos.z;

    printf("%f %f %f \n", m_pos.x, m_pos.y, m_pos.z);
}

float Boneco::convertParaRadianos(float angulo) {

    return (angulo * valor_PI) / 180.0f;
}

void Boneco::atualizarYaw(float dYaw) {
    valor_yaw += dYaw;
}
void Boneco::atualizarYaw() {
    float angulo = convertParaRadianos(valor_yaw);
    m_dir.x = sin(angulo);
    m_dir.z = -cos(angulo);
    m_dir.normalizar();
}

void Boneco::ativarVisaoCima() {
    gluLookAt(150, 370, 150, // Posição da câmera (x, y, z)
        150, -0.5, 150,    // Ponto para onde a câmera está olhando (x, y, z)
        0.0, 0.0, 1.0); //UP da Boneco
}

vector3d Boneco::getPos() {
    return this->m_pos;
}


// void Boneco::alterarDirecao(SentidoB sentidoEscolhido) {
//     if (sentidoEscolhido == sentido) {
//         return;
//     }

//     if (sentidoEscolhido == FRENTE) {
//         sentido = FRENTE;
//         ponto_Boneco.z = 0;
//         ponto_Boneco.x = m_pos.x;

//         m_dir = this->pos_frente;
//     }

//     if (sentidoEscolhido == DIREITA) {
//         sentido = DIREITA;
//         ponto_Boneco.x = 300;
//         ponto_Boneco.z = m_pos.z;

//         m_dir = this->pos_direita;
//     }

//     if (sentidoEscolhido == COSTA) {
//         sentido = COSTA;
//         ponto_Boneco.x = m_pos.x;
//         ponto_Boneco.z = 300;

//         m_dir = this->pos_costas;
//     }

//     if (sentidoEscolhido == ESQUERDA) {
//         sentido = ESQUERDA;
//         ponto_Boneco.x = 0;
//         ponto_Boneco.z = m_pos.z;

//         m_dir = this->pos_esquerda;
//     }
// }

// void Boneco::virarDireita() {
//     if (sentido == FRENTE) {
//         sentido = DIREITA;
//         ponto_Boneco.x = 300;
//         m_dir = this->pos_direita;
//     }

//     if (sentido == DIREITA) {
//         sentido = COSTA;
//         ponto_Boneco.z = 300;
//         m_dir = this->pos_costas;
//     }

//     if (sentido == COSTA) {
//         sentido = ESQUERDA;

//         ponto_Boneco.x = 0;
//         m_dir = this->pos_esquerda;
//     }

//     if (sentido == ESQUERDA) {
//         sentido = FRENTE;
//         m_dir = this->pos_frente;
//         ponto_Boneco.z = 0;
//     }
// }

// void Boneco::virarEsquerda() {
//     if (sentido == FRENTE) {
//         sentido = ESQUERDA;
//         m_dir = this->pos_esquerda;
//         ponto_Boneco.x = 0;
//     }

//     if (sentido == ESQUERDA) {
//         sentido = COSTA;
//         m_dir = this->pos_costas;
//         ponto_Boneco.z = 300;
//     }

//     if (sentido == COSTA) {
//         sentido = DIREITA;
//         m_dir = this->pos_direita;
//         ponto_Boneco.x = 300;
//     }

//     if (sentido == DIREITA) {
//         sentido = FRENTE;
//         m_dir = this->pos_frente;
//         ponto_Boneco.z = 0;
//     }
// }