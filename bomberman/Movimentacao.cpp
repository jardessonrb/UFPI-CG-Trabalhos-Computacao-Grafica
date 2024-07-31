#include "Movimentacao.h"

Movimentacao::Movimentacao(vector3d pos) {
    m_pos = pos;
    ponto_camera = vector3d(0, 0, 0);
    up_camera = vector3d(0, 0, 0);

    m_dir = vector3d(0, 0, -1);
    m_left = vector3d(-1, 0, 0);
    m_up = vector3d(0, 1, 0);
    m_velocidade = vector3d(0, 0, 0);
}

Movimentacao::Movimentacao(vector3d pos, vector3d ponto, vector3d up) {
    m_pos = pos;
    ponto_camera = ponto;
    up_camera = up;

    m_dir = vector3d(0, 0, -1);
    m_left = vector3d(-1, 0, 0);
    m_up = vector3d(0, 1, 0);
    m_velocidade = vector3d(0, 0, 0);
}

Movimentacao::~Movimentacao() {
}

void Movimentacao::ativar() {
    gluLookAt(m_pos.x, m_pos.y, m_pos.z, ponto_camera.x, ponto_camera.y, ponto_camera.z, up_camera.x, up_camera.y, up_camera.z);
}

void Movimentacao::frente() {
    m_velocidade = m_dir * escala_velocidade;
    m_pos = m_velocidade + m_pos;

    ponto_camera.x = m_pos.x;
    ponto_camera.z = 0;

    printf("%f %f %f \n", m_pos.x, m_pos.y, m_pos.z);
}

void Movimentacao::tras() {
    m_velocidade = m_dir * (-escala_velocidade);
    m_pos = m_velocidade + m_pos;

    ponto_camera.x = m_pos.x;
    ponto_camera.z = 300;

    printf("%f %f %f \n", m_pos.x, m_pos.y, m_pos.z);
}

void Movimentacao::esquerda() {
    m_velocidade = m_left * escala_velocidade;
    m_pos = m_pos + m_velocidade;

    ponto_camera.x = 0;
    ponto_camera.z = m_pos.z;

    printf("%f %f %f \n", m_pos.x, m_pos.y, m_pos.z);
}

void Movimentacao::direita() {
    m_velocidade = m_left * (-escala_velocidade);
    m_pos = m_pos + m_velocidade;

    ponto_camera.x = 300;
    ponto_camera.z = m_pos.z;

    printf("%f %f %f \n", m_pos.x, m_pos.y, m_pos.z);
}

void Movimentacao::ativarVisaoCima() {
    gluLookAt(150, 370, 150, // Posição da câmera (x, y, z)
        150, -0.5, 150, // Ponto para onde a câmera está olhando (x, y, z)
        0.0, 0.0, 1.0); // UP da câmera
}
