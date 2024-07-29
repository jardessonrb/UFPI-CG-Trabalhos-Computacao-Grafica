#ifndef CAMERA_H
#define CAMERA_H
#include "vector3d.h"

enum Sentido { FRENTE, TRAS, DIREITA, ESQUERDA };

class Camera {
private:
    vector3d m_pos;
    vector3d m_dir;
    vector3d m_left;
    vector3d m_up;
    vector3d m_velocidade;

    vector3d ponto_camera;
    vector3d up_camera;
    float escala_velocidade = 10.0f;

    float valor_yaw;
    float valor_PI = 3.14159265;

    Sentido sentido = FRENTE;

public:
    Camera(vector3d pos);
    Camera(vector3d pos, vector3d ponto, vector3d up);
    static Camera initOrigem();
    ~Camera();
    void ativar();
    void ativarVisaoCima();
    void frente();
    void tras();
    void esquerda();
    void direita();
    void atualizarYaw(float dYaw);
    void atualizarYaw();

private:
    float convertParaRadianos(float angulo);
};

#endif // CAMERA_H