#include "GL\freeglut.h"
#include "Ship.h"
#include <iostream>

using namespace std;

GLuint firstPersonWindow;
GLuint thirdPersonWindow;
Ship ships[16];

int width = 512;
int height = 512;
double xPos = 0.0;
double zPos = 4.5;
double vFOV = 50;
bool wireFrameMode = true;
double xAngle = 0.0;
double yAngle  = 0.0;

bool GLReturnedError(char * s)
{
	bool return_error = false;
	GLenum glerror;

	while ((glerror = glGetError()) != GL_NO_ERROR)
	{
		return_error = true;
		cerr << s << ": " << gluErrorString(glerror) << endl;
	}

	return return_error;
}

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
		vFOV = vFOV + 1 <= 80 ? vFOV += 1.0 : vFOV = 80;
		glutPostRedisplay();
		break;
	case GLUT_KEY_PAGE_DOWN:
		vFOV = vFOV - 1 >= 10 ? vFOV -= 1.0 : vFOV = 10;
		cout << vFOV << endl;
		glutPostRedisplay();
		break;
	case GLUT_KEY_UP:
		xAngle = xAngle < 89 ? xAngle += 1.0 : xAngle = 89;
		glutPostRedisplay();
		break;
	case GLUT_KEY_DOWN:
		xAngle = xAngle > -89 ? xAngle -= 1.0 : xAngle = -89;
		glutPostRedisplay();
		break;
	case GLUT_KEY_LEFT:
		yAngle += 1;
		glutPostRedisplay();
		break;
	case GLUT_KEY_RIGHT:
		yAngle -= 1;
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
	default:
		break;
	}
}

//#define test
void DrawMany()
{
#ifdef test
	Ship ship = Ship();
	for (int x = -3; x <= 3; x+=2)
	{
		for (int z = -3; z <= 3; z+=2)
		{			
			glPushMatrix();
			glTranslated(x * 2.5, 0.0, z*2.5);
			ship.DrawShip();
			glPopMatrix();
		}

	}
#else
	int shipNum = 0;
	for (int x = -3; x <= 3; x += 2)
	{
		for (int z = -3; z <= 3; z += 2)
		{
			glPushMatrix();
			if (ships[shipNum].GetHandler() == -1)
			{
				ships[shipNum] = Ship(x * 2.5, 0.0, z * 2.5);
			}
			ships[shipNum].DrawShip();
			glPopMatrix();
			shipNum++;
		}
	}
#endif
}

void DisplayFunc()
{
	int elapsed_time = glutGet(GLUT_ELAPSED_TIME);

	GLReturnedError("Entering DisplayFunc");

	glClearColor(51 / 255.0f, 51 / 255.0f, 51 / 255.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(vFOV, width / (double)height, 1.0, 23);

	gluLookAt(0.0, 0.0, 15.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	glRotated(xAngle, 1, 0.0, 0.0);
	glRotated(yAngle, 0.0, 1.0, 0.0);

	glViewport(0, 0, width, height);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	DrawMany();
	glutSwapBuffers();
	glFlush();
}

void DisplayFunc2()
{
	int elapsed_time = glutGet(GLUT_ELAPSED_TIME);

	GLReturnedError("Entering DisplayFunc");

	glClearColor(51 / 255.0f, 51 / 255.0f, 51 / 255.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(vFOV, width / (double)height, 1.0, 23);

	gluLookAt(0.0, 0.0, 15.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	glRotated(xAngle, 1, 0.0, 0.0);
	glRotated(yAngle, 0.0, 1.0, 0.0);

	glViewport(0, 0, width, height);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	DrawMany();
	glutSwapBuffers();
	glFlush();
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

	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(width, height);
	glutInitWindowPosition(width, height);
	thirdPersonWindow = glutCreateWindow("Third Person");
	//glutTimerFunc(1000 / 60, TimerFunc, 1000 / 60);
	glutReshapeFunc(ReshapeFunc);
	glutKeyboardFunc(KeyboardFunc);
	glutSpecialFunc(SpecialFunc);
	glutDisplayFunc(DisplayFunc2);
	glutMainLoop();
}