#include "GL\freeglut.h"
#include "Ship.h"
#include <iostream>

using namespace std;

GLuint firstPersonWindow;
GLuint thirdPersonWindow;

int width = 512;
int height = 512;
double xPos = 0.0;
double zPos = 4.5;
double zCenter = 0.0;
double xCenter = 0.0;
double yCenter = 0.0;
double vFOV = 50;
bool wireFrameMode = true;

void TimerFunc(int period)
{
	glutTimerFunc(period, TimerFunc, period);
	glutPostRedisplay();
}


void ReshapeFunc(int w, int h)
{
	width = w;
	height = h;
	glutPostRedisplay();
}

void SpecialFunc(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_PAGE_UP:
		vFOV -= 1.0;
		glutPostRedisplay();
		break;
	case GLUT_KEY_PAGE_DOWN:
		vFOV += 1.0;
		glutPostRedisplay();
		break;
	case GLUT_KEY_UP:
		zPos -= 1.0;
		glutPostRedisplay();
		break;
	case GLUT_KEY_DOWN:
		zPos += 1.0;
		glutPostRedisplay();
		break;
	case GLUT_KEY_LEFT:
		xPos -= 1.0;
		glutPostRedisplay();
		break;
	case GLUT_KEY_RIGHT:
		xPos += 1.0;
		cout << "key right" << endl;
		glutPostRedisplay();
		break;
	default:
		break;
	}
}

void KeyboardFunc(unsigned char c, int x, int y)
{
	cout << "keyfunc" << endl;
	switch (c)
	{
	case 27:
	case 'x':
		glutLeaveMainLoop();
		break;
	case 'w':
		glPolygonMode(GL_FRONT_AND_BACK, wireFrameMode ? GL_LINE : GL_FILL);
		wireFrameMode = !wireFrameMode;
		glutPostRedisplay();
		break;
	case 'i':
		zCenter -= 1.0;
		glutPostRedisplay();
		break;
	case 'k':
		zCenter += 1.0;
		glutPostRedisplay();
		break;
	case 'l':
		xCenter += 1.0;
		glutPostRedisplay;
		break;
	case 'j':
		xCenter -= 1.0;
		glutPostRedisplay;
		break;
	case 'p':
		yCenter += 1.0;
		glutPostRedisplay;
		break;
	case ';':
		yCenter -= 1.0;
		glutPostRedisplay;
		break;

	default:
		break;
	}
}

void DrawShips()
{
	for (int x = -3; x <= 3; x+=2)
	{
		for (int z = -3; z <= 3; z+=2)
		{
			
			glPushMatrix();
			glTranslated(x * 2.5, 0.0, z*2.5);
			Ship ship = Ship();
			ship.drawShip();
			glPopMatrix();
		}

	}
}

void DisplayFunc()
{
	int elapsed_time = glutGet(GLUT_ELAPSED_TIME);

	glClearColor(51 / 255.0f, 51 / 255.0f, 51 / 255.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(vFOV, width / (double)height, 1.0, 35);

	gluLookAt(xPos, 4.0, zPos, xCenter, yCenter, zCenter, 0.0, 1.0, 0.0);

	glViewport(0, 0, width, height);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	glRotated(elapsed_time / 100.0, 0.0, 1.0, 0.0);

	DrawShips();

	glutSwapBuffers();
}


int main(int argc, char * argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(width, height);
	firstPersonWindow = glutCreateWindow("First Person");
	glutTimerFunc(1000 / 60, TimerFunc, 1000 / 60);
	glutReshapeFunc(ReshapeFunc);
	glutKeyboardFunc(KeyboardFunc);
	glutSpecialFunc(SpecialFunc);
	glutDisplayFunc(DisplayFunc);
	glutMainLoop();
}