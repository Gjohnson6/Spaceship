#include "GL\freeglut.h"
#include "Ship.h"
#include "glm\glm.hpp"
#include "glm/gtc/constants.hpp"
#include <iostream>

using namespace std;
using namespace glm;


GLuint firstPersonWindow;
GLuint thirdPersonWindow;
HDC firstPersonHDC;
HDC thirdPersonHDC;
HGLRC firstPersonHGLRC;
HGLRC thirdPersonHGLRC;

Ship ships[16];

int width = 512;
int height = 512;
int tpWidth = 1024;
int tpHeight = 512;
double xPos = 0.0;
double zPos = 4.5;
double vFOV = 50;
bool wireFrameMode = true;
double xAngle = 0.0;
double yAngle  = 0.0;

double cameraX = 0.0;
double cameraY = 0.0;
double cameraZ = 15.0;

void FirstPersonDisplayFunc();
void ThirdPersonDisplayFunc();


float DegreesToRadians(float degrees)
{
	return degrees / 180.0f * pi<float>();
}

void Redisplay()
{
	wglMakeCurrent(firstPersonHDC, firstPersonHGLRC);
	FirstPersonDisplayFunc();
	wglShareLists(firstPersonHGLRC, thirdPersonHGLRC);

	SwapBuffers(thirdPersonHDC);
	wglMakeCurrent(thirdPersonHDC, thirdPersonHGLRC);
	ThirdPersonDisplayFunc();
}

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

void DrawMany()
{
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
}

//First Person Window Functions
void FirstPersonReshapeFunc(int w, int h)
{
	width = w;
	height = h;
	SwapBuffers(firstPersonHDC);
	wglMakeCurrent(firstPersonHDC, firstPersonHGLRC);
	FirstPersonDisplayFunc();
	wglShareLists(firstPersonHGLRC, thirdPersonHGLRC);
}

