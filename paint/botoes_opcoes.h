#ifndef opcoes_botoes_h
#define opcoes_botoes_h
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
#include "rasterizacao_circulo.h"
#include "glut_text.h"
#include <vector>

void desenharMenuOpcoes(int alturaJanela) {

    //Triangulo
    glColor3f(0, 0, 0);
    glBegin(GL_LINE_LOOP);
    glVertex2f(20, alturaJanela - 40);
    glVertex2f(40, alturaJanela - 40);
    glVertex2f(30, alturaJanela - 20);
    glEnd();

    //Linha
    glPointSize(5);
    glBegin(GL_LINES);
    glVertex2f(60, alturaJanela - 20);
    glVertex2f(80, alturaJanela - 40);
    glEnd();

    //Poligano
    glBegin(GL_LINE_LOOP);
    glVertex2f(110, alturaJanela - 30);
    glVertex2f(130, alturaJanela - 20);
    glVertex2f(140, alturaJanela - 30);
    glVertex2f(140, alturaJanela - 40);
    glVertex2f(110, alturaJanela - 30);
    glEnd();

    //Quadrado
    glBegin(GL_LINE_LOOP);
    glVertex2f(160, alturaJanela - 20);
    glVertex2f(190, alturaJanela - 20);
    glVertex2f(190, alturaJanela - 40);
    glVertex2f(160, alturaJanela - 40);
    glEnd();

    desenharCirculo(220, alturaJanela - 30, 230, alturaJanela - 30);
}

void desenharOpcoesEixoEPintura(int alturaJanela) {
    glColor3f(0, 0, 0);
    glBegin(GL_LINE_LOOP);
    glVertex2f(270, alturaJanela - 10);
    glVertex2f(340, alturaJanela - 10);
    glVertex2f(340, alturaJanela - 90);
    glVertex2f(270, alturaJanela - 90);
    glEnd();

    //1° linha vertical
    glBegin(GL_LINES);
    glVertex2f(270, alturaJanela - 35);
    glVertex2f(340, alturaJanela - 35);
    glEnd();

    //2° linha vertical
    glBegin(GL_LINES);
    glVertex2f(270, alturaJanela - 60);
    glVertex2f(340, alturaJanela - 60);
    glEnd();

    draw_text_stroke(280, alturaJanela - 30, "Eixo-X", 0.1);
    draw_text_stroke(280, alturaJanela - 50, "Eixo-Y", 0.1);
    draw_text_stroke(280, alturaJanela - 80, "Pintar", 0.1);
}

void desenharQuadroOpcoes(int alturaJanela) {
    //Quadrado de opções de formas e transformações
    glColor3f(0, 0, 0);
    glBegin(GL_LINE_LOOP);
    glVertex2f(10, alturaJanela - 10);
    glVertex2f(260, alturaJanela - 10);
    glVertex2f(260, alturaJanela - 90);
    glVertex2f(10, alturaJanela - 90);
    glEnd();

    //Linha do horizontal
    glBegin(GL_LINES);
    glVertex2f(10, alturaJanela - 50);
    glVertex2f(260, alturaJanela - 50);
    glEnd();

    //1° linha vertical
    glBegin(GL_LINES);
    glVertex2f(50, alturaJanela - 10);
    glVertex2f(50, alturaJanela - 90);
    glEnd();

    //2° linha vertical
    glBegin(GL_LINES);
    glVertex2f(100, alturaJanela - 10);
    glVertex2f(100, alturaJanela - 90);
    glEnd();

    //3° linha vertical
    glBegin(GL_LINES);
    glVertex2f(150, alturaJanela - 10);
    glVertex2f(150, alturaJanela - 90);
    glEnd();

    //4° linha vertical
    glBegin(GL_LINES);
    glVertex2f(200, alturaJanela - 10);
    glVertex2f(200, alturaJanela - 90);
    glEnd();

}


#endif