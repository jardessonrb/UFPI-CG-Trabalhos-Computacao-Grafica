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
    printf("%f %f %f \n", m_pos.x, m_pos.y, m_pos.z);
}

void Camera::tras() {
    m_velocidade = m_dir * (-escala_velocidade);
    m_pos = m_velocidade + m_pos;

    ponto_camera.x = m_pos.x;
    printf("%f %f %f \n", m_pos.x, m_pos.y, m_pos.z);
}

void Camera::esquerda() {
    m_velocidade = m_left * escala_velocidade;
    m_pos = m_pos + m_velocidade;
    ponto_camera.x = m_pos.x;

    printf("%f %f %f \n", m_pos.x, m_pos.y, m_pos.z);
}

void Camera::direita() {
    m_velocidade = m_left * (-escala_velocidade);
    m_pos = m_pos + m_velocidade;
    ponto_camera.x = m_pos.x;
    printf("%f %f %f \n", m_pos.x, m_pos.y, m_pos.z);
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
    gluLookAt(150, 370, 150, // Posição da câmera (x, y, z)
        150, -0.5, 150,    // Ponto para onde a câmera está olhando (x, y, z)
        0.0, 0.0, 1.0); //UP da camera
}