void FirstPersonDisplayFunc()
{
	int elapsed_time = glutGet(GLUT_ELAPSED_TIME);

	GLReturnedError("Entering DisplayFunc");

	glClearColor(51 / 255.0f, 51 / 255.0f, 51 / 255.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(vFOV, width / (double)height, 1.0, 23);

	glPolygonMode(GL_FRONT_AND_BACK, wireFrameMode ?  GL_FILL : GL_LINE);

	glViewport(0, 0, width, height);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//cout << cameraX << "," << cameraY << "," << cameraZ << endl;
	gluLookAt(cameraX, cameraY, cameraZ, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

/*
	glRotated(xAngle, 1, 0.0, 0.0);
	glRotated(yAngle, 0.0, 1.0, 0.0);*/

	DrawMany();
	glutSwapBuffers();
	glFlush();
}

//Third Person Window Functions
void ThirdPersonDisplayFunc()
{
	int elapsed_time = glutGet(GLUT_ELAPSED_TIME);

	GLReturnedError("Entering DisplayFunc");

	glClearColor(51 / 255.0f, 51 / 255.0f, 51 / 255.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(-(tpWidth/50.0), tpWidth/50.0, -(tpHeight/20.0), tpHeight/20.0, 1, 1000);


	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glViewport(0, 0, tpWidth/2, tpHeight);

	glPushMatrix();
	gluLookAt(50.0, 0.0, 50, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	glBegin(GL_LINES);
	glVertex3d(cameraX, cameraY, cameraZ);
	glVertex3d(0.0, 0.0, 0.0);
	glEnd();

	GLUquadricObj* quadric;
	quadric = gluNewQuadric();
	gluQuadricDrawStyle(quadric, GL_LINE);
	glPushMatrix();
	glTranslated(cameraX, cameraY, cameraZ);
	gluSphere(quadric, .25, 10, 10);
	glPopMatrix();
	glRotated(45, 0.0, 1.0, 0.0);
	gluSphere(quadric, 15, 40, 18);
	glRotated(-45, 0.0, 1.0, 0.0);
	glPolygonMode(GL_FRONT_AND_BACK, wireFrameMode ? GL_FILL : GL_LINE);
	DrawMany();

	glPopMatrix();
	glViewport(tpWidth / 2, 0, tpWidth / 2, tpHeight);
	gluLookAt(50.0, 0.0, -50.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	glBegin(GL_LINES);
	glVertex3d(cameraX, cameraY, cameraZ);
	glVertex3d(0.0, 0.0, 0.0);
	glEnd();
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glRotated(45, 0.0, 1.0, 0.0);
	gluSphere(quadric, 15, 40, 18);
	glRotated(-45, 0.0, 1.0, 0.0);
	glPolygonMode(GL_FRONT_AND_BACK, wireFrameMode ? GL_FILL : GL_LINE);
	gluDeleteQuadric(quadric);
	DrawMany();
	glutSwapBuffers();
	glFlush();
}

void ThirdPersonReshapeFunc(int w, int h)
{
	tpWidth = w;
	tpHeight = h;
	SwapBuffers(thirdPersonHDC);
	wglMakeCurrent(thirdPersonHDC, thirdPersonHGLRC);
	ThirdPersonDisplayFunc();
}


void TimerFunc(int period)
{
	glutTimerFunc(period, TimerFunc, period);
	wglMakeCurrent(firstPersonHDC, firstPersonHGLRC);
	FirstPersonDisplayFunc();
	wglShareLists(firstPersonHGLRC, thirdPersonHGLRC);

	SwapBuffers(thirdPersonHDC);
	wglMakeCurrent(thirdPersonHDC, thirdPersonHGLRC);
	ThirdPersonDisplayFunc();
	SwapBuffers(firstPersonHDC);
}

void UpdateCamera()
{
	cameraX = 15.0 * cos(DegreesToRadians( xAngle)) * cos(DegreesToRadians(yAngle));
	cameraY = 15.0 * sin(DegreesToRadians(xAngle));
	cameraZ = 15.0 * cos(DegreesToRadians(xAngle)) * sin(DegreesToRadians(yAngle));
}

void SpecialFunc(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_PAGE_UP:
		vFOV = vFOV + 1 <= 80 ? vFOV += 1.0 : vFOV = 80;
		Redisplay();
		break;
	case GLUT_KEY_PAGE_DOWN:
		vFOV = vFOV - 1 >= 10 ? vFOV -= 1.0 : vFOV = 10;
		Redisplay();
		break;
	case GLUT_KEY_UP:
		xAngle = xAngle < 89 ? xAngle += 1.0 : xAngle = 89;
		UpdateCamera();
		Redisplay();
		break;
	case GLUT_KEY_DOWN:
		xAngle = xAngle > -89 ? xAngle -= 1.0 : xAngle = -89;

		UpdateCamera();
		Redisplay();
		break;
	case GLUT_KEY_LEFT:
		yAngle += 1;
		UpdateCamera();
		Redisplay();
		break;
	case GLUT_KEY_RIGHT:
		yAngle -= 1;
		UpdateCamera();
		Redisplay();
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
		wireFrameMode = !wireFrameMode;
		Redisplay();
		break;
	default:
		break;
	}
}



int main(int argc, char * argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(width, height);
	firstPersonWindow = glutCreateWindow("First Person");
	firstPersonHDC = wglGetCurrentDC();
	firstPersonHGLRC = wglCreateContext(firstPersonHDC);

	glutTimerFunc(1000 / 60, TimerFunc, 1000 / 60);
	glutReshapeFunc(FirstPersonReshapeFunc);
	glutKeyboardFunc(KeyboardFunc);
	glutSpecialFunc(SpecialFunc);
	glutDisplayFunc(FirstPersonDisplayFunc);


	glutInitWindowPosition(width, height);
	glutInitWindowSize(tpWidth, tpHeight);
	thirdPersonWindow = glutCreateWindow("Third Person");
	thirdPersonHDC = wglGetCurrentDC();
	thirdPersonHGLRC = wglCreateContext(firstPersonHDC);
	glutReshapeFunc(ThirdPersonReshapeFunc);
	glutKeyboardFunc(KeyboardFunc);
	glutSpecialFunc(SpecialFunc);
	glutDisplayFunc(ThirdPersonDisplayFunc);
	glutMainLoop();
}