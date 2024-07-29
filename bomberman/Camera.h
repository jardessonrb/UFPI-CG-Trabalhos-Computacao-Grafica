#ifndef CAMERA_H
#define CAMERA_H
#include "vector3d.h"

enum Sentido { FRENTE, DIREITA, COSTA, ESQUERDA };

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
    vector3d pos_direita = vector3d(1, 0, 0);
    vector3d pos_esquerda = vector3d(-1, 0, 0);
    vector3d pos_frente = vector3d(0, 0, -1);
    vector3d pos_costas = vector3d(0, 0, 1);

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
    // void alterarDirecao(Sentido sentidoEscolhido);
    void virarDireita();
    void virarEsquerda();

private:
    float convertParaRadianos(float angulo);
};

#endif // CAMERA_H