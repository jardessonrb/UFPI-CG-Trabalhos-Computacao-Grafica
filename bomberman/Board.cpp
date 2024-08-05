#include "board.h"

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
#include "loader/Loader.cpp"

Loader loader;

Board::~Board() {
}

Board::Board() {
}

Board::Board(CGQuadrado cg) {
    this->coordenada_chao_board = std::vector<std::vector<int>>(300, std::vector(300, 0));
    this->baloes = std::vector<Balao>();

    this->cgQuadrado = cg;
    this->coordenada_caixotes = std::vector<std::vector<int>>(16, std::vector(3, 0));
    this->coordenada_caixotes = {
        { 140, 40, 1},
        { 20, 200, 1},
        { 20, 160, 1},
        { 60, 20, 1},
        { 100, 20, 1},
        { 140, 20, 1},
        { 200, 20, 1},
        { 140, 160, 1},
        { 140, 180, 1},
        { 140, 200, 1},
        { 140, 220, 1},
        { 140, 240, 1},
        { 140, 260, 1},
        { 240, 140, 1},
        { 240, 180, 1},
        { 240, 220, 1},
    };
}

Board::Board(CGQuadrado cg, std::vector<Balao> baloes) {
    this->coordenada_chao_board = std::vector<std::vector<int>>(300, std::vector(300, 0));
    this->baloes = baloes;
    this->cgQuadrado = cg;
    this->coordenada_caixotes = std::vector<std::vector<int>>(16, std::vector(3, 0));
    this->coordenada_caixotes = {
        { 140, 40, 1},
        { 20, 200, 1},
        { 20, 160, 1},
        { 60, 20, 1},
        { 100, 20, 1},
        { 140, 20, 1},
        { 200, 20, 1},
        { 140, 160, 1},
        { 140, 180, 1},
        { 140, 200, 1},
        { 140, 220, 1},
        { 140, 240, 1},
        { 140, 260, 1},
        { 240, 140, 1},
        { 240, 180, 1},
        { 240, 220, 1},
    };
}

bool Board::isParede(int x, int z) {
    return false;
}

void Board::desenhar_chao() {
    glColor3f(0.0f, 0.0f, 0.0f); // Define a cor preta
    glBegin(GL_QUADS); // Inicia o desenho de um quadrilátero
    glVertex3f(0.0f, altura_y, 300.0f); // Primeiro vértice
    glVertex3f(300.0f, altura_y, 300.0f);  // Segundo vértice
    glVertex3f(300.0f, altura_y, 0.0f);   // Terceiro vértice
    glVertex3f(0.0f, altura_y, 0.0f);  // Quarto vértice
    glEnd(); // Termina o desenho do quadrilátero
}

void Board::desenhar_obstaculos() {
    glColor3f(0.0, 0.0, 0.5);

    //Lado esquerdo e direito
    for (int z = 0; z < this->comprimento; z += this->tamanho_bloco)
    {
        this->cgQuadrado.desenhaQuadrado(0, z, this->tamanho_bloco);
        this->cgQuadrado.desenhaQuadrado(this->comprimento - this->tamanho_bloco, z, this->tamanho_bloco);

        this->marcar_matriz(0, z, 1);
        this->marcar_matriz(this->comprimento - this->tamanho_bloco, z, 1);
    }

    // Lado frente e limite
    for (int x = 0; x < this->comprimento; x += this->tamanho_bloco)
    {
        this->cgQuadrado.desenhaQuadrado(x, 0, this->tamanho_bloco);
        this->cgQuadrado.desenhaQuadrado(x, largura - this->tamanho_bloco, this->tamanho_bloco);

        this->marcar_matriz(x, 0, 1);
        this->marcar_matriz(x, largura - this->tamanho_bloco, 1);
    }

    //Quadrados Alternados
    bool desenha = true;
    for (int x = (2 * this->tamanho_bloco), z = (2 * this->tamanho_bloco); x < comprimento; x += this->tamanho_bloco)
    {
        if (desenha) {
            bool desenha_linha = true;
            for (int z_linha = z; z_linha < comprimento; z_linha += this->tamanho_bloco)
            {
                if (desenha_linha) {
                    this->cgQuadrado.desenhaQuadrado(x, z_linha, this->tamanho_bloco);
                    this->marcar_matriz(x, z_linha, 1);
                }

                desenha_linha = !desenha_linha;
            }
        }
        desenha = !desenha;
    }
    //Desenha cubos
    glColor3f(0.5, 0.4, 0.0);
    for (int i = 0; i < this->coordenada_caixotes.size(); i++)
    {
        if (this->coordenada_caixotes[i][2]) {
            this->cgQuadrado.desenhaQuadrado(this->coordenada_caixotes[i][0], this->coordenada_caixotes[i][1], this->tamanho_bloco);
            this->marcar_matriz(this->coordenada_caixotes[i][0], this->coordenada_caixotes[i][1], 2);
        }
    }

}

void Board::apagar_caixote(int x, int z) {
    for (int i = 0; i < this->coordenada_caixotes.size(); i++)
    {
        if (this->coordenada_caixotes[i][0] == x && this->coordenada_caixotes[i][1] == z) {
            this->coordenada_caixotes[i][2] = 0;
            this->marcar_matriz(this->coordenada_caixotes[i][0], this->coordenada_caixotes[i][1], 0);
        }
    }

}

void Board::detectar_explosao(int x, int z, int tamanho) {
    int x_linha = x + tamanho;
    int z_linha = z + tamanho;


    if (x < 0 || x >= 300 || x_linha < 0 || x_linha >= 300 || z < 0 || z >= 300 || z_linha < 0 || z_linha >= 300) {
        return;
    }

    for (int i = 0; i < this->coordenada_caixotes.size(); i++)
    {
        // printf("%d - %d - %d - %d ", x, z, x_linha, z_linha);
        if (this->coordenada_chao_board[x][z] == 2) {
            // printf("%d - %d detectado", x, z);
            this->apagar_caixote(x, z);
        }

        if (this->coordenada_chao_board[x][z_linha] == 2) {
            // printf("%d - %d detectado", x, z_linha);
            this->apagar_caixote(x, z_linha);
        }

        if (this->coordenada_chao_board[x_linha][z] == 2) {
            // printf("%d - %d detectado", x_linha, z);
            this->apagar_caixote(x_linha, z);
        }

        if (this->coordenada_chao_board[x_linha][z_linha] == 2) {
            // printf("%d - %d detectado", x_linha, z_linha);
            this->apagar_caixote(x_linha, z_linha);
        }
    }

    //Explosão dos balões
    for (int i = 0; i < this->baloes.size(); i++)
    {
        vector3d posicao_balao = this->baloes[i].getPos();
        int x_balao = posicao_balao.x;
        int z_balao = posicao_balao.z;

        int x_linha = x_balao - x;
        int z_linha = z_balao - z;

        x_linha = x_linha < 0 ? (-1 * x_linha) : x_linha;
        z_linha = z_linha < 0 ? (-1 * z_linha) : z_linha;

        if (x_linha <= 10 && z_linha <= 10) {
            this->baloes[i].morrer();
            // printf("Balao %d morreu [%d][%d]", i, x_balao, z_balao);
        }
    }

    //Explosão do Boneco
    vector3d posicao_boneco = this->boneco.getPos();
    int x_boneco = posicao_boneco.x;
    int z_boneco = posicao_boneco.z;

    int x_linha_boneco = x_boneco - x;
    int z_linha_boneco = z_boneco - z;

    x_linha_boneco = x_linha_boneco < 0 ? (-1 * x_linha_boneco) : x_linha_boneco;
    z_linha_boneco = z_linha_boneco < 0 ? (-1 * z_linha_boneco) : z_linha_boneco;

    if (x_linha_boneco <= 10 && z_linha_boneco <= 10) {
        this->boneco.morrer();
        this->is_boneco_vivo = false;
    }

}

void Board::desenhar_linhas_no_chao() {
    int z_inicial = 0;

    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_LINES);
    for (int i = 0; i <= this->largura && z_inicial >= -this->largura; i += this->tamanho_bloco, z_inicial += this->tamanho_bloco)
    {

        //Linha comprimento
        glVertex3f(i, altura_y, 0);
        glVertex3f(i, altura_y, this->largura);

        //Linha largura
        glVertex3f(0, altura_y, z_inicial);
        glVertex3f(this->largura, altura_y, z_inicial);


    }
    glEnd();
}

void Board::iniciar_matriz() {

}

void Board::marcar_matriz(int x, int z, int peso) {
    for (int i = x; i < (x + this->tamanho_bloco); i++)
    {
        for (int j = z; j < (z + this->tamanho_bloco); j++)
        {
            if (i >= 0 && i < this->largura && j >= 0 && j < this->comprimento) {
                this->coordenada_chao_board[i][j] = peso;
            }
        }
    }
}

void Board::desenhar_personagem(int x, int z, int tamanho) {
    this->cgQuadrado.desenhaQuadrado(x, z, tamanho);
}

std::vector<std::vector<int>>& Board::getCoordenadas() {
    return this->coordenada_chao_board;
}

void Board::desenhar_bomba(int x, int z, int tamanho) {
    glColor3f(0.67, 0.35, 0.12);
    this->cgQuadrado.desenhaQuadrado(x, z, tamanho);
}

void Board::desenhar_cenario() {
    desenhar_chao();
    desenhar_linhas_no_chao();
    iniciar_matriz();
    desenhar_obstaculos();
    ativar_bomba();
    desenhar_baloes();
    if (!this->isCameraPrimeiraPessoa) {
        glColor3f(1.0, 0.0, 0.0);
        vector3d coordenada_boneco_a = this->boneco.getPos();
        desenhar_personagem(coordenada_boneco_a.x, coordenada_boneco_a.z, 10);
    }

}

void Board::ativar_bomba() {
    if (this->isSoltarBomba && this->inicio_tempo_bomba > glutGet(GLUT_ELAPSED_TIME)) {
        this->desenhar_bomba(this->posicao_bomba.first, this->posicao_bomba.second, this->tamanho_bomba);
    }

    if (this->inicio_tempo_bomba < glutGet(GLUT_ELAPSED_TIME)) {
        this->isSoltarBomba = false;
        if (!this->isBombaEmOutoro && this->quantidade_bombas == 1) {
            this->isBombaEmOutoro = true;
            this->inicio_tempo_bomba_estouro = (glutGet(GLUT_ELAPSED_TIME) + 2000);
            this->quantidade_bombas = 0;
        }
    }

    if (this->isBombaEmOutoro && (this->inicio_tempo_bomba_estouro > glutGet(GLUT_ELAPSED_TIME))) {
        this->estourar_bomba(this->posicao_bomba.first, this->posicao_bomba.second, this->tamanho_bloco, this->potencia_bomba);
    }
    else if (this->inicio_tempo_bomba_estouro < glutGet(GLUT_ELAPSED_TIME)) {
        this->isBombaEmOutoro = false;
    }
}

void Board::soltar_bomba(int x, int z, int tamanho, int potencia) {
    this->potencia_bomba = potencia;
    this->tamanho_bomba = tamanho;
    if (!this->isSoltarBomba && !this->isBombaEmOutoro) {
        this->isSoltarBomba = true;
        this->inicio_tempo_bomba = (glutGet(GLUT_ELAPSED_TIME) + 3000);
        this->quantidade_bombas = 1;
        this->posicao_bomba = std::pair<float, float>(x, z);
    }
}

void Board::estourar_bomba(int x, int z, int tamanho, int potencia) {
    x = (x / this->tamanho_bloco) * this->tamanho_bloco;
    z = (z / this->tamanho_bloco) * this->tamanho_bloco;

    for (int i = 0, z_linha_mais = z, z_linha_menos = z; i < potencia; i++, z_linha_mais += tamanho, z_linha_menos -= tamanho)
    {
        this->desenhar_bomba(x, z_linha_mais, tamanho);
        this->desenhar_bomba(x, z_linha_menos, tamanho);

        this->detectar_explosao(x, z_linha_mais, tamanho);
        this->detectar_explosao(x, z_linha_menos, tamanho);
    }

    for (int i = 0, x_linha_mais = x, x_linha_menos = x; i < potencia; i++, x_linha_mais += tamanho, x_linha_menos -= tamanho)
    {
        this->desenhar_bomba(x_linha_mais, z, tamanho);
        this->desenhar_bomba(x_linha_menos, z, tamanho);

        this->detectar_explosao(x_linha_mais, z, tamanho);
        this->detectar_explosao(x_linha_menos, z, tamanho);
    }
}

void Board::desenhar_baloes() {
    glColor3f(0.0f, 0.8f, 0.0f);
    for (int i = 0; i < this->baloes.size(); i++)
    {
        if (this->baloes[i].is_vivo()) {
            vector3d posicao_balao = this->baloes[i].calcular_direcao(getCoordenadas());
            this->desenhar_personagem(posicao_balao.x, posicao_balao.z, 10);
        }
    }
}

void Board::add_balao(Balao b) {
    this->baloes.push_back(b);
}

void Board::add_boneco(Boneco bnc) {
    this->boneco = bnc;
}

void Board::add_camera(Camera camera) {
    this->camera = camera;
}

void Board::alternar_camera() {
    this->isCameraPrimeiraPessoa = !this->isCameraPrimeiraPessoa;
}

void Board::evento_keyboard(char keyboard) {
    if (keyboard == 'w') {
        this->camera.frente(getCoordenadas());
        this->boneco.frente(getCoordenadas());

        std::vector<vector3d> coordenadas_boneco(1, this->boneco.getPos());
        this->camera.atualizar_posicao_camera(coordenadas_boneco);
    }
    if (keyboard == 'a') {
        this->camera.esquerda(getCoordenadas());
        this->boneco.esquerda(getCoordenadas());

        std::vector<vector3d> coordenadas_boneco(1, this->boneco.getPos());
        this->camera.atualizar_posicao_camera(coordenadas_boneco);
    }
    if (keyboard == 'd') {
        this->camera.direita(getCoordenadas());
        this->boneco.direita(getCoordenadas());

        std::vector<vector3d> coordenadas_boneco(1, this->boneco.getPos());
        this->camera.atualizar_posicao_camera(coordenadas_boneco);
    }
    if (keyboard == 's') {
        this->camera.tras(getCoordenadas());
        this->boneco.tras(getCoordenadas());

        std::vector<vector3d> coordenadas_boneco(1, this->boneco.getPos());
        this->camera.atualizar_posicao_camera(coordenadas_boneco);
    }

    if (keyboard == 'b') {
        vector3d coordenada_boneco_a = this->boneco.getPos();
        soltar_bomba(coordenada_boneco_a.x, coordenada_boneco_a.z, 5, 2);
    }

    if (keyboard == 'c') {
        alternar_camera();
    }

    if (keyboard == 'p' && this->isCameraPrimeiraPessoa) {
        std::vector<vector3d> coordenadas = this->camera.andar(getCoordenadas());
        this->boneco.atualizarCoordenada(coordenadas);
    }
}

void Board::ativar_camera() {
    if (this->isCameraPrimeiraPessoa) {
        this->camera.ativar();
    }
    else {
        this->camera.ativarVisaoCima();
    }
}

void Board::desenhar_chao_bomba_obj() {
    loader.loadOBJ("C:\\ufpi\\9 periodo\\COMPUTACAO_GRAFICA\\atividades\\opengl_glut\\bomberman\\models\\bomb.obj");
    glLoadIdentity();
    glTranslatef(30.0f, 5.0f, 240.0f);
    glScalef(1.2f, 1.2f, 1.2f);
    int cont = 0;
    for (const auto& face : loader.faces) {
        glBegin(GL_POLYGON);
        for (const int vertexIndex : face) {
            if (vertexIndex < loader.normals.size()) {
                glNormal3f(loader.normals[vertexIndex].x, loader.normals[vertexIndex].y, loader.normals[vertexIndex].z);
            }
            glVertex3f(loader.vertices[vertexIndex].x, loader.vertices[vertexIndex].y, loader.vertices[vertexIndex].z);
        }
        glEnd();
    }

    printf("Importou o .obj");
}

int Board::get_status_jogo() {
    if (!this->is_boneco_vivo) {
        return 2;
    }

    if (this->is_boneco_vivo && this->isBombaEmOutoro) {
        return 1;
    }

    return 0;
}