#ifndef BALAO_H
#define BALAO_H
#include "../vector3d.h"

class Balao {
private:
    vector3d m_pos;
    vector3d m_pos_temp;
    vector3d m_dir;
    vector3d m_left;
    vector3d m_up;
    vector3d m_velocidade;

    vector3d ponto_camera;
    vector3d up_camera;
    float escala_velocidade = 5.0f;

    unsigned int tempo_ultimo_movimento = 0;

    Sentido sentido = FRENTE;
    vector3d pos_direita = vector3d(1, 0, 0);
    vector3d pos_esquerda = vector3d(-1, 0, 0);
    vector3d pos_frente = vector3d(0, 0, -1);
    vector3d pos_costas = vector3d(0, 0, 1);
    bool is_balao_vivo = true;
    bool contatoCenario(std::vector<std::vector<int>>& coordenadas, int x, int z);
    int ultima_direcao_escolhida = 0;
    bool contato_com_cenario_ultima_posicao = false;
    int contador_passos = 0;

public:
    Balao(vector3d pos);
    Balao(vector3d pos, vector3d ponto, vector3d up);
    ~Balao();
    void frente();
    void tras();
    void esquerda();
    void direita();
    vector3d getPos();
    void direita(std::vector<std::vector<int>>& coordenadas);
    void frente(std::vector<std::vector<int>>& coordenadas);
    void esquerda(std::vector<std::vector<int>>& coordenadas);
    void tras(std::vector<std::vector<int>>& coordenadas);
    void morrer();
    bool is_vivo();
    vector3d calcular_direcao(std::vector<std::vector<int>>& coordenadas);
};

#endif // BALAO_H