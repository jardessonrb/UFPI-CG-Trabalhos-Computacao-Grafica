/*
 * Computacao Grafica
 * Codigo Exemplo: Modelagem Geometrica 2D usando GLUT/OpenGL
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
int graus = 10; // incremento do angulo de rotacao

float ratioX = 1.0; // Manter proporcional a projecao na horizontal
float ratioY = 1.0; // Manter proporcional a projecao na vertical

float R = 0.0, G = 0.0, B = 1.0; //Cores do desenho (inicial: cor azul)

// Vertices do objeto
#define N_VERTICES 16
const GLfloat vertices[N_VERTICES][3] = {
    { 0.0,  6.0, 0.0}, //Vertice 0
    {-1.0,  2.0, 0.0}, //Vertice 1
    {-2.0,  1.0, 0.0}, //Vertice 2
    {-6.0,  0.0, 0.0}, //Vertice 3
    {-2.0, -1.0, 0.0}, //Vertice 4
    {-1.0, -2.0, 0.0}, //Vertice 5
    { 0.0, -6.0, 0.0}, //Vertice 6
    { 1.0, -2.0, 0.0}, //Vertice 7
    { 2.0, -1.0, 0.0}, //Vertice 8
    { 6.0,  0.0, 0.0}, //Vertice 9
    { 2.0,  1.0, 0.0}, //Vertice 10
    { 1.0,  2.0, 0.0}, //Vertice 11
    { 0.0,  1.0, 0.0}, //Vertice 12
    {-1.0,  0.0, 0.0}, //Vertice 13
    { 0.0, -1.0, 0.0}, //Vertice 14
    { 1.0,  0.0, 0.0}  //Vertice 15
 };

// Faces do objeto (Triangulos)
#define N_FACES 16
const GLuint faces[N_FACES][3] = {
    { 0, 1,11}, //Face 0
    { 2, 3, 4}, //Face 1
    { 5, 6, 7}, //Face 2
    {10, 8, 9}, //Face 3
    { 1,12,11}, //Face 4
    { 1, 2,12}, //Face 5
    { 2,13,12}, //Face 6
    { 2, 4,13}, //Face 7
    {13, 4,14}, //Face 8
    {14, 4, 5}, //Face 9
    {14, 5, 7}, //Face 10
    {14, 7, 8}, //Face 11
    {15,14, 8}, //Face 12
    {15, 8,10}, //Face 13
    {12,15,10}, //Face 14
    {11,12,10}  //Face 15
 };
 
/*
 * Declaracoes antecipadas (forward) das funcoes (assinaturas das funcoes)
 */
void init();
void draw_object();
void display();
void reshape(int w, int h);
void computeFPS();
void timer(int value);
void idle();
void menu(int value);
void keyboard_special(int key, int x, int y);
void keyboard(unsigned char key, int x, int y);

/*
 * Funcao principal
 */
int main(int argc, char** argv){

	// inicia o GLUT
	glutInit(&argc,argv);

    // inicia o display usando RGB e double-buffering
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(640,480);
    glutInitWindowPosition(100,100);
    glutCreateWindow("Computacao Grafica: Modelando e Animando Objetos Complexos em 2D");

    // define as func›es de callback
    glutKeyboardFunc(keyboard);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutSpecialFunc(keyboard_special);

    glutCreateMenu(menu);
    glutAddMenuEntry("Desenhar azul", 0);
    glutAddMenuEntry("Desenhar verde", 1);
    glutAddMenuEntry("Desenhar vermelho", 2);
    glutAddMenuEntry("Sair", 3);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    glutTimerFunc(1000/fps_desejado, timer, 0); //(mseg, timer_callback, value)
//    glutIdleFunc(idle_callback);
	
    // inicializa alguns parametros do OpenGL
	init();
   
	// funcao de controle do GLUT
	glutMainLoop();

	return EXIT_SUCCESS;
}

/*
 * Inicializa alguns parametros do GLUT
 */
void init(){
    // define a cor com a qual a tela sera apagada
    glClearColor(0.0,0.0,0.0,0.0);
}

/*
 * Ajusta a projecao para o redesenho da janela
 */
void reshape(int w, int h){
    // muda para o modo GL_PROJECTION e reinicia a projecao
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity();

    // define a zona da janela onde se vai desenhar
    glViewport (0, 0, (GLsizei) w, (GLsizei) h);

    // adapta a projecao ortografica de acordo com as dimensoes da janela
    ratioX = 1.0; ratioY = 1.0;
    if (h > w){
        ratioY = (GLfloat)h/(GLfloat)w;
    }else{
        ratioX = (GLfloat)w/(GLfloat)h;
    }
    glOrtho(-ratioX, ratioX, -ratioY, ratioY,0.0,1.0);

    // muda para o modo GL_MODELVIEW (nao pretendemos alterar a projecao
    // quando estivermos desenhando na tela)
    glMatrixMode (GL_MODELVIEW);
}

/*
 * Desenha o objeto, triangulo por triangulo
 */
void draw_object(void){
    int i;

    // Desenha todos os triangulos do objeto
    glBegin(GL_TRIANGLES);
        for (i=0; i<N_FACES; i++){
            glVertex3fv(vertices[faces[i][0]]);
            glVertex3fv(vertices[faces[i][1]]);
            glVertex3fv(vertices[faces[i][2]]);
        }
    glEnd();

}

/*
 * Controla os desenhos na tela (desenha cada frame na tela)
 */
void display(void){
    // Computa a taxa  de desenho de frames por segundo
    computeFPS();

    // Apaga o tela e reinicia a matriz
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
         
    // faz variar o angulo entre 0 e 359 graus
    angulo += graus;
    if (angulo >= 359.0) angulo -= 359.0;

    // memoriza a matriz atual (referencial) empilhando
    glColor3f(R, G, B); //seleciona a cor para o objeto
    glPushMatrix();
        // Move o referencial para a esquerda
        glTranslatef(-0.6, 0.0, 0.0);

        // Roda o referencial ("angulo" graus) sobre o eixo Z
        glRotatef(angulo, 0.0, 0.0, 1.0);

        // Redimensiona o referencial para o objeto para caber na tela
        glScalef(0.1, 0.1, 0.1);

        // Desenha o objeto
        draw_object();

    // desempilha o referencial
    glPopMatrix();

    // Semelhante ao que foi feito acima, exceto que o referencial e movido
    // para a direita e rotaciona no sentido oposto ("-angulo" graus).
    glPushMatrix();
        glTranslatef(0.6, 0.0, 0.0);
        glRotatef(-angulo, 0.0, 0.0, 1.0);
        glScalef(0.1, 0.1, 0.1);
        draw_object();
    glPopMatrix();

    // desenhando textos na janela GLUT
    glColor3f(1.0, 1.0, 1.0); // seleciona a cor branca para o texto
    draw_text_stroke(-ratioX, 0.9, "(ESC)EXIT (</>)FPS: " + to_string(fps) + " (-/+)GRAUS: " + to_string(graus));
    
    // troca os buffers, mostrando o que acabou de ser desenhado
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


/*
 * Funcao utilizada para animacao sem temporizador
 */
void idle(void){
    glutPostRedisplay(); // Manda redesenhar a tela em cada frame
}

/*
 * Funcao callback para o controle do menu pop-up
 */
void menu(int value){
    switch(value){
        case 0: R = 0.0; G = 0.0; B = 1.0; break;//Muda a cor para azul
        case 1: R = 0.0; G = 1.0; B = 0.0; break;//Muda a cor para verde
        case 2: R = 1.0; G = 0.0; B = 0.0; break;//Muda a cor para vermelho
        case 3: exit(EXIT_SUCCESS); break; //Sai do programa
    }
}

/*
 * Controle das teclas especiais (F1 a F12 etc...)
 */
void keyboard_special(int key, int x, int y){
    printf("ASCII de tecla especial: %i\n", key);
    switch(key){
        case GLUT_KEY_F1: R = 0.0; G = 0.0; B = 1.0; break;//Muda a cor para azul
        case GLUT_KEY_F2: R = 0.0; G = 1.0; B = 0.0; break;//Muda a cor para verde
        case GLUT_KEY_F3: R = 1.0; G = 0.0; B = 0.0; break;//Muda a cor para vermelho
    }

}

/*
 * Controle das teclas comuns
 */
void keyboard(unsigned char key, int x, int y){
    printf("ASCII de tecla comum: %i\n", key);
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
