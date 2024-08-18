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

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <filesystem>
#include "Loader.cpp"


void desenhar();

Loader loader;

// Inicialização do OpenGL
void init() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
}

// Função de renderização
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushMatrix();
    glColor3f(0.5f, 0.5f, 0.5f);

    desenhar();

    glPopMatrix();
    glutSwapBuffers();
}

// Função de reshape
void reshape(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)width / height, 1.0, 200.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0, 0.0, 5.0,   // Posição da câmera
        -0.5, 0.0, 0.0,   // Ponto para onde a câmera está olhando
        0.0, 1.0, 0.0);  // Vetor "up" da câmera
}

// Função principal
int main(int argc, char** argv) {
    std::cout << "Current working directory: " << std::filesystem::current_path() << std::endl;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("OBJ Loader");

    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    glClearColor(1.0, 1.0, 1.0, 1.0);
    glutMainLoop();

    return 0;
}

void desenhar() {

    loader.loadOBJ("C:\\ufpi\\9 periodo\\COMPUTACAO_GRAFICA\\atividades\\opengl_glut\\carregar_obj\\bomb.obj");

    glScalef(1.5f, 1.5f, 1.5f);
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
}
