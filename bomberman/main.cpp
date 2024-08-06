/*
 * Computacao Grafica
 */

 /* Inclui os headers do OpenGL, GLU, e GLUT */
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
#include <cmath>
#include "glut_text.h"
#include "vector3d.h"
#include "Camera.cpp"
#include "Board.cpp"
#include "cg/CGQuadrado.cpp"
#include "textureRGB.h"
#include "boneco/Boneco.cpp"
#include "balao/Balao.cpp"
#include <vector>
#include "tocar/tocar.h"
#include <thread>


#define ESC 27 // ASCII para a tecla ESC
#define MAX_FPS 70 // Maximo de Frames Por Segundo (FPS) desejado
#define FPS 60 // FPS desejado atualmente
#define KEY_COUNT 256
#define QUANT_TEX 3 //Quantidade de texturas que serao utilizadas


unsigned int id_texturas[QUANT_TEX];
GLint apply_texture = GL_ADD;

//Minhas Variáveis

Board board;
const int chao_coord_y = -0.5f;
bool camera_primeira_pessoa = false;
int som_ativo = 1;
bool keyStates[KEY_COUNT];
const int LARGURA_WINDOW = 800;
const int COMPRIMENTO_WINDOW = 700;
static int ultima_posicao_mouse = 0;
static bool primeiro_click_mouse = true;
static bool btn_mouse_esquerdo_pressionado = false;
bool is_janela_2_ativada = false;
int id_segunda_janela;

int fps_desejado = FPS / 2; // variavel para alterar os frames por segundo desejado
int fps = 0; //contador de frames por segundo
float R = 0.0, G = 0.0, B = 0.0; //Cores do desenho (inicial: cor azul)

/*
 * Declaracoes antecipadas (forward) das funcoes (assinaturas)
 */
void init_glut(const char* window_name, int argc, char** argv);
void draw_object_smooth(void);
void display(void);
void reshape(int w, int h);
void timer(int value);
void keyboard(unsigned char key, int x, int y);
void keyboardUp(unsigned char key, int x, int y);
void keyboardOnpress(unsigned char key, int x, int y);
void iniciar_jogo();
void desenharJogo();
void play_som();


void iniciar_nova_janela();
void display_janela_2(void);
void reshape_janela_2(int w, int h);
void mouse_click_callback_janela_2(int button, int state, int x, int y);
void fechar_segunda_janela();

/*
 * Funcao principal
 */
int main(int argc, char** argv) {
    iniciar_jogo();

    /* inicia o GLUT e alguns parametros do OpenGL */
    init_glut("Bomberman 3D - Jardesson Ribeiro", argc, argv);

    std::thread soundThread(play_som);

    glutMainLoop();

    // // Espera a thread do som terminar antes de sair
    soundThread.join();

    return EXIT_SUCCESS;
}

/*
 * inicia o GLUT e alguns parametros do OpenGL
 */
void init_glut(const char* nome_janela, int argc, char** argv) {
    // Inicia o GLUT
    glutInit(&argc, argv);

    // Inicia o display usando RGB, double-buffering e Z-buffering
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(LARGURA_WINDOW, COMPRIMENTO_WINDOW);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(nome_janela);
    memset(keyStates, 0, sizeof(keyStates));
    // Define as funcoes de callback
    glutKeyboardFunc(keyboard);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardUpFunc(keyboardUp);
    glutTimerFunc(1000 / fps_desejado, timer, 0); //(mseg, timer, value)


    /*
     *  Ativacao dos parametros do modelo de iluminacao para a Luz 0
     */
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_Ka);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_Kd);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_Ks);

    /*
     *  Ativacao dos parametros do material para uso do modelo de iluminacao
     *  Para usar este material a iluminacao (GL_LIGHTING) deve estar ativa
     *  e a cor do material (GL_COLOR_MATERIAL) desativada
     */
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, material_Ka);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material_Kd);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material_Ks);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, material_Ke);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, material_Se);

    glEnable(GL_LIGHTING); // Ativa o modelo de iluminacao
    glEnable(GL_LIGHT0); // Ativa a Luz 0. O OpenGL suporta pelo menos 8 pontos de luz.


    /*
     *  Configurando o OpenGL para o uso de Texturas
     */
     //Gera nomes identificadores (IDs) para as texturas
    glGenTextures(QUANT_TEX, id_texturas);

    //Ativa a textura que sera criada ou usada no momento, por meio do seu ID.
    glBindTexture(GL_TEXTURE_2D, id_texturas[0]);

    //Cria a extrutura da textura na mem�ria
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, TEXTURE_WIDTH, TEXTURE_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, texture_data_3);

    //Definicao dos parametros da textura
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    //Define como a textura sera aplicada ao objeto
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, apply_texture);

    //Ativa a visualizacao de texturas 2D (Texturizacao 2D)
    glEnable(GL_TEXTURE_2D);

    // Ativa o modelo de sombreamento de "Gouraud" (Smooth
    glShadeModel(GL_SMOOTH);
    // Ativa o z-buffering, de modo a remover as superficies escondidas
    glEnable(GL_DEPTH_TEST);


    // Inicia a iluminacao
    GLfloat light_position[] = { -1.0, 1.0, 1.0, 0.0 };
    GLfloat light_color[] = { 1.0, 1.0, 1.0, 0.0 };
    glLightfv(GL_LIGHT0, GL_AMBIENT_AND_DIFFUSE, light_color);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glEnable(GL_LIGHTING); //Ativa o modelo de iluminacao
    glEnable(GL_LIGHT0); //Ativa a Luz 0. O OpenGL suporta pelo menos 8 pontos de luz

    // Inicia as caracteristicas gerais dos materiais
    GLfloat mat_ambient_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_ambient_diffuse);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL); //Um ou mais parametros do material rastreiam a cor atual do glColor.

    // Ativa o modelo de sombreamento de Gouraud.
    glShadeModel(GL_SMOOTH);

    // Ativa o z-buffering, de modo a remover as superficies escondidas
    glEnable(GL_DEPTH_TEST);

    // define a cor com a qual a tela sera apagada
    glClearColor(1.0, 1.0, 1.0, 1.0);
}

/*
 * Processa o reshape da janela
 */
void reshape(int w, int h) {
    // Muda para o modo GL_PROJECTION e reinicia a projecao
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Define a zona da janela onde se vai desenhar
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);

    // Define uma projecao perspectiva (3D)
    // ratio = (float)w / (float)h;
    gluPerspective(45.0, 1.0, 1.0, 1000.0); // Define uma perspectiva de visão (ângulo de visão, proporção, plano próximo, plano distante)

    // muda para o modo GL_MODELVIEW para preparar o desenho
    glMatrixMode(GL_MODELVIEW);
}

/*
 * Funcao para controlar o display
 */
void display(void) {
    // Apaga o video e o depth buffer, e reinicia a matriz
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    board.ativar_camera();

    glColor3f(R, G, B); //Define a cor do objeto

    glPushMatrix();
    desenharJogo();
    glPopMatrix();
    // Troca os buffers, mostrando o que acabou de ser desenhado
    glutSwapBuffers();
}

/*
 * Funcao utilizada para a animacao com temporizador
 */
void timer(int value) {
    glutTimerFunc(1000 / fps_desejado, timer, 0);
    glutPostRedisplay(); // Manda redesenhar a tela em cada frame
}

void keyboardOnpress(unsigned char key, int x, int y) {
    board.evento_keyboard(key);
    glutPostRedisplay();
}

/*
 * Controle das teclas comuns
 */
void keyboard(unsigned char key, int x, int y) {
    keyStates[key] = true;
    keyboardOnpress(key, x, y);
}

void keyboardUp(unsigned char key, int x, int y) {
    keyStates[key] = false;
}

void desenharJogo() {
    board.desenhar_cenario();
    if (board.get_status_jogo() == 2 && !is_janela_2_ativada) {
        iniciar_nova_janela();
        is_janela_2_ativada = true;
    }
}

void play_som() {
    tocar toca;
    toca.tocar_som_jogo();
}

void iniciar_jogo() {
    Board board_iniciado((CGQuadrado(chao_coord_y)));
    Camera camera(vector3d(30, 10, 270), vector3d(0, 0.5, 0), vector3d(0, 1.0, 0));
    CGQuadrado cgrQuadrado(chao_coord_y);
    Boneco boneco(vector3d(25, chao_coord_y, 260));

    Balao balao_1((vector3d(260, chao_coord_y, 40)));
    Balao balao_2((vector3d(25, chao_coord_y, 30)));
    Balao balao_3((vector3d(260, chao_coord_y, 260)));

    board_iniciado.add_balao(balao_1);
    board_iniciado.add_balao(balao_2);
    board_iniciado.add_balao(balao_3);
    board_iniciado.add_boneco(boneco);
    board_iniciado.add_camera(camera);

    board = board_iniciado;
    camera_primeira_pessoa = false;
}


void mouse_click_callback_janela_2(int button, int state, int x, int y) {
    if (state == GLUT_DOWN) {
        if (button == GLUT_LEFT_BUTTON) {
            y = 400 - y;
            if (x >= 120 && x <= 280 && y <= (400 - 300) && y >= (400 - 350)) {
                fechar_segunda_janela();
            }
        }
    }
}

void display_janela_2(void) {
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glPointSize(1);
    glLineWidth(1);

    glColor3f(0.5, 0.6, 1.0); // seleciona a cor preta para o texto
    std::string mensagem = "Fim de Jogo";
    if (board.get_motivo_morte() == 1) {
        mensagem = "Morreu atingido por bomba ... :(";
    }

    if (board.get_motivo_morte() == 2) {
        mensagem = "Morreu pego por um fantasma ... :(";
    }
    draw_text_stroke(100, 400 - 200, "Fim de Jogo", 0.2, 2.0);
    draw_text_stroke(100, 400 - 250, mensagem, 0.1);
    draw_text_stroke(100, 400 - 280, "Quantidade de pontos: " + to_string(board.get_quantidade_pontos()), 0.1);


    glColor3f(0, 0, 0);
    glBegin(GL_LINE_LOOP);
    glVertex2f(120, 400 - 300);
    glVertex2f(280, 400 - 300);
    glVertex2f(280, 400 - 350);
    glVertex2f(120, 400 - 350);
    glEnd();

    draw_text_stroke(150, 400 - 320, "Fechar Jogo", 0.1);

    glutSwapBuffers();

}

void reshape_janela_2(int w, int h) {
    int larguraJanela = 400;
    int alturaJanela = 400;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, larguraJanela, alturaJanela);


    glOrtho(0, larguraJanela, 0, alturaJanela, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void iniciar_nova_janela() {
    // Inicialize a nova janela
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(400, 400);
    glutInitWindowPosition(100, 100);
    id_segunda_janela = glutCreateWindow("Mensagem do Jogo");

    glutDisplayFunc(display_janela_2);
    glutReshapeFunc(reshape_janela_2);
    glutMouseFunc(mouse_click_callback_janela_2);
    glClearColor(1.0, 1.0, 1.0, 1.0);
}


void fechar_segunda_janela() {
    glutDestroyWindow(id_segunda_janela);
}
