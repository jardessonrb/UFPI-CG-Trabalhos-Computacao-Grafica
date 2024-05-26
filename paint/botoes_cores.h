#ifndef botoes_cores_h
#define botoes_cores_h

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

#include "glut_text.h"

void desenharQuadroDeCores(float corAtual[], int alturaJanela) {
    //Quadrado de cores
    glColor3f(0, 0, 0);
    glBegin(GL_LINE_LOOP);
    glVertex2f(350, alturaJanela - 10);
    glVertex2f(600, alturaJanela - 10);
    glVertex2f(600, alturaJanela - 90);
    glVertex2f(350, alturaJanela - 90);
    glEnd();

    //COR
    glColor3f(0.0f, 0.0f, 1.0f);
    glBegin(GL_QUADS);
    glVertex2f(350, alturaJanela - 10);
    glVertex2f(400, alturaJanela - 10);
    glVertex2f(400, alturaJanela - 50);
    glVertex2f(350, alturaJanela - 50);
    glEnd();

    //COR
    glColor3f(0.5f, 0.0f, 0.5f);
    glBegin(GL_QUADS);
    glVertex2f(400, alturaJanela - 10);
    glVertex2f(450, alturaJanela - 10);
    glVertex2f(450, alturaJanela - 50);
    glVertex2f(400, alturaJanela - 50);
    glEnd();

    //COR
    glColor3f(0.54f, 0.17f, 0.89f);
    glBegin(GL_QUADS);
    glVertex2f(450, alturaJanela - 10);
    glVertex2f(500, alturaJanela - 10);
    glVertex2f(500, alturaJanela - 50);
    glVertex2f(450, alturaJanela - 50);
    glEnd();

    glColor3f(1.0f, 1.0f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(500, alturaJanela - 10);
    glVertex2f(550, alturaJanela - 10);
    glVertex2f(550, alturaJanela - 50);
    glVertex2f(500, alturaJanela - 50);
    glEnd();

    glColor3f(0.54f, 0.17f, 0.89f);
    glBegin(GL_QUADS);
    glVertex2f(550, alturaJanela - 10);
    glVertex2f(600, alturaJanela - 10);
    glVertex2f(600, alturaJanela - 50);
    glVertex2f(550, alturaJanela - 50);
    glEnd();

    //Cores de baixo
    //COR
    glColor3f(1.0f, 0.5f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(350, alturaJanela - 50);
    glVertex2f(400, alturaJanela - 50);
    glVertex2f(400, alturaJanela - 90);
    glVertex2f(350, alturaJanela - 90);
    glEnd();

    //COR
    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(400, alturaJanela - 50);
    glVertex2f(450, alturaJanela - 50);
    glVertex2f(450, alturaJanela - 90);
    glVertex2f(400, alturaJanela - 90);
    glEnd();

    //COR
    glColor3f(1.0f, 0.0f, 1.0f);
    glBegin(GL_QUADS);
    glVertex2f(450, alturaJanela - 50);
    glVertex2f(500, alturaJanela - 50);
    glVertex2f(500, alturaJanela - 90);
    glVertex2f(450, alturaJanela - 90);
    glEnd();

    glColor3f(0.0f, 1.0f, 1.0f);
    glBegin(GL_QUADS);
    glVertex2f(500, alturaJanela - 50);
    glVertex2f(550, alturaJanela - 50);
    glVertex2f(550, alturaJanela - 90);
    glVertex2f(500, alturaJanela - 90);
    glEnd();

    glColor3f(0.0f, 0.5f, 0.5f);
    glBegin(GL_QUADS);
    glVertex2f(550, alturaJanela - 50);
    glVertex2f(600, alturaJanela - 50);
    glVertex2f(600, alturaJanela - 90);
    glVertex2f(550, alturaJanela - 90);
    glEnd();

    //Linha do Meio
    glBegin(GL_LINES);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex2f(350, alturaJanela - 50);
    glVertex2f(600, alturaJanela - 50);
    glEnd();

    glColor3f(0.0f, 0.0f, 0.0f);
    draw_text_stroke(620, alturaJanela - 30, "Cor Selecionada", 0.07);
    //Cor Atual selecionada
    glColor3f(corAtual[0], corAtual[1], corAtual[2]);
    glBegin(GL_QUADS);
    glVertex2f(630, alturaJanela - 50);
    glVertex2f(680, alturaJanela - 50);
    glVertex2f(680, alturaJanela - 90);
    glVertex2f(630, alturaJanela - 90);
    glEnd();
}



#endif