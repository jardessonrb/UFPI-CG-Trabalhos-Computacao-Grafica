// Bibliotecas utilizadas pelo OpenGL

/*
Aluno: Járdesson Ribeiro
Atividade: Braço Robô em 3D
Formas de usar:
c(minúsculo) rotaciona o antebraço sobre seu eixo 'z' - sentido horário
C(maiúsculo) rotaciona o antebraço sobre seu eixo 'z' - sentido anti-horário
y(minúsculo) rotaciona o braço sobre seu eixo 'y' - sentido horário
Y(maiúsculo) rotaciona o braço sobre seu eixo 'y' - sentido anti-horário
m(minúsculo) rotaciona a mão sobre seu eixo 'y' - sentido horário
M(maiúsculo) rotaciona a mão sobre seu eixo 'y' - sentido anti-horário
a(minúsculo) rotaciona o dedo anelar sobre seu eixo 'z' - sentido horário semelhante ao fechamento do dedo
A(maiúsculo) rotaciona o dedo anelar sobre seu eixo 'z' - sentido anti-horário semelhante a abertura da mão
i(minúsculo) rotaciona o dedo indicador sobre seu eixo 'z' - sentido horário semelhante ao fechamento do dedo
I(maiúsculo) rotaciona o dedo indicador sobre seu eixo 'z' - sentido anti-horário semelhante a abertura da mão
p(minúsculo) rotaciona o dedo polegar sobre seu eixo 'z' - sentido anti-horário semelhante ao fechamento do dedo
P(maiúsculo) rotaciona o dedo polegar sobre seu eixo 'z' - sentido horário semelhante a abertura da mão
f(minúsculo) fecha toda a mão
F(maiúsculo) abre toda a mão

ESC - para sair


*/
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

#include <stdio.h>
#include <stdlib.h>

#define ESC 27

static int  antebraco = 0, dedo_anelar = 90, dedo_indicador = 90, dedo_polegar = 90, mao = 0;
static int braco = 0, ombro = 0;


void init(void);
void keyboard(unsigned char key, int x, int y);
void display(void);
void reshape(int w, int h);
float* normalizarCor(float r, float g, float b);

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Jardesson - Braco Robo");

    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return EXIT_SUCCESS;
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Limpa o Buffer de Cores
    glLoadIdentity();

    //Base
    glPushMatrix();
    glTranslatef(0.0, -2.0, 0.0);
    glRotatef(-90, 1, 0, 0);
    glColor3fv(normalizarCor(96.0f, 96.0f, 96.0f));
    glutWireCone(1.0, 1.0, 30, 30); // base, altura, fatias, pilhas
    glPopMatrix();

    //Ombro
    glPushMatrix();
    glTranslatef(0.0, -1.0, 0.0);
    glColor3fv(normalizarCor(96.0f, 96.0f, 96.0f));
    glutWireSphere(0.3, 50, 30); // raio, fatias, pilhas
    glPopMatrix();

    //Movimento do braco
    glPushMatrix();
    glTranslatef(0.0, -1.0, 0.0);
    glRotatef((GLfloat)braco, 0.0, 1.0, 0.0);
    glTranslatef(0.0, 1.0, 0.0);
    // Braco
    glPushMatrix();
    glRotatef(90, 0, 0, 1);
    glScalef(2.0, 0.4, 0.4);
    glColor3fv(normalizarCor(127.0f, 0.0f, 255.0f));
    glutSolidCube(1.0); // Tamanho
    glPopMatrix();

    //Cotovelo
    glPushMatrix();
    glTranslatef(0.0, 1.0, 0.0);
    glColor3fv(normalizarCor(96.0f, 96.0f, 96.0f));
    glutWireSphere(0.3, 50, 30); // raio, fatias, pilhas
    glPopMatrix();

    //Movimento do antebraco
    glPushMatrix();
    //origem posicionada no cotovelo
    glTranslatef(0.0, 1.0, 0.0);
    glRotatef((GLfloat)antebraco, 0.0, 0.0, 1.0);
    glTranslatef(0.0, 1.0, 0.0);
    // Antebraco
    glPushMatrix();
    glRotatef(90, 0, 0, 1);
    glScalef(2.0, 0.4, 0.4);
    glColor3fv(normalizarCor(255.0f, 128.0f, 0.0f));
    glutSolidCube(1.0); // Tamanho
    glPopMatrix();

    //Mão
    glTranslatef(-0.01, 0.4, 0.0);
    glPushMatrix();
    glTranslatef(0.0, 0.0, 0.0);
    glRotatef((GLfloat)mao, 0.0, 0.10, 0.0);
    glTranslatef(0.0, 0.7, 0.0);
    // Dedo Anelar
    glPushMatrix();
    glTranslatef(0.0, 0.0, 0.0);
    glRotatef((GLfloat)dedo_anelar, 0.0, 0.0, 0.8);
    glTranslatef(0.2, 0.25, 0.10);
    glScalef(0.5, 0.1, 0.10);
    glColor3fv(normalizarCor(0.0f, 204.0f, 204.0f));
    glutSolidCube(1.0);
    glPopMatrix();

    // Dedo Indicador
    glPushMatrix();
    glTranslatef(0.0, 0.0, 0.0);
    glRotatef((GLfloat)dedo_indicador, 0.0, 0.0, 0.8);
    glTranslatef(0.2, 0.25, -0.10);
    glScalef(0.5, 0.1, 0.10);
    glColor3fv(normalizarCor(1.0f, 0.0f, 0.0f));
    glutSolidCube(1.0);
    glPopMatrix();

    // Dedo Polegar
    glPushMatrix();
    glTranslatef(0.0, 0.0, 0.0);
    glRotatef((GLfloat)dedo_polegar, 0.0, 0.0, 0.8);
    glTranslatef(0.2, -0.2, 0.0);
    glScalef(0.5, 0.1, 0.10);
    glColor3fv(normalizarCor(0.0f, 102.0f, 0.0f));
    glutSolidCube(1.0);
    glPopMatrix();


    glColor3fv(normalizarCor(51.0f, 51.0f, 255.0f));
    glutWireSphere(0.2, 30, 30); // raio, fatias, pilhas
    glPopMatrix();
    glPopMatrix();
    // origem volta para o sistema de coordenadas original
    glPopMatrix();
    // Troca os buffers, mostrando o que acabou de ser desenhado
    glutSwapBuffers();
}

void init(void) {
    glClearColor(1.0, 1.0, 1.0, 1.0); //Limpa a tela com a cor branca;
    glEnable(GL_DEPTH_TEST); // Habilita o algoritmo Z-Buffer
}

void keyboard(unsigned char key, int x, int y) {
    int temp;
    int temp_anelar;
    int temp_indicador;
    int temp_polegar;

    switch (key) {
    case 'c':
        antebraco = (antebraco - 5) % 360;
        break; // sentido horario
    case 'C':
        antebraco = (antebraco + 5) % 360;
        break; // sentido anti-horario
    case 'a':
        temp = (dedo_anelar - 5);
        if (temp >= 60) {
            dedo_anelar = temp % 360;
        }
        break; // sentido horario
    case 'A':
        temp = (dedo_anelar + 5);
        if (temp <= 120) {
            dedo_anelar = temp % 360;
        }
        break; // sentido anti-horario
    case 'i':
        temp = (dedo_indicador - 5);
        if (temp >= 60) {
            dedo_indicador = temp % 360;
        }
        break; // sentido horario
    case 'I':
        temp = (dedo_indicador + 5);
        if (temp <= 120) {
            dedo_indicador = temp % 360;
        }
        break; // sentido anti-horario
    case 'p':
        temp = (dedo_polegar + 5);
        if (temp <= 120) {
            dedo_polegar = temp % 360;
        }
        break; // sentido horario
    case 'P':
        temp = (dedo_polegar - 5);
        if (temp >= 60) {
            dedo_polegar = temp % 360;
        }
        break; // sentido anti-horario
    case 'm':
        mao = (mao - 5) % 360;
        break; // sentido horario
    case 'M':
        mao = (mao + 5) % 360;
        break; // sentido anti-horario
    case 'y':
        braco = (braco - 5) % 360;
        break; // sentido horario
    case 'Y':
        braco = (braco + 5) % 360;
        break; // sentido anti-horario
    case 'f':
        temp_anelar = (dedo_anelar - 5);
        temp_indicador = (dedo_indicador - 5);
        temp_polegar = (dedo_polegar + 5);

        if (temp_anelar >= 60) {
            dedo_anelar = temp_anelar % 360;
        }

        if (temp_indicador >= 60) {
            dedo_indicador = temp_indicador % 360;
        }

        if (temp_polegar <= 120) {
            dedo_polegar = temp_polegar % 360;
        }
        break;
    case 'F':
        temp_anelar = (dedo_anelar + 5);
        temp_indicador = (dedo_indicador + 5);
        temp_polegar = (dedo_polegar - 5);

        if (temp_anelar <= 120) {
            dedo_anelar = temp_anelar % 360;
        }

        if (temp_indicador <= 120) {
            dedo_indicador = temp_indicador % 360;
        }

        if (temp_polegar >= 60) {
            dedo_polegar = temp_polegar % 360;
        }
        break;
    case ESC: exit(EXIT_SUCCESS); break; // sai do programa

    }
    glutPostRedisplay();
}

void reshape(int w, int h) {
    // muda para o modo GL_PROJECTION e reinicia a matriz de projecao
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // define o tamanho da area de desenho da janela
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);

    // Define a forma do volume de visualizacao para termos
    // uma projecao perspectiva (3D).
    gluPerspective(60, (float)w / (float)h, 1.0, 9.0); //(angulo, aspecto, ponto_proximo, ponto distante)
    gluLookAt(0.0, 0.0, 7.0,  // posicao da camera (olho)
        0.0, 1.0, 0.0,  // direcao da camera (geralmente para centro da cena)
        0.0, 1.0, 0.0); // sentido ou orientacao da camera (de cabeca para cima)
    // muda para o modo GL_MODELVIEW para desenhar na tela
    glMatrixMode(GL_MODELVIEW);
}

float* normalizarCor(float r, float g, float b)
{
    static float rgb[3];
    rgb[0] = r / 255.0f;
    rgb[1] = g / 255.0f;
    rgb[2] = b / 255.0f;

    return rgb;
}
