#ifndef BONECO_H
#define BONECO_H
#include "../vector3d.h"

// enum SentidoB { FRENTE, DIREITA, COSTA, ESQUERDA };

class Boneco {
private:
    vector3d m_pos;
    vector3d m_dir;
    vector3d m_left;
    vector3d m_up;
    vector3d m_velocidade;

    vector3d ponto_Boneco;
    vector3d up_Boneco;
    float escala_velocidade = 10.0f;

    float valor_yaw;
    float valor_PI = 3.14159265;

    Sentido sentido = FRENTE;
    vector3d pos_direita = vector3d(1, 0, 0);
    vector3d pos_esquerda = vector3d(-1, 0, 0);
    vector3d pos_frente = vector3d(0, 0, -1);
    vector3d pos_costas = vector3d(0, 0, 1);

public:
    Boneco(vector3d pos);
    Boneco(vector3d pos, vector3d ponto, vector3d up);
    static Boneco initOrigem();
    ~Boneco();
    void ativar();
    void ativarVisaoCima();
    void frente();
    void tras();
    void esquerda();
    void direita();
    void atualizarYaw(float dYaw);
    void atualizarYaw();
    // void alterarDirecao(SentidoB sentidoEscolhido);
    void virarDireita();
    void virarEsquerda();
    vector3d getPos();

private:
    float convertParaRadianos(float angulo);
};

#endif // BONECO_H