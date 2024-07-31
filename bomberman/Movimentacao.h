#ifndef MOVIMENTACAO_H
#define MOVIMENTACAO_H

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
#include "vector3d.h"

class Movimentacao {
protected:
    vector3d m_pos;
    vector3d ponto_camera;
    vector3d up_camera;
    vector3d m_dir;
    vector3d m_left;
    vector3d m_up;
    vector3d m_velocidade;
    float escala_velocidade;
    float valor_yaw;
    const float valor_PI = 3.14159265359f;

public:
    Movimentacao(vector3d pos);
    Movimentacao(vector3d pos, vector3d ponto, vector3d up);
    virtual ~Movimentacao();

    virtual void ativar();
    virtual void frente();
    virtual void tras();
    virtual void esquerda();
    virtual void direita();
    virtual void ativarVisaoCima();
};

#endif // MOVIMENTACAO_H