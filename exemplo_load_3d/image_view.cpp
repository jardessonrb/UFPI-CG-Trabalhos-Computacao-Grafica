/*
 * Computacao Grafica
 * Codigo Exemplo: Vizualizador de Imagens
 * Autor: Prof. Laurindo de Sousa Britto Neto
 */

 // Bibliotecas utilizadas pelo OpenGL
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

#include <cstdlib>
#include "textureRGB.h"

/*
 * Declaracao de constantes e variaveis
 */
#define ESC 27 // ASCII para a tecla ESC
int zoom = 7;

/*
 * Declaracoes antecipadas (forward) das funcoes (assinaturas das funcoes)
 */
void init(void);
void reshape(int w, int h);
void keyboard(unsigned char key, int x, int y);
void keyboard_special(int key, int x, int y);
void display(void);

/*
 * Funcao principal
 */
int main(int argc, char** argv) {
    glutInit(&argc, argv); // Passagens de parametro C para o glut
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); // Selecao do Modo do Display e do Sistema de cor utilizado
    glutInitWindowSize(512, 512);  // Tamanho da janela do OpenGL
    glutInitWindowPosition(100, 100); //Posicao inicial da janela do OpenGL
    glutCreateWindow("Computacao Grafica: Visualizador de Imagens"); //Da nome para uma janela OpenGL
    init(); // Chama funcao init();
    glutReshapeFunc(reshape); //funcao callback para redesenhar a tela
    glutDisplayFunc(display); //funcao callback de desenho
    glutKeyboardFunc(keyboard); //funcao callback do teclado
    glutMainLoop(); // executa o loop do OpenGL
    return EXIT_SUCCESS; // Retorna 0 para o tipo inteiro da funcao main()
}


/*
 * Inicializa alguns parametros do GLUT
 */
void init(void) {
    glClearColor(1.0, 1.0, 1.0, 1.0); //Limpa a tela com a cor branca;
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
}

/*
 * Ajusta a projecao para o redesenho da janela
 */
void reshape(int w, int h)
{
    // Reinicializa o sistema de coordenadas
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Definindo o Viewport para o tamanho da janela
    glViewport(0, 0, w, h);

    glOrtho(0, 512, 0, 512, -1, 1);  // Define o volume de projecao ortografica;
    // Nesse caso, o ponto (0,0,0) se encontra no
    // canto inferior esquerdo da tela e o viewport
    // tem 0 a 512 pixel no eixo x, 0 a 512 pixel no
    // eixo y. Como sera trabalhado imagens 2D definimos
    // o eixo z com tamanho -1 a 1;

// muda para o modo GL_MODELVIEW (nao pretendemos alterar a projecao
// quando estivermos desenhando na tela)
    glMatrixMode(GL_MODELVIEW);
}

/*
 * Controle das teclas comuns do teclado
 */
void keyboard(unsigned char key, int x, int y) {
    switch (key) { // key - variavel que possui valor ASCII da tecla precionada
    case ESC: exit(EXIT_SUCCESS); break;
    case '+': case '=': zoom += 1; break;
    case '-': case '_': if (zoom > 0) zoom -= 1; break;
    }
    glutPostRedisplay();
}

/*
 * Funcao usada na funcao callback para desenhar na tela
 */
void display(void) {

    glClear(GL_COLOR_BUFFER_BIT); //Limpa o Buffer de Cores
    glLoadIdentity();

    glRasterPos2d(0, 0);
    glPixelZoom(zoom, zoom);
    glDrawPixels(TEXTURE_WIDTH, TEXTURE_HEIGHT, //largura e altura
        GL_RGB, GL_UNSIGNED_BYTE, // formato dos dados
        texture_data_3); //dados da imagem

    glutSwapBuffers(); // manda o OpenGl renderizar as primitivas
}

