/*
Aluno: Járdesson Ribeiro
Atividade: Castelo em 2D
*/

#ifdef __APPLE__ // MacOS
#define GL_SILENCE_DEPRECATION
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else // Windows e Linux
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#endif
#include <cstdlib>

#define ESC 27
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265f
#endif

//Variaveis Globais usadas para definicao de cores
float R, G, B;
float cores_torres[] = { 1.0f, 0.50f, 0.0f };
float cores_triangulos[] = { 0.88f, 0.19f, 0.19f };
float cores_circulos[] = { 0.58f, 0.23f, 0.88f };
float cor_amarela[] = { 1.0f, 0.93f, 0.023f };

// Declaracoes antecipadas (forward) de funcoes para as funcoes callback do OpenGL
void init(void);
void reshape(int w, int h);
void keyboard(unsigned char key, int x, int y);
void keyboard_special(int key, int x, int y);
void display(void);
void desenhaCirculo(float raio, int num_linhas, bool preenchido);

int main(int argc, char** argv)
{
    glutInit(&argc, argv); // Passagens de parametros C para o glut
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); // Selecao do Modo do Display e do Sistema de cor utilizado
    glutInitWindowSize(750, 750);  // Tamanho da janela do OpenGL
    glutInitWindowPosition(100, 100); //Posicao inicial da janela do OpenGL
    glutCreateWindow("Castelo 2D com OpenGL e Glut"); // Da nome para uma janela OpenGL
    init(); // Chama a funcao init();
    glutReshapeFunc(reshape); //funcao callback para redesenhar a tela
    glutDisplayFunc(display); //funcao callback de desenho
    // glutKeyboardFunc(keyboard); //funcao callback do teclado
    // glutSpecialFunc(keyboard_special);	//funcao callback do teclado especial
    glutMainLoop(); // executa o loop do OpenGL
    return EXIT_SUCCESS; // 
}

void init(void) {
    glClearColor(1.0, 1.0, 1.0, 1.0); //Limpa a tela com a cor branca;
    R = 0.0;
    G = 0.0;
    B = 0.0;
}

void reshape(int w, int h)
{
    // Reinicializa o sistema de coordenadas
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Definindo o Viewport para o tamanho da janela
    glViewport(0, 0, w, h);

    // glOrtho(0, 256, 0, 256, -1, 1);  // Define o volume de projecao ortografica;
    // Nesse caso, o ponto (0,0,0) se encontra no
    // canto inferior esquerdo da tela e o viewport
    // tem 0 a 512 pixel no eixo x, 0 a 512 pixel no
    // eixo y. Como sera trabalhado imagens 2D definimos
    // o eixo z com tamanho -1 a 1;

    glOrtho(0, w - 50, 0, h, -1, 1);

    //   glOrtho (-256, 256, -256, 256, -1 ,1); // Nesse caso, continuamos com 512 pixels, porem o
                                                // centro dos eixos passa a se localizar no centro
                                                // da tela.

        // glOrtho(-(w / 2), (w / 2), -(h / 2), (h / 2), -1, 1);   // Nesse caso, ficamos com a quantidade de pixels variavel,
        // dependente da largura e altura da janela, e o
        // centro dos eixos continua a se localizar no centro
        // da tela.

    // muda para o modo GL_MODELVIEW (nao pretendemos alterar a projecao
    // quando estivermos desenhando na tela)
    glMatrixMode(GL_MODELVIEW);
}

