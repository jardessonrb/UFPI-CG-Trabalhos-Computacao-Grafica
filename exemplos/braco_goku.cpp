/*#############################################################################*/
/*	Teclas:
			o ou O para girar o ombro
			c ou C para girar o cotovelo
			p ou P para girar o Polegar
			i ou I para girar o Indicador
			m ou M para girar o Dedo medio
			
*/
/*#############################################################################*/

#include <stdlib.h>
#include <GL/glut.h>

static int ombro = 0, cotovelo = 0, indicador = 0, polegar = 0, medio = 0;

void init(void)
{
	glClearColor (0.0, 0.0, 0.0, 0.0);
}

void display(void)
{
	glClear (GL_COLOR_BUFFER_BIT);
	glPushMatrix();

	/* origem posicionada no ombro */
	glTranslatef (-1.0, 0.0, 0.0);
	glRotatef ((GLfloat) ombro, 0.0, 0.0, 1.0);

	/* origem posicionada no centro do braco */
	glColor3f(0.5,0.5,0.5);
	glTranslatef (1.0, 0.0, 0.0);
	glPushMatrix();
	glScalef (2.0, 0.4, 1.0);
	glutSolidCube (1.0);
	glPopMatrix();

	/* origem posicionada no cotovelo */
	glColor3f(0.7,0.7,0.7);
	glTranslatef (1.0, 0.0, 0.0);
	glRotatef ((GLfloat) cotovelo, 0.0, 0.0, 1.0);
	glTranslatef (1.0, 0.0, 0.0);
	glPushMatrix();
	glScalef (2.0, 0.4, 1.0);
	glutSolidCube (1.0);
	glPopMatrix();

	/* origem posicionada no Polegar */
	glColor3f(1,0,0);
	glTranslatef (1.0, 0.0, 0.0);
	glPushMatrix();
	glTranslatef (0.0, -0.2, 0.0);
	glRotatef ((GLfloat) polegar, 0.0, 0.0, 1.0);
	glTranslatef (0.25, 0.0, 0.0);
	glScalef (0.5, 0.1, 0.33);
	glutSolidCube (1.0);
	glPopMatrix();

	/* origem posicionada no Indicador */
	glColor3f(0,1,0);
	glTranslatef (0.0, 0.2, 0.0);
	glPushMatrix();
	glTranslatef (0.0, 0.0, 0.25);
	glRotatef ((GLfloat) indicador, 0.0, 0.0, 1.0);
	glTranslatef (0.25, 0.0, 0.0);
	glScalef (0.5, 0.1, 0.33);
	glutSolidCube (1.0);
	glPopMatrix();

	/* origem posicionada no Dedo Médio */
	glColor3f(0,0,1);
	glTranslatef (0.0, 0.0, -0.25);
	glRotatef ((GLfloat) medio, 0.0, 0.0, 1.0);
	glTranslatef (0.25, 0.0, 0.0);
	glScalef (0.5, 0.1, 0.33);
	glutSolidCube (1.0);
	glPopMatrix();

	/* origem volta para o sistema de coordenadas original */
	glPopMatrix();
	glutSwapBuffers();
}

void reshape (int w, int h)
{
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluPerspective(65.0, (GLfloat) w / (GLfloat) h, 1.0, 20.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef (0.0, 0.0, -10.0);
}

void keyboard (unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'o':
		ombro = (ombro + 5) % 360;
		glutPostRedisplay();
		break;
	case 'O':
		ombro = (ombro - 5) % 360;
		glutPostRedisplay();
		break;
	case 'c':
		cotovelo = (cotovelo + 5) % 360;
		glutPostRedisplay();
		break;
	case 'C':
		cotovelo = (cotovelo - 5) % 360;
		glutPostRedisplay();
		break;
	case 'i':
		indicador = (indicador + 5) % 360;
		glutPostRedisplay();
		break;
	case 'I':
		indicador = (indicador - 5) % 360;
		glutPostRedisplay();
		break;
	case 'p':
		polegar = (polegar + 5) % 360;
		glutPostRedisplay();
		break;
	case 'P':
		polegar = (polegar - 5) % 360;
		glutPostRedisplay();
		break;
	case 'm':
		medio = (medio + 5) % 360;
		glutPostRedisplay();
		break;
	case 'M':
		medio = (medio - 5) % 360;
		glutPostRedisplay();
		break;
	case 27:
		exit(0);
		break;
	default:
		break;
	}
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize (600, 600);
	glutInitWindowPosition (100, 100);
	glutCreateWindow (argv[0]);
	init ();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
	return 0;
}