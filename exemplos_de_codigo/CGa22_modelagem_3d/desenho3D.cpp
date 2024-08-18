/*
 * Computacao Grafica
 * Codigo Exemplo: Modelagem Geometrica 3D usando GLUT/OpenGL
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

#include <cstdio>
#include <cmath>
#include "glut_text.h"

/*
 * Declaracao de constantes e variaveis
 */
#define ESC 27 // ASCII para a tecla ESC
#define MAX_FPS 70 // Maximo de Frames Por Segundo (FPS) desejado
#define FPS 60 // FPS desejado atualmente

int fps_desejado = FPS/2; // variavel para alterar os frames por segundo desejado
int fps = 0; //contador de frames por segundo

float angulo = 0.0; // angulo de rotacao
int graus = 5; // incremento do angulo de rotacao

float ratio = 1.0; // Manter proporcional a projecao

float R = 0.0, G = 0.0, B = 1.0; //Cores do desenho (inicial: cor azul)

bool use_gouraud = true; // Determina o uso de Gouraud ou Flat shading

bool use_depth_test = true; // Determina o uso de Z-buffering

bool use_light = true; // Determina se liga a luz

// Vertices do objeto
#define N_VERTICES 4
const GLfloat vertices[N_VERTICES][3] = {
    { -0.5, -0.272,  0.289 }, //Vertice 0
    {  0.5, -0.272,  0.289 }, //Vertice 1
    {  0.0, -0.272, -0.577 }, //Vertice 2
    {  0.0,  0.544,  0.000 }  //Vertice 3
};

// Faces do objeto (Triangulos)
#define N_FACES 4
const GLuint faces[N_FACES][3] = {
    {3, 0, 1}, //Face 0
    {3, 1, 2}, //Face 1
    {3, 2, 0}, //Face 2

    /* ATENCAO:
     * De acordo com a vista de cima, os vertices do triangulo da base
     * da piramide estao definidos no sentido horario, ao contrario do
     * que temos feito ate aqui. Isso porque, segundo a vista de cima,
     * esse triangulo esta na realidade sendo visto "de costas".
     */
    {0, 2, 1}  //Face 3
};



// Vetores normais dos vertices do objeto
GLfloat vertex_normals[N_VERTICES][3];

// Vetores normais das faces do objeto
GLfloat face_normals[N_FACES][3];

//Estrutura criada para facilitar o entendimento no calculo das normais
struct vertice{
    float x;
    float y;
    float z;
};
  
void normalizar(vertice * n){
	GLfloat length;
	
	length = (GLfloat) sqrt((n->x * n->x) + (n->y * n->y) + (n->z * n->z));
		
	if (length == 0.0f) length =  1.0f;

	n->x = n->x / length;
	n->y = n->y / length;
	n->z = n->z / length;
}
    
void calcularNormaisFaces(){
    float x[3], y[3], z[3];
    vertice a, b, n;
        
    for(int i = 0; i < N_FACES; i++){
        x[0] = vertices[faces[i][0]][0];
        y[0] = vertices[faces[i][0]][1];
        z[0] = vertices[faces[i][0]][2];
    
        x[1] = vertices[faces[i][1]][0];
        y[1] = vertices[faces[i][1]][1];
        z[1] = vertices[faces[i][1]][2];
    
        x[2] = vertices[faces[i][2]][0];
        y[2] = vertices[faces[i][2]][1];
        z[2] = vertices[faces[i][2]][2];
        
        a.x = x[2]- x[0]; //
        a.y = y[2]- y[0];
        a.z = z[2]- z[0];
        
        b.x = x[2]- x[1];
        b.y = y[2]- y[1];
        b.z = z[2]- z[1];
        
        n.x = a.y * b.z - a.z * b.y;
        n.y = a.z * b.x - a.x * b.z;
        n.z = a.x * b.y - a.y * b.x;
       
        normalizar(&n);
        
        face_normals[i][0] = n.x;
        face_normals[i][1] = n.y;
        face_normals[i][2] = n.z;
    }
}

void calcularNormaisVertices(){
    vertice n;
    for(unsigned int i = 0; i < N_VERTICES; i++){
        n.x = 0.0;
        n.y = 0.0;
        n.z = 0.0;
        for(int j = 0; j < N_FACES; j++){
            if(faces[j][0] == i || faces[j][1] == i || faces[j][2] == i){
                n.x += face_normals[j][0];
                n.y += face_normals[j][1];
                n.z += face_normals[j][2];
            }
        }
        
        n.x /= 3.0;
        n.y /= 3.0;
        n.z /= 3.0;        
        
        normalizar(&n);
        
        vertex_normals[i][0] = n.x;
        vertex_normals[i][1] = n.y;
        vertex_normals[i][2] = n.z;        

    }
    
}        
    
/*
 * Declaracoes antecipadas (forward) das funcoes (assinaturas)
 */
void init_glut(const char *window_name, int argc, char** argv);
void draw_object_smooth(void);
void draw_object_flat(void);
void display(void);
void reshape(int w, int h);
void computeFPS();
void timer(int value);
void keyboard(unsigned char key, int x, int y);
void keyboard_special(int key, int x, int y);
void info_modotexto();

/*
 * Funcao principal
 */
int main(int argc, char** argv){
    calcularNormaisFaces();
    calcularNormaisVertices();
    
    info_modotexto();
    
   /* inicia o GLUT e alguns parametros do OpenGL */
   init_glut("Computacao Grafica: Modelando e Animando Objetos Complexos em 3D", argc, argv);

   /* funcao de controle do GLUT */
   glutMainLoop();

   return EXIT_SUCCESS;
}

/*
 * inicia o GLUT e alguns parametros do OpenGL
 */
void init_glut(const char *nome_janela, int argc, char** argv){
    // Inicia o GLUT
    glutInit(&argc,argv);

    // Inicia o display usando RGB, double-buffering e Z-buffering
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(640,480);
    glutInitWindowPosition(100,100);
    glutCreateWindow(nome_janela);

    // Define as funcoes de callback
    glutKeyboardFunc(keyboard);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutSpecialFunc(keyboard_special);

    glutTimerFunc(1000/fps_desejado, timer, 0); //(mseg, timer, value)

    // Inicia a iluminacao
    GLfloat light_position[] = {-1.0, 1.0, 1.0, 0.0};
	GLfloat light_color[] = {1.0, 1.0, 1.0, 0.0};
    glLightfv(GL_LIGHT0, GL_AMBIENT_AND_DIFFUSE, light_color);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glEnable(GL_LIGHTING); //Ativa o modelo de iluminacao
    glEnable(GL_LIGHT0); //Ativa a Luz 0. O OpenGL suporta pelo menos 8 pontos de luz

    // Inicia as caracteristicas gerais dos materiais
	GLfloat mat_ambient_diffuse[] = {1.0, 1.0, 1.0, 1.0};
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
void reshape(int w, int h){

    // Muda para o modo GL_PROJECTION e reinicia a projecao
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity();

    // Define a zona da janela onde se vai desenhar
    glViewport (0, 0, (GLsizei) w, (GLsizei) h);
	
    // Define uma projecao perspectiva (3D)
    ratio = (float)w/(float)h;
    gluPerspective(30, ratio, 1.0, 100.0);

	// Translada a camera no eixo Z se distanciando do objeto
    gluLookAt (0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    // muda para o modo GL_MODELVIEW para preparar o desenho
    glMatrixMode (GL_MODELVIEW);
}

/*
 * Desenha o objeto, triangulo por triangulo, definindo os vetores
 * normais de cada vertice (ideal para usar com Gouraud shading).
 */
void draw_object_smooth(void){
    GLuint i;

    // Desenha todos os triangulos do objeto
    glBegin(GL_TRIANGLES);
        for (i=0; i<N_FACES; i++){
            glNormal3fv(vertex_normals[faces[i][0]]);
            glVertex3fv(vertices[faces[i][0]]);

            glNormal3fv(vertex_normals[faces[i][1]]);
            glVertex3fv(vertices[faces[i][1]]);

            glNormal3fv(vertex_normals[faces[i][2]]);
            glVertex3fv(vertices[faces[i][2]]);
        }
    glEnd();
}

/*
 * Desenha o objeto, triangulo por triangulo, utilizando um vetor
 * normal para cada triangulo (ideal para usar com Flat shading).
 */
void draw_object_flat(void){
    GLuint i;

    // Desenha todos os triangulos do objeto
    glBegin(GL_TRIANGLES);
        for (i=0; i<N_FACES; i++){
            glNormal3fv(face_normals[i]);
            glVertex3fv(vertices[faces[i][0]]);
            glVertex3fv(vertices[faces[i][1]]);
            glVertex3fv(vertices[faces[i][2]]);
        }
    glEnd();
}

/*
 * Funcao para controlar o display
 */
void display(void){
    // Computa a taxa  de desenho de frames por segundo
    computeFPS();

    // Apaga o video e o depth buffer, e reinicia a matriz
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    
    glColor3f(R, G, B); //Define a cor do objeto

    // Faz variar o angulo de rotacao entre 0 e 359 graus, e roda
    // Roda o objeto segundo o eixo x e y.
    angulo += graus;
    if (angulo >= 359.0) angulo -= 359.0;
    glPushMatrix();
        glRotatef(angulo, 1.0, 1.0, 0.0);
        // Desenha o objeto
        if (use_gouraud) draw_object_smooth();
        else draw_object_flat();
    glPopMatrix();

    // desenhando textos na janela GLUT
    glColor3f(0.0, 0.0, 0.0); // seleciona a cor preta para o texto
    std::string luz = (use_light)?"On":"Off";
    std::string dep = (use_depth_test)?"On":"Off";
    std::string mat = (use_gouraud)?"Smooth":"Flat";
    std::string red = (R == 1.0)?"On":"Off";
    std::string gre = (G == 1.0)?"On":"Off";
    std::string blu = (B == 1.0)?"On":"Off";
    float x = -ratio-0.45;
    draw_text_stroke(x , 1.25, "(ESC)EXIT (</>)FPS: " + to_string(fps) + " (-/+)GRAUS: " + to_string(graus));
    draw_text_stroke(x , 1.15, "(F1)BLUE: " + blu + "  (F2)GREEN: " + gre + "  (F3)RED: " + red);
    draw_text_stroke(x , 1.05, "(F4)LIGHT: "+ luz + "  (F5)DEPTH: " +dep + "  (F6)SHADE: " + mat);

    
    // Troca os buffers, mostrando o que acabou de ser desenhado
    glutSwapBuffers();
}

/*
 * Computa a quantidade de frames por segundo da animacao
 */
void computeFPS(){
    static GLuint frames = 0;        // frames 1000 milissegundos
    static GLuint clock;             // em milissegudos
    static GLuint next_clock = 0;    // em milissegudos

    frames++;
    clock = glutGet(GLUT_ELAPSED_TIME); //Nœmero de milissegundos desde a chamada a glutInit()
    if (clock < next_clock ) return;

    fps = frames; // guarda o nœmero de frames por segundo

    // Evita o reinicio da contagem dos frames na primeira iteracao
    if(next_clock != 0) frames = 0;//Reinicia a contagem dos frames a cada 1000 milissegundos
    next_clock = clock + 1000; // 1000 milissegundos = 1 segundo
}

/*
 * Funcao utilizada para a animacao com temporizador
 */
void timer(int value){
    glutTimerFunc(1000/fps_desejado, timer, 0);
    glutPostRedisplay(); // Manda redesenhar a tela em cada frame
}


void info_modotexto(){
    puts("MENU DE AJUDA\n");
    puts("F1: Muda a cor para Azul");
    puts("F2: Muda a cor para Verde");
    puts("F3: Muda a cor para Vermelho");     
    puts("F4: liga/desliga a luz");
    puts("F5: liga/desliga o gouraud shading");
    puts("F6: liga/desliga o Z-buffering");
    puts("</>: decrementa/incrementa o FPS desejado");
    puts("-/+: decrementa/incrementa o graus");
    puts("ESC: sai do programa");
    puts("");
    puts("NORMAIS DA FACE\n");
    for(int i=0; i < N_FACES; i++){
        printf("Normal Face %d: (%f, %f, %f)\n",i, face_normals[i][0],face_normals[i][1],face_normals[i][2]);
    }
    puts("");
    puts("NORMAIS DO VERTICE\n");
    for(int i=0; i < N_VERTICES; i++){
        printf("Normal Vertice %d: (%f, %f, %f)\n",i, vertex_normals[i][0],vertex_normals[i][1],vertex_normals[i][2]);
    }   
}

/*
 * Controle das teclas comuns
 */
void keyboard(unsigned char key, int x, int y){
    switch(key){
        // Tecle - ou + para alterar o incremento do angulo de rotacao
        case '-': case '_': if(graus > 1) graus -= 1; break;
        case '+': case '=': if(graus < 360) graus += 1; break;
        
        // Tecle < ou > para alterar o FPS desejado
        case ',': case '<': if(fps_desejado > 1) fps_desejado -= 1; break;
        case '.': case '>': if(fps_desejado*2 < MAX_FPS) fps_desejado += 1; break;
        case ESC: exit(EXIT_SUCCESS); break;
    }
}


/*
 * Controle das teclas especiais (Cursores, F1 a F12, etc...)
 */
void keyboard_special(int key, int x, int y){
    switch(key){
        case GLUT_KEY_F1: R = 0.0; G = 0.0; B = 1.0; break;//Muda a cor para azul
        case GLUT_KEY_F2: R = 0.0; G = 1.0; B = 0.0; break;//Muda a cor para verde
        case GLUT_KEY_F3: R = 1.0; G = 0.0; B = 0.0; break;//Muda a cor para vermelho
        case GLUT_KEY_F4: //F4: liga/desliga a luz
            if (use_light){
               use_light = false;
               glDisable(GL_LIGHTING);
            }else{
               use_light = true;
               glEnable(GL_LIGHTING);
            }
        break;
        case GLUT_KEY_F5: //F5: liga/desliga o Z-buffering
            if (use_depth_test){
               use_depth_test = false;
               glDisable(GL_DEPTH_TEST);
            }else{
               use_depth_test = true;
               glEnable(GL_DEPTH_TEST);
            }
        break;

        case GLUT_KEY_F6: //F6: liga/desliga o gouraud shading
            if (use_gouraud){
               use_gouraud = false;
               glShadeModel(GL_FLAT);
               
            }else{
               use_gouraud = true;
               glShadeModel(GL_SMOOTH);
            }
        break;

    }
}