void display(void) {
    //Limpa o Buffer de Cores e reinicia a matriz
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    // gluOrtho2D(0, 28, 0, 22);

    // Transformacoes usadas no triangulo com vertice esquerdo na origem dos eixos cartesianos
    glColor3f(R, G, B);
    //	glScalef(0.5, 0.5, 0);
    //	glTranslatef(200,200, 0);
    //	glRotatef(180, 0, 0, 1);


    //Torre da esquerda
    glBegin(GL_QUADS);
    glColor3f(cores_torres[0], cores_torres[1], cores_torres[2]);
    glVertex2f(50, 0);
    glVertex2f(200, 0);
    glVertex2f(200, 400);
    glVertex2f(50, 400);

    glEnd();

    //Corpo do meio
    glBegin(GL_QUADS);
    glColor3f(cor_amarela[0], cor_amarela[1], cor_amarela[2]);
    glVertex2f(200, 0);
    glVertex2f(200, 250);
    glVertex2f(500, 250);
    glVertex2f(500, 0);
    glEnd();

    //Torre da direita
    glBegin(GL_QUADS);
    glColor3f(cores_torres[0], cores_torres[1], cores_torres[2]);
    glVertex2f(500, 0);
    glVertex2f(500, 400);
    glVertex2f(650, 400);
    glVertex2f(650, 0);
    glEnd();

    //Torre do meio
    glBegin(GL_QUADS);
    glColor3f(cores_torres[0], cores_torres[1], cores_torres[2]);
    glVertex2f(275, 250);
    glVertex2f(275, 450);
    glVertex2f(425, 450);
    glVertex2f(425, 250);
    glEnd();

    //Triangulo da esquerda
    glBegin(GL_TRIANGLES);
    glColor3f(cores_triangulos[0], cores_triangulos[1], cores_triangulos[2]);
    glVertex2f(20, 400);
    glVertex2f(230, 400);
    glVertex2f(125, 550);
    glEnd();

    //Triangulo do meio
    glBegin(GL_TRIANGLES);
    glColor3f(cores_triangulos[0], cores_triangulos[1], cores_triangulos[2]);
    glVertex2f(245, 450);
    glVertex2f(455, 450);
    glVertex2f(350, 650);
    glEnd();

    //Triangulo da direita
    glBegin(GL_TRIANGLES);
    glColor3f(cores_triangulos[0], cores_triangulos[1], cores_triangulos[2]);
    glVertex2f(470, 400);
    glVertex2f(680, 400);
    glVertex2f(575, 550);
    glEnd();

    //Bandeira torre da direita
    glBegin(GL_LINE_STRIP);
    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex2f(125, 550);
    glVertex2f(125, 650);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(0.0, 255.0, 0.0);
    glVertex2f(25, 600);
    glVertex2f(25, 650);
    glVertex2f(124, 650);
    glVertex2f(124, 600);
    glEnd();

    //Bandeira torre do meio
    glBegin(GL_LINE_STRIP);
    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex2f(350, 650);
    glVertex2f(350, 750);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(0.0, 255.0, 0.0);
    glVertex2f(250, 700);
    glVertex2f(250, 750);
    glVertex2f(349, 750);
    glVertex2f(349, 700);
    glEnd();

    //Bandeira torre da esquerda
    glBegin(GL_LINE_STRIP);
    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex2f(575, 550);
    glVertex2f(575, 650);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(0.0, 255.0, 0.0);
    glVertex2f(576, 600);
    glVertex2f(576, 650);
    glVertex2f(675, 650);
    glVertex2f(675, 600);
    glEnd();


    //A translação no x é exatamente o ponto do meio do triangulo
    //A translação do y é a altura do meio da torre
    //Circulo da torre da esquerda
    glTranslatef(125, 250, 0);
    glColor3f(cores_circulos[0], cores_circulos[1], cores_circulos[2]);
    desenhaCirculo(50, 30, true);
    glTranslatef(-125, -250, 0);

    //Circulo da torre do meio
    glTranslatef(575, 250, 0);
    glColor3f(cores_circulos[0], cores_circulos[1], cores_circulos[2]);
    desenhaCirculo(50, 30, true);
    glTranslatef(-575, -250, 0);

    //Circulo da torre da direita
    glTranslatef(350, 350, 0);
    glColor3f(cores_circulos[0], cores_circulos[1], cores_circulos[2]);
    desenhaCirculo(50, 30, true);
    glTranslatef(-350, -350, 0);

    //Decoração da torre esquerda
    glBegin(GL_QUADS);
    glColor3f(cor_amarela[0], cor_amarela[1], cor_amarela[2]);
    glVertex2f(60, 150);
    glVertex2f(60, 180);
    glVertex2f(125, 180);
    glVertex2f(125, 150);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(cor_amarela[0], cor_amarela[1], cor_amarela[2]);
    glVertex2f(125, 110);
    glVertex2f(125, 140);
    glVertex2f(190, 140);
    glVertex2f(190, 110);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(cor_amarela[0], cor_amarela[1], cor_amarela[2]);
    glVertex2f(60, 70);
    glVertex2f(60, 100);
    glVertex2f(125, 100);
    glVertex2f(125, 70);
    glEnd();

    //Decoração da torre direita
    glBegin(GL_QUADS);
    glColor3f(cor_amarela[0], cor_amarela[1], cor_amarela[2]);
    glVertex2f(575, 150);
    glVertex2f(575, 180);
    glVertex2f(640, 180);
    glVertex2f(640, 150);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(cor_amarela[0], cor_amarela[1], cor_amarela[2]);
    glVertex2f(510, 110);
    glVertex2f(510, 140);
    glVertex2f(575, 140);
    glVertex2f(575, 110);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(cor_amarela[0], cor_amarela[1], cor_amarela[2]);
    glVertex2f(575, 70);
    glVertex2f(575, 100);
    glVertex2f(640, 100);
    glVertex2f(640, 70);
    glEnd();

    //Portao
    glBegin(GL_QUADS);
    glColor3f(0.0, 0.0, 0.0);
    glVertex2f(275, 0);
    glVertex2f(275, 150);
    glVertex2f(425, 150);
    glVertex2f(425, 0);
    glEnd();

    //Circulo da torre da direita
    glTranslatef(350, 100, 0);
    glColor3f(cor_amarela[0], cor_amarela[1], cor_amarela[2]);
    desenhaCirculo(20, 40, false);
    glTranslatef(-350, -100, 0);

    //Circulo da torre da direita
    glTranslatef(350, 120, 0);
    glColor3f(cor_amarela[0], cor_amarela[1], cor_amarela[2]);
    desenhaCirculo(10, 40, true);
    glTranslatef(-350, -120, 0);


    glBegin(GL_QUADS);
    glColor3f(cores_torres[0], cores_torres[1], cores_torres[2]);
    glVertex2f(205, 30);
    glVertex2f(205, 60);
    glVertex2f(265, 60);
    glVertex2f(265, 30);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(cores_torres[0], cores_torres[1], cores_torres[2]);
    glVertex2f(435, 30);
    glVertex2f(435, 60);
    glVertex2f(495, 60);
    glVertex2f(495, 30);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(cores_torres[0], cores_torres[1], cores_torres[2]);
    glVertex2f(230, 180);
    glVertex2f(230, 210);
    glVertex2f(290, 210);
    glVertex2f(290, 180);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(cores_torres[0], cores_torres[1], cores_torres[2]);
    glVertex2f(410, 180);
    glVertex2f(410, 210);
    glVertex2f(470, 210);
    glVertex2f(470, 180);
    glEnd();

    // Funcao criada para desenhar circulos

    glFlush(); // manda o OpenGl renderizar as primitivas

}



void desenhaCirculo(float raio, int num_linhas, bool preenchido)
{
    int i;
    GLfloat angulo;

    angulo = (2 * M_PI) / num_linhas; //divide 360 graus em radianos pela quantidade de linhas que serao desenhadas

    if (preenchido) {
        glBegin(GL_TRIANGLE_FAN);   // Primitiva que liga os vertices, gerando triangulos com o primeiro vertice em comum
    }
    else {
        glBegin(GL_LINE_LOOP);// Primitiva que liga os vertices, gerando segmentos de reta em um loop
    }

    for (i = 1; i <= num_linhas; i++) // FOR usado para o calculo de cada ponto pertencente aos extremos das retas
    {
        //comando que calcula as coord. da rotacao e desenha as retas na tela
        glVertex2f(-raio * sin(i * angulo), raio * cos(i * angulo));	// centro = (0,0), x = 0 e y = raio;
        //glVertex2f(raio*cos(i * angulo) , raio*sin(i * angulo));	// centro = (0,0), x = raio e y = 0;
    }
    glEnd();
}