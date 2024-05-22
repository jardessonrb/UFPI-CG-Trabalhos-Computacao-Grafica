/*
* Computacao Grafica
* Codigo Exemplo: Biblioteca com fucao para desenhar circulo em OpenGL
* Autor: Prof. Laurindo de Sousa Britto Neto
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

#ifndef circulo_h
#define circulo_h

#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265f
#endif

void desenhaCirculo(float raio, int num_linhas, bool preenchido)
{
    int i;
    GLfloat angulo;

    angulo = (2 * M_PI) / num_linhas; //divide 360 graus em radianos pela quantidade de linhas que serao desenhadas

    if (preenchido) glBegin(GL_TRIANGLE_FAN);   // Primitiva que liga os vertices, gerando triangulos com o primeiro vertice em comum
    else glBegin(GL_LINE_LOOP);     // Primitiva que liga os vertices, gerando segmentos de reta em um loop

    for (i = 1; i <= num_linhas; i++) // FOR usado para o calculo de cada ponto pertencente aos extremos das retas
    {
        // comando que calcula as coord. da rotacao e desenha as retas na tela
        glVertex2f(-raio * sin(i * angulo), raio * cos(i * angulo));	// centro = (0,0), x = 0 e y = raio;
        //		glVertex2f(raio*cos(i * angulo) , raio*sin(i * angulo));	// centro = (0,0), x = raio e y = 0;
    }
    glEnd();
}
#endif /* circulo_h */
