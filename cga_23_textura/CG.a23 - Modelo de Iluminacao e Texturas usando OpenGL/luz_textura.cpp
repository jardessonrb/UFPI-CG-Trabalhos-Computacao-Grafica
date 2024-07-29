/*
 * Computacao Grafica
 * Codigo Exemplo: Usando texturas no OpenGL
 * Autor: Prof. Laurindo de Sousa Britto Neto
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

/* Inclui o arquivo header com a textura */
#include "textureRGB.h"
#include "glut_text.h"


/*
 * Declaracao de constantes e variaveis
 */
#define ESC 27 // ASCII para a tecla ESC
#define MAX_FPS 70 // Maximo de Frames Por Segundo (FPS) desejado
#define FPS 60 // FPS desejado atualmente

int fps_desejado = FPS / 2; // variavel para alterar os frames por segundo desejado
int fps = 0; //contador de frames por segundo

float angulo = 0.0; // angulo de rotacao
int graus = 5; // incremento do angulo de rotacao

float ratio = 1.0; // Manter proporcional a projecao

float R = 0.0, G = 0.0, B = 1.0; //Cores do desenho controladas pelo glColor (inicial: cor azul)

bool use_texture = true; // Determina se vamos usar texturas

bool use_color_material = false; // Determina se vamos usar a glColor como parametro do material

bool use_depth_test = true; // Determina se vamos usar Z-buffering

bool use_light = true; // Determina se liga a luz

GLint apply_texture = GL_ADD; //Determina como a textura sera aplicada no objeto
std::string apply_texture_name = "GL_ADD"; //nome da fucao que sera usada para aplicar a textura no objeto

#define QUANT_TEX 1 //Quantidade de texturas que serao utilizadas
unsigned int id_texturas[QUANT_TEX]; //Matriz para armazenar os identificadores de textura

// Vertices do objeto
#define N_VERTICES 4
const GLfloat vertex_coords[N_VERTICES][3] = {
    { -0.5, -0.272,  0.289 },
    {  0.5, -0.272,  0.289 },
    {  0.0, -0.272, -0.577 },
    {  0.0,  0.544,  0.000 }
};

// Faces do objeto (triangulos)
#define N_FACES 4
const GLuint faces[N_FACES][3] = {
    {3, 0, 1},
    {3, 1, 2},
    {3, 0, 2},
    {0, 2, 1}
};


// Vetores normais dos vertices do objeto
const GLfloat vertex_normals[N_VERTICES][3] = {
    { -0.81670904, -0.33290246,  0.47134089 },
    {  0.81670904, -0.33290252,  0.47134092 },
    {  0.00000000, -0.33331525, -0.94281548 },
    {  0.00000000,  1.00000000, -0.00019993 }
};

// Coordenadas de textura dos vertices do objeto
const GLfloat tex_coords[N_FACES][3][2] = {
    {{0.5, 0.000}, {0.0, 0.875}, {1.0, 0.875}},
    {{0.5, 0.000}, {0.0, 0.875}, {1.0, 0.875}},
    {{0.0, 0.875}, {1.0, 0.875}, {0.5, 0.000}},
    {{0.5, 0.000}, {1.0, 0.875}, {0.0, 0.875}}
};

/*
 *  Definicao dos parametros do modelo de iluminacao
 */
GLfloat light_pos[] = { -2.0, 2.0, 2.0, 0.0 };
GLfloat light_Ka[] = { 0.4, 0.4, 0.4, 0.0 };
GLfloat light_Kd[] = { 1.0, 1.0, 1.0, 0.0 };
GLfloat light_Ks[] = { 1.0, 1.0, 1.0, 0.0 };

/*
 *  Definicao dos parametros do material para o modelo de iluminacao
 *  Parametros para material amarelado, cor de latao (Brass)
 */
GLfloat material_Ka[] = { 0.33, 0.22, 0.03, 1.00 };
GLfloat material_Kd[] = { 0.78, 0.57, 0.11, 1.00 };
GLfloat material_Ks[] = { 0.99, 0.94, 0.81, 1.00 };
GLfloat material_Ke[] = { 0.00, 0.00, 0.00, 0.00 };
GLfloat material_Se = 28;


/*
 * Declaracoes antecipadas (forward) das funcoes (assinaturas)
 */
