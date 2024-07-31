#ifndef BONECO_H
#define BONECO_H
#include "../vector3d.h"

// enum SentidoB { FRENTE, DIREITA, COSTA, ESQUERDA };

class Boneco {
private:
    vector3d m_pos;
    vector3d m_pos_temp;
    vector3d m_dir;
    vector3d m_left;
    vector3d m_up;
    vector3d m_velocidade;

    vector3d ponto_Boneco;
    vector3d up_Boneco;
    float escala_velocidade = 5.0f;

    float valor_yaw;
    float valor_PI = 3.14159265;

    Sentido sentido = FRENTE;
    vector3d pos_direita = vector3d(1, 0, 0);
    vector3d pos_esquerda = vector3d(-1, 0, 0);
    vector3d pos_frente = vector3d(0, 0, -1);
    vector3d pos_costas = vector3d(0, 0, 1);

    bool contatoCenario(std::vector<std::vector<int>>& coordenadas, int x, int z);

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
    vector3d getPosTem();
    void commitPos();
    vector3d getPos();
    void direita(std::vector<std::vector<int>>& coordenadas);
    void frente(std::vector<std::vector<int>>& coordenadas);
    void esquerda(std::vector<std::vector<int>>& coordenadas);
    void tras(std::vector<std::vector<int>>& coordenadas);
    void soltarBomba();


private:
    float convertParaRadianos(float angulo);
};

#endif // BONECO_H