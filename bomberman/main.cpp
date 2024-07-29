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

/*
 * Declaracao de constantes e variaveis
 */

#define ESC 27 // ASCII para a tecla ESC
#define MAX_FPS 70 // Maximo de Frames Por Segundo (FPS) desejado
#define FPS 60 // FPS desejado atualmente
#define KEY_COUNT 256

#define QUANT_TEX 3 //Quantidade de texturas que serao utilizadas
unsigned int id_texturas[QUANT_TEX];
GLint apply_texture = GL_ADD;

const int chao_coord_y = -0.5f;
const int chao_tamanho_x = 600;
const int chao_tamanho_z = 600;
GLdouble cam_x1 = 0.0;
GLdouble cam_y1 = 500.0;
GLdouble cam_z1 = 850.0;


// // Vetores normais dos vertices do objeto
// const GLfloat vertex_normals[N_VERTICES][3] = {
//     { -0.81670904, -0.33290246,  0.47134089 },
//     {  0.81670904, -0.33290252,  0.47134092 },
//     {  0.00000000, -0.33331525, -0.94281548 },
//     {  0.00000000,  1.00000000, -0.00019993 }
// };

// // Coordenadas de textura dos vertices do objeto
// const GLfloat tex_coords[N_FACES][3][2] = {
//     {{0.5, 0.000}, {0.0, 0.875}, {1.0, 0.875}},
//     {{0.5, 0.000}, {0.0, 0.875}, {1.0, 0.875}},
//     {{0.0, 0.875}, {1.0, 0.875}, {0.5, 0.000}},
//     {{0.5, 0.000}, {1.0, 0.875}, {0.0, 0.875}}
// };

// /*
//  *  Definicao dos parametros do modelo de iluminacao
//  */
// GLfloat light_pos[] = { -2.0, 2.0, 2.0, 0.0 };
// GLfloat light_Ka[] = { 0.4, 0.4, 0.4, 0.0 };
// GLfloat light_Kd[] = { 1.0, 1.0, 1.0, 0.0 };
// GLfloat light_Ks[] = { 1.0, 1.0, 1.0, 0.0 };

// /*
//  *  Definicao dos parametros do material para o modelo de iluminacao
//  *  Parametros para material amarelado, cor de latao (Brass)
//  */
// GLfloat material_Ka[] = { 0.33, 0.22, 0.03, 1.00 };
// GLfloat material_Kd[] = { 0.78, 0.57, 0.11, 1.00 };
// GLfloat material_Ks[] = { 0.99, 0.94, 0.81, 1.00 };
// GLfloat material_Ke[] = { 0.00, 0.00, 0.00, 0.00 };
// GLfloat material_Se = 28;



// gluLookAt(0, 40, 100, // Posição da câmera (x, y, z)
//         0.0, 0.5, -300.0,    // Ponto para onde a câmera está olhando (x, y, z)
//         0.0, 1.0, 0.0); //UP da camera
Camera camera(vector3d(0, 20, 400), vector3d(0, 0.5, 0), vector3d(0, 1.0, 0));
Board board;
CGQuadrado cgrQuadrado(chao_coord_y);
bool camera_primeira_pessoa = false;


bool keyStates[KEY_COUNT];

const int LARGURA_WINDOW = 800;
const int COMPRIMENTO_WINDOW = 700;
static int ultima_posicao_mouse = 0;
static bool primeiro_click_mouse = true;
static bool btn_mouse_esquerdo_pressionado = false;

int fps_desejado = FPS / 2; // variavel para alterar os frames por segundo desejado
int fps = 0; //contador de frames por segundo

float angulo = 0.0; // angulo de rotacao
int graus = 5; // incremento do angulo de rotacao

float ratio = 1.0; // Manter proporcional a projecao

float R = 0.0, G = 0.0, B = 0.0; //Cores do desenho (inicial: cor azul)

bool use_gouraud = false; // Determina o uso de Gouraud ou Flat shading

bool use_depth_test = true; // Determina o uso de Z-buffering

bool use_light = true; // Determina se liga a luz
//Estrutura criada para facilitar o entendimento no calculo das normais
struct vertice {
    float x;
    float y;
    float z;
};

/*
 * Declaracoes antecipadas (forward) das funcoes (assinaturas)
 */
void init_glut(const char* window_name, int argc, char** argv);
void draw_object_smooth(void);
void display(void);
void reshape(int w, int h);
void computeFPS();
void timer(int value);
void keyboard(unsigned char key, int x, int y);
void keyboardUp(unsigned char key, int x, int y);
void keyboardOnpress(unsigned char key, int x, int y);
void keyboard_special(int key, int x, int y);

void mouse_click_callback(int button, int state, int x, int y);
void mouse_passive_callback(int x, int y);
void mouse_active_click__callback(int x, int y);

/*
 * Funcao principal
 */
int main(int argc, char** argv) {

    /* inicia o GLUT e alguns parametros do OpenGL */
    init_glut("Bomberman 3D", argc, argv);

    /* funcao de controle do GLUT */
    glutMainLoop();

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
    glutSpecialFunc(keyboard_special);
    glutKeyboardUpFunc(keyboardUp);
    glutMouseFunc(mouse_click_callback);
    glutPassiveMotionFunc(mouse_passive_callback);
    glutMotionFunc(mouse_active_click__callback);
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

    // Translada a camera no eixo Z se distanciando do objeto
    // gluLookAt(0, 40, 400, // Posição da câmera (x, y, z)
    //     0.0, 0.5, -300.0,    // Ponto para onde a câmera está olhando (x, y, z)
    //     0.0, 1.0, 0.0);   // Vetor "up" da câmera (x, y, z)

    // muda para o modo GL_MODELVIEW para preparar o desenho
    glMatrixMode(GL_MODELVIEW);
}

/*
 * Funcao para controlar o display
 */
void display(void) {
    float x = -ratio - 0.45;
    // Computa a taxa  de desenho de frames por segundo
    computeFPS();

    // Apaga o video e o depth buffer, e reinicia a matriz
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    if (camera_primeira_pessoa) {
        camera.ativar();
    }
    else {
        camera.ativarVisaoCima();
    }

    glColor3f(R, G, B); //Define a cor do objeto

    // Faz variar o angulo de rotacao entre 0 e 359 graus, e roda
    // Roda o objeto segundo o eixo x e y.
    // glRotatef(angulo, 0.0, 1.0, 0.0);
    // glEnable(GL_TEXTURE_2D);
    glColor3f(1.0, 1.0, 1.0); // seleciona a cor preta para o texto
    draw_text_stroke(0, 40, "Bomberman - FPS: " + to_string(fps));

    glPushMatrix();
    board.desenhar_cenario();
    // cgrQuadrado.desenhaQuadrado(0, 0, 20);
    cgrQuadrado.desenhaObstaculos(300, 300, 20);
    glPopMatrix();
    // Troca os buffers, mostrando o que acabou de ser desenhado
    glutSwapBuffers();
}

/*
 * Computa a quantidade de frames por segundo da animacao
 */
void computeFPS() {
    static GLuint frames = 0;        // frames 1000 milissegundos
    static GLuint clock;             // em milissegudos
    static GLuint next_clock = 0;    // em milissegudos

    frames++;
    clock = glutGet(GLUT_ELAPSED_TIME); //N�mero de milissegundos desde a chamada a glutInit()
    if (clock < next_clock) return;

    fps = frames; // guarda o n�mero de frames por segundo

    // Evita o reinicio da contagem dos frames na primeira iteracao
    if (next_clock != 0) frames = 0;//Reinicia a contagem dos frames a cada 1000 milissegundos
    next_clock = clock + 1000; // 1000 milissegundos = 1 segundo
}

/*
 * Funcao utilizada para a animacao com temporizador
 */
void timer(int value) {
    glutTimerFunc(1000 / fps_desejado, timer, 0);
    glutPostRedisplay(); // Manda redesenhar a tela em cada frame
}

void keyboardOnpress(unsigned char key, int x, int y) {
    if (keyStates['w']) {
        printf("pra cima");
        camera.frente();
    }
    if (keyStates['a']) {
        printf("pra esquerda");
        camera.esquerda();
    }
    if (keyStates['d']) {
        printf("pra direita");
        camera.direita();
    }
    if (keyStates['s']) {
        printf("pra baixo");
        camera.tras();
    }

    glutPostRedisplay();
}

/*
 * Controle das teclas comuns
 */
void keyboard(unsigned char key, int x, int y) {
    keyStates[key] = true;
    keyboardOnpress(key, x, y);

    switch (key) {
        // Tecle - ou + para alterar o incremento do angulo de rotacao
    case '-': case '_': if (graus > 1) graus -= 1; break;
    case '+': case '=': if (graus < 360) graus += 1; break;

        // Tecle < ou > para alterar o FPS desejado
    case ',': case '<': if (fps_desejado > 1) fps_desejado -= 1; break;
    case '.': case '>': if (fps_desejado * 2 < MAX_FPS) fps_desejado += 1; break;



    case 'c':
        camera_primeira_pessoa = !camera_primeira_pessoa;
        break;
    case ESC: exit(EXIT_SUCCESS); break;
    }
}


void keyboardUp(unsigned char key, int x, int y) {
    keyStates[key] = false;
}

/*
 * Controle das teclas especiais (Cursores, F1 a F12, etc...)
 */
void keyboard_special(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_F1: R = 0.0; G = 0.0; B = 1.0; break;//Muda a cor para azul
    case GLUT_KEY_F2: R = 0.0; G = 1.0; B = 0.0; break;//Muda a cor para verde
    case GLUT_KEY_F3: R = 1.0; G = 0.0; B = 0.0; break;//Muda a cor para vermelho
    case GLUT_KEY_F4: //F4: liga/desliga a luz
        if (use_light) {
            use_light = false;
            glDisable(GL_LIGHTING);
        }
        else {
            use_light = true;
            glEnable(GL_LIGHTING);
        }
        break;
    case GLUT_KEY_F5: //F5: liga/desliga o Z-buffering
        if (use_depth_test) {
            use_depth_test = false;
            glDisable(GL_DEPTH_TEST);
        }
        else {
            use_depth_test = true;
            glEnable(GL_DEPTH_TEST);
        }
        break;

    case GLUT_KEY_F6: //F6: liga/desliga o gouraud shading
        if (use_gouraud) {
            use_gouraud = false;
            glShadeModel(GL_FLAT);

        }
        else {
            use_gouraud = true;
            glShadeModel(GL_SMOOTH);
        }
        break;

    }
}
void mouse_click_callback(int button, int state, int x, int y) {

    if (state == GLUT_DOWN) {
        if (button == GLUT_LEFT_BUTTON) {
            btn_mouse_esquerdo_pressionado = true;
            ultima_posicao_mouse = x;
            printf("Botão esquerdo pressionado em (%d, %d)\n", x, y);
        }
    }
    else if (state == GLUT_UP) {
        btn_mouse_esquerdo_pressionado = false;
        float dx;

        if (primeiro_click_mouse) {
            dx = 0;
            ultima_posicao_mouse = x;
            primeiro_click_mouse = false;
        }

        dx = x - ultima_posicao_mouse;
        ultima_posicao_mouse = x;

        // camera.atualizarYaw(dx);
        // camera.atualizarYaw();
        printf("Botão liberado em (%d, %d)\n", x, y);
    }
}
void mouse_passive_callback(int x, int y) {
    // printf("Movimento passivo em (%d, %d)\n", x, y);
}
void mouse_active_click__callback(int x, int y) {
    // printf("Movimento com botão pressionado em (%d, %d)\n", x, y);
}