void init_glut(const char* window_name, int argc, char** argv);
void draw_object(void);
void display(void);
void reshape(int w, int h);
void computeFPS();
void timer(int value);
void keyboard(unsigned char key, int x, int y);
void keyboard_special(int key, int x, int y);
void menu(int value);

/*
 * Funcao principal
 */
int main(int argc, char** argv) {

    // inicia o GLUT e alguns parametros do OpenGL
    init_glut("Computacao Grafica: Modelo de Iluminacao e Textura", argc, argv);

    // funcao de controle do GLUT
    glutMainLoop();

    return EXIT_SUCCESS;
}

/*
 * Funcao para inicializacao do GLUT e de alguns parametros do OpenGL
 */
void init_glut(const char* nome_janela, int argc, char** argv) {
    // inicia o GLUT
    glutInit(&argc, argv);

    // Inicia o display usando RGB, double-buffering e Z-buffering
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(640, 480);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(nome_janela);

    // Define as funcoes de callback
    glutKeyboardFunc(keyboard);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutSpecialFunc(keyboard_special);
    glutTimerFunc(1000 / fps_desejado, timer, 0); //(mseg, timer, value)

    // Define o menu
    glutCreateMenu(menu);
    glutAddMenuEntry("Desenhar a azul", 1);
    glutAddMenuEntry("Desenhar a verde", 2);
    glutAddMenuEntry("Desenhar a vermelho", 3);
    glutAddMenuEntry("Sair", 0);
    glutAttachMenu(GLUT_RIGHT_BUTTON);


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
    // Define a cor de fundo (branco)
    glClearColor(1.0, 1.0, 1.0, 1.0);
}

/*
 * Processa o reshape da janela
 */
void reshape(int w, int h) {
    // muda para o modo GL_PROJECTION e reinicia a projecao
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // define a zona da janela onde se vai desenhar
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);

    //Define a projecao perspectiva
    ratio = (float)w / (float)h;
    gluPerspective(30, (float)w / (float)h, 1.0, 100.0);

    // Translada a camera no eixo Z se distanciando do objeto
    gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    // muda para o modo GL_MODELVIEW para preparar o desenho
    glMatrixMode(GL_MODELVIEW);
}


/*
 * Desenha o objeto, triangulo por triangulo, definindo os vetores
 * normais de cada vertice (ideal para usar com Gouraud shading) e
 * as coordenadas de textura de cada vertice
 */
void draw_object(void) {
    GLuint i, j;

    /* Desenha todos os triangulos do objeto */
    glBegin(GL_TRIANGLES);
    for (i = 0; i < N_FACES; i++) {
        for (j = 0; j < 3; j++) {
            /* Define o vertice j da face i */
            glTexCoord2fv(tex_coords[i][j]);
            glNormal3fv(vertex_normals[faces[i][j]]);
            glVertex3fv(vertex_coords[faces[i][j]]);
        }
    }
    glEnd();
}


/*
 * Funcao para controlar o display
 */
void display(void) {
    // Computa a taxa  de desenho de frames por segundo
    computeFPS();

    // Apaga o video e o depth buffer, e reinicia a matriz
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Ativa a Texturizacao 2D (Visualizacao de Texturas 2D)
    if (use_texture) glEnable(GL_TEXTURE_2D);
    else glDisable(GL_TEXTURE_2D);

    //Define a cor do objeto
    glColor3f(R, G, B);
    // Faz variar o angulo de rotacao entre 0 e 359 graus, e roda
    // Roda o objeto segundo o eixo x e y.
    angulo += graus;
    if (angulo >= 359.0) angulo -= 359.0;
    glPushMatrix();
    glRotatef(angulo, 1.0, 1.0, 0.0);
    draw_object(); // Desenha o objeto
    glPopMatrix();

    // desenhando textos na janela GLUT
    glDisable(GL_TEXTURE_2D); //Desativa a Texturizacao 2D para desenhar o texto
    glColor3f(0.0, 0.0, 0.0); // seleciona a cor preta para o texto
    std::string red = (R == 1.0) ? "On" : "Off";
    std::string gre = (G == 1.0) ? "On" : "Off";
    std::string blu = (B == 1.0) ? "On" : "Off";
    std::string luz = (use_light) ? "On" : "Off";
    std::string dep = (use_depth_test) ? "On" : "Off";
    std::string mat = (use_color_material) ? "On" : "Off";
    std::string tex = (use_texture) ? "On" : "Off";
    float x = -ratio - 0.45;
    draw_text_stroke(x, 1.25, "(ESC)EXIT (</>)FPS: " + to_string(fps) + "  (-/+)GRAUS: " + to_string(graus));
    draw_text_stroke(x, 1.15, "(F1)BLUE: " + blu + " (F2)GREEN: " + gre + " (F3)RED: " + red + " (F4)LIGHT: " + luz);
    draw_text_stroke(x, 1.05, "(F5)DEPTH: " + dep + " (F6)C.MAT: " + mat + " (F7)TEX: " + tex + " (F8)APPLY: " + apply_texture_name);

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


/*
 * Controle das teclas comuns
 */
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        // Tecle - ou + para alterar o incremento do angulo de rotacao
    case '-': case '_': if (graus > 1) graus -= 1; break;
    case '+': case '=': if (graus < 360) graus += 1; break;

        // Tecle < ou > para alterar o FPS desejado
    case ',': case '<': if (fps_desejado > 1) fps_desejado -= 1; break;
    case '.': case '>': if (fps_desejado * 2 < MAX_FPS) fps_desejado += 1; break;
    case ESC: exit(EXIT_SUCCESS); break;
    }
}


/*
 * Controle das teclas especiais (Cursores, F1 a F12, etc...)
 */
void keyboard_special(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_F1: R = 0.0; G = 0.0; B = 1.0; break;//Muda a cor para azul
    case GLUT_KEY_F2: R = 0.0; G = 1.0; B = 0.0; break;//Muda a cor para verde
    case GLUT_KEY_F3: R = 1.0; G = 0.0; B = 0.0; break;//Muda a cor para vermelho
    case GLUT_KEY_F4: //Liga/desliga a luz
        if (use_light) {
            use_light = false;
            glDisable(GL_LIGHTING);
        }
        else {
            use_light = true;
            glEnable(GL_LIGHTING);
        }
        break;
    case GLUT_KEY_F5: //Liga/desliga o Z-buffering
        if (use_depth_test) {
            use_depth_test = false;
            glDisable(GL_DEPTH_TEST);
        }
        else {
            use_depth_test = true;
            glEnable(GL_DEPTH_TEST);
        }
        break;
    case GLUT_KEY_F6: //Liga/desliga o color material
        if (use_color_material) {
            use_color_material = false;
            glDisable(GL_COLOR_MATERIAL);

            //Redefine os parametros do material para os valores iniciais, que estavam rastreando o glColor
            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, material_Ka);
            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material_Kd);
            glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material_Ks);
            glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, material_Ke);
            glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, material_Se);
        }
        else {
            use_color_material = true;
            glEnable(GL_COLOR_MATERIAL);
        }
        break;
    case GLUT_KEY_F7: //Liga/desliga a visualizacao da textura
        if (use_texture) use_texture = false;
        else use_texture = true;
        break;
    case GLUT_KEY_F8: //Troca como a textura sera aplicada no objeto
        switch (apply_texture) {
        case GL_ADD:
            apply_texture = GL_MODULATE;
            apply_texture_name = "GL_MODULATE";
            break;
        case GL_MODULATE:
            apply_texture = GL_BLEND;
            apply_texture_name = "GL_BLEND";
            break;
        case GL_BLEND:
            apply_texture = GL_DECAL;
            apply_texture_name = "GL_DECAL";
            break;
        case GL_DECAL:
            apply_texture = GL_ADD;
            apply_texture_name = "GL_ADD";
            break;
            //Funcoes usadas para combinar mais de uma texturas
//                case GL_COMBINE: //Uso sem combinar tem o mesmo efeito do GL_MODULATE
//                break;
//                case GL_REPLACE: //Uso sem combinar tem o mesmo efeito do GL_DECAL
//                break;
        }
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, apply_texture);
        break;
    }
}

/*
 * Controle do menu pop-up
 */
void menu(int value) {
    switch (value) {
    case GLUT_KEY_F1: R = 0.0; G = 0.0; B = 1.0; break;//Muda a cor para azul
    case GLUT_KEY_F2: R = 0.0; G = 1.0; B = 0.0; break;//Muda a cor para verde
    case GLUT_KEY_F3: R = 1.0; G = 0.0; B = 0.0; break;//Muda a cor para vermelho
    case 0: exit(EXIT_SUCCESS); break; //Sai do programa
    }
}


