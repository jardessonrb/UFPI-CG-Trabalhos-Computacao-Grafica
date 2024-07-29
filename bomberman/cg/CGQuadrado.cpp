
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
#include "CGQuadrado.h"

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


CGQuadrado::CGQuadrado(float alturaY) {
    this->alturaY = alturaY;
}

CGQuadrado::~CGQuadrado() {

}

void CGQuadrado::desenhaQuadrado(float x, float z, float tamanho) {
    int n_vertices = 8;
    int n_faces = 12;

    float x_tamanho = x + tamanho;
    float y_tamanho = this->alturaY + tamanho;
    float z_tamanho = z + tamanho;
    float y = this->alturaY;

    const GLfloat vertices[n_vertices][3] = {
        { x,         y,         z }, // Vertice 0
        { x_tamanho, y,         z }, // Vertice 1
        { x_tamanho, y_tamanho, z }, // Vertice 2
        { x,         y_tamanho, z }, // Vertice 3
        { x,         y,         z_tamanho }, // Vertice 4
        { x_tamanho, y,         z_tamanho }, // Vertice 5
        { x_tamanho, y_tamanho, z_tamanho }, // Vertice 6
        { x,         y_tamanho, z_tamanho }  // Vertice 7
    };

    const GLuint faces[n_faces][3] = {
        {3, 2, 0}, // Face inferior 
        {1, 2, 0}, // Face inferior 
        {6, 5, 7}, // Face superior 
        {4, 5, 7}, // Face superior
        {4, 1, 5}, // Face frontal 
        {2, 1, 5}, // Face frontal 
        {5, 2, 6}, // Face direita 
        {3, 2, 6}, // Face direita 
        {6, 3, 7}, // Face traseira 
        {0, 3, 7}, // Face traseira 
        {7, 0, 4}, // Face esquerda 
        {1, 4, 0}  // Face esquerda 
    };

    // /* Desenha todos os triangulos do objeto */
    glBegin(GL_TRIANGLES);
    for (int i = 0; i < n_faces; i++) {
        for (int j = 0; j < 3; j++) {
            /* Define o vertice j da face i */
            glTexCoord2fv(tex_coords[i][j]);
            glNormal3fv(vertex_normals[faces[i][j]]);
            glVertex3fv(vertices[faces[i][j]]);
        }
    }
    glEnd();
}

void CGQuadrado::desenhaObstaculos(int comprimento, int largura, float tamanho) {
    //Lado esquerdo e direito
    for (int z = 0; z < comprimento; z += tamanho)
    {
        desenhaQuadrado(0, z, tamanho);
        desenhaQuadrado(comprimento - tamanho, z, tamanho);
    }

    // Lado frente e limite
    for (int x = 0; x < comprimento; x += tamanho)
    {
        desenhaQuadrado(x, 0, tamanho);
        desenhaQuadrado(x, largura - tamanho, tamanho);
    }

    //Quadrados Alternados
    bool desenha = true;
    for (int x = (2 * tamanho), z = (2 * tamanho); x < comprimento; x += tamanho)
    {
        if (desenha) {
            bool desenha_linha = true;
            for (int z_linha = z; z_linha < comprimento; z_linha += tamanho)
            {
                if (desenha_linha) {
                    desenhaQuadrado(x, z_linha, tamanho);
                }

                desenha_linha = !desenha_linha;
            }

        }

        desenha = !desenha;
    }
}