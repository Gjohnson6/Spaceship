#include "GL\freeglut.h"
#include "Ship.h"
#include "glm\glm.hpp"
#include "glm\gtc\constants.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\matrix_inverse.hpp"
#include "glm\detail\func_geometric.hpp"
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
float xAngle = 0.0;
float yAngle  = 90.0;
vec4 frustumCoordinates[8];

double cameraX = 0.0;
double cameraY = 0.0;
double cameraZ = 15.0;

vec3 cameraCoords = {0.0f, 0.0f, 15.0f};


void FirstPersonDisplayFunc();
void ThirdPersonDisplayFunc();


float DegreesToRadians(float degrees)
{
	return degrees / 180.0f * pi<float>();
}

void Redisplay()
{
	
	cout << "Redisplay" << endl;

	//Works but no rockets in third person
	/*
	glutSetWindow(firstPersonWindow);
	glutPostRedisplay();
	glutSetWindow(thirdPersonWindow);
	glutPostRedisplay();*/

	//'Works' but very buggy
	/*
	SwapBuffers(firstPersonHDC);
	wglMakeCurrent(firstPersonHDC, firstPersonHGLRC);
	FirstPersonDisplayFunc();
	wglShareLists(firstPersonHGLRC, thirdPersonHGLRC);

	SwapBuffers(thirdPersonHDC);
	wglMakeCurrent(thirdPersonHDC, thirdPersonHGLRC);
	ThirdPersonDisplayFunc();*/
	wglMakeCurrent(firstPersonHDC, firstPersonHGLRC);
	FirstPersonDisplayFunc();
	wglShareLists(firstPersonHGLRC, thirdPersonHGLRC);
	SwapBuffers(firstPersonHDC);
	wglMakeCurrent(thirdPersonHDC, thirdPersonHGLRC);
	ThirdPersonDisplayFunc();
	SwapBuffers(thirdPersonHDC);
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
	wglMakeCurrent(firstPersonHDC, firstPersonHGLRC);
	FirstPersonDisplayFunc();
}
void DrawFrustum();

void FirstPersonDisplayFunc()
{
	int elapsed_time = glutGet(GLUT_ELAPSED_TIME);

	//cout << "First person display func" << endl;
	GLReturnedError("Entering DisplayFunc");

	glClearColor(51 / 255.0f, 51 / 255.0f, 51 / 255.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(vFOV, width / (double)height, 3.0, 23);


	glPolygonMode(GL_FRONT_AND_BACK, wireFrameMode ?  GL_FILL : GL_LINE);
	glViewport(0, 0, width, height);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(cameraX, cameraY, cameraZ, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	glRotated(45, 0.0, 1.0, 0.0);
	DrawMany();
	
	/*
	glScaled(.25, .25, .25);
	const unsigned char chars[] = { 'M', 't' };
	glutStrokeString(GLUT_STROKE_ROMAN ,  chars);*/
	wglShareLists(firstPersonHGLRC, thirdPersonHGLRC);
	glutSwapBuffers();
	glFlush();
}

//Third Person Window Functions
void DrawThirdPerson()
{
	glBegin(GL_LINES);
	glColor3d(1., 1.0, 1.0);
	glVertex3d(cameraX, cameraY, cameraZ);
	glVertex3d(-(cameraX)* .75, -(cameraY)*.75, -(cameraZ)*.75);
	glEnd();


	GLUquadricObj* quadric;
	quadric = gluNewQuadric();
	glPushMatrix();
	glTranslated(cameraX, cameraY, cameraZ);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	gluSphere(quadric, .5, 10, 10);
	glPopMatrix();
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glPushMatrix();
	gluQuadricDrawStyle(quadric, GL_LINE);
	glLineWidth(0.1);
	glCullFace(GL_FRONT);
	glColor3d(153/255.0f, 153/255.0f, 153/255.0f);
	gluSphere(quadric, 15, 40, 18);
	DrawFrustum();
	glPopMatrix();
	glCullFace(GL_BACK);
	glPolygonMode(GL_FRONT_AND_BACK, wireFrameMode ? GL_FILL : GL_LINE);
	glRotated(45, 0.0, 1.0, 0.0);
	
	DrawMany();
	gluDeleteQuadric(quadric);
}

void DrawFrustum()
{
	int coordNum = 0;
	for (float x = -1; x <= 1; x += 2)
	{
		for (float y = -1; y <= 1; y += 2)
		{
			for (float z = -1; z <= 1; z +=2)
			{				
				mat4 fpmv = lookAt(cameraCoords, vec3(0.0, 0.0, 0.0), vec3(0.0, 1.0, 0.0));

				mat4 fpp = perspective((double)DegreesToRadians(vFOV), width / (double)height, 3.0, 23.0);
				
				frustumCoordinates[coordNum] = inverse(fpmv) * inverse(fpp) * vec4(x, y, z, 1.0f);

				frustumCoordinates[coordNum].x = frustumCoordinates[coordNum].x / frustumCoordinates[coordNum].w;
				frustumCoordinates[coordNum].y = frustumCoordinates[coordNum].y / frustumCoordinates[coordNum].w;
				frustumCoordinates[coordNum].z = frustumCoordinates[coordNum].z / frustumCoordinates[coordNum].w;				
				coordNum++;
			}
		}
	}

	//Draw Frustum
	glPushMatrix();
	double temp = -12 / frustumCoordinates[1].z;

	//change to GL_QUADS?
	//
	glBegin(GL_LINES);
	//cout << frustumCoordinates[0].x << " " << frustumCoordinates[0].y << " " << frustumCoordinates[0].z << endl;
	//cout << frustumCoordinates[1].x << " " << frustumCoordinates[1].y << " " << frustumCoordinates[1].z << endl;
	glVertex3d(frustumCoordinates[0].x, frustumCoordinates[0].y, frustumCoordinates[0].z);
	glVertex3d(frustumCoordinates[1].x, frustumCoordinates[1].y, frustumCoordinates[1].z);

	glVertex3d(frustumCoordinates[0].x, frustumCoordinates[0].y, frustumCoordinates[0].z);
	glVertex3d(frustumCoordinates[2].x, frustumCoordinates[2].y, frustumCoordinates[2].z);

	glVertex3d(frustumCoordinates[0].x, frustumCoordinates[0].y, frustumCoordinates[0].z);
	glVertex3d(frustumCoordinates[4].x, frustumCoordinates[4].y, frustumCoordinates[4].z);

	glVertex3d(frustumCoordinates[1].x, frustumCoordinates[1].y, frustumCoordinates[1].z);
	glVertex3d(frustumCoordinates[3].x, frustumCoordinates[3].y, frustumCoordinates[3].z);

	glVertex3d(frustumCoordinates[1].x, frustumCoordinates[1].y, frustumCoordinates[1].z);
	glVertex3d(frustumCoordinates[5].x, frustumCoordinates[5].y, frustumCoordinates[5].z);


	glVertex3d(frustumCoordinates[2].x, frustumCoordinates[2].y, frustumCoordinates[2].z);
	glVertex3d(frustumCoordinates[3].x, frustumCoordinates[3].y, frustumCoordinates[3].z);

	glVertex3d(frustumCoordinates[2].x, frustumCoordinates[2].y, frustumCoordinates[2].z);
	glVertex3d(frustumCoordinates[6].x, frustumCoordinates[6].y, frustumCoordinates[6].z);

	glVertex3d(frustumCoordinates[3].x, frustumCoordinates[3].y, frustumCoordinates[3].z);
	glVertex3d(frustumCoordinates[7].x, frustumCoordinates[7].y, frustumCoordinates[7].z);

	glVertex3d(frustumCoordinates[4].x, frustumCoordinates[4].y, frustumCoordinates[4].z);
	glVertex3d(frustumCoordinates[5].x, frustumCoordinates[5].y, frustumCoordinates[5].z);

	glVertex3d(frustumCoordinates[4].x, frustumCoordinates[4].y, frustumCoordinates[4].z);
	glVertex3d(frustumCoordinates[6].x, frustumCoordinates[6].y, frustumCoordinates[6].z);

	glVertex3d(frustumCoordinates[5].x, frustumCoordinates[5].y, frustumCoordinates[5].z);
	glVertex3d(frustumCoordinates[7].x, frustumCoordinates[7].y, frustumCoordinates[7].z);

	glVertex3d(frustumCoordinates[6].x, frustumCoordinates[6].y, frustumCoordinates[6].z);
	glVertex3d(frustumCoordinates[7].x, frustumCoordinates[7].y, frustumCoordinates[7].z);

	glEnd();
	
	//Draw spheres at cornrs
	GLUquadricObj* quadric;
	quadric = gluNewQuadric();
	glColor3d(1.0, 1.0, 1.0);
	for (int vertex = 0; vertex < 8; vertex++)//Go through each vertex in frustumCoordinates
	{
		glPushMatrix();
		glTranslated(frustumCoordinates[vertex].x, frustumCoordinates[vertex].y, frustumCoordinates[vertex].z);//Translate to that vertex
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		gluSphere(quadric, .25, 10, 10);//Draw a sphere
		glPopMatrix();
	}

	gluDeleteQuadric(quadric);

	glPopMatrix();
	
}

void ThirdPersonDisplayFunc()
{
	int elapsed_time = glutGet(GLUT_ELAPSED_TIME);
	GLReturnedError("Entering ThirdPersonDisplayFunc");
	glClearColor(51 / 255.0f, 51 / 255.0f, 51 / 255.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	double hScale = tpWidth / (double)1024;
	double vScale = tpHeight / (double)512;
	double scale = hScale / vScale;
	//cout << "Display" << endl;

	glOrtho(-(25/vScale) * hScale, 25/vScale * hScale,  -(25), 25, 0, 100);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//for text: glutStrokeString()

	glViewport(0, 0, tpWidth/2, tpHeight);
	glPushMatrix();
	gluLookAt(0.0, 0.0, 50.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	
	DrawThirdPerson();

	glPopMatrix();
	glLoadIdentity();
	glViewport(tpWidth / 2, 0, tpWidth / 2, tpHeight);
	gluLookAt(50.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	DrawThirdPerson();
	glutSwapBuffers();
	glFlush();
}

void ThirdPersonReshapeFunc(int w, int h)
{
	tpWidth = w;
	tpHeight = h;

	wglMakeCurrent(thirdPersonHDC, thirdPersonHGLRC);
	ThirdPersonDisplayFunc();
}


void TimerFunc(int period)
{
	glutTimerFunc(period, TimerFunc, period);
	Redisplay();
	/*
	SwapBuffers(firstPersonHDC);
	wglMakeCurrent(firstPersonHDC, firstPersonHGLRC);*/

	/*
	SwapBuffers(firstPersonHDC);
	wglMakeCurrent(firstPersonHDC, firstPersonHGLRC);
	FirstPersonDisplayFunc();
	wglShareLists(firstPersonHGLRC, thirdPersonHGLRC);

	SwapBuffers(thirdPersonHDC);
	wglMakeCurrent(thirdPersonHDC, thirdPersonHGLRC);
	ThirdPersonDisplayFunc();*/

}

void UpdateCamera()
{
	cameraX = 15.0 * cos(DegreesToRadians(xAngle)) * cos(DegreesToRadians(yAngle));
	cameraY = 15.0 * sin(DegreesToRadians(xAngle));
	cameraZ = 15.0 * cos(DegreesToRadians(xAngle)) * sin(DegreesToRadians(yAngle));

	cameraCoords = {cameraX, cameraY, cameraZ};
}

void SpecialFunc(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_PAGE_UP:
		vFOV = vFOV + 1 <= 80 ? vFOV += 1.0 : vFOV = 80;
		break;
	case GLUT_KEY_PAGE_DOWN:
		vFOV = vFOV - 1 >= 10 ? vFOV -= 1.0 : vFOV = 10;
		break;
	case GLUT_KEY_UP:
		xAngle = xAngle < 89 ? xAngle += 1.0 : xAngle = 89;
		UpdateCamera();
	
		//glutPostRedisplay();
		/*
		wglMakeCurrent(firstPersonHDC, firstPersonHGLRC);
		FirstPersonDisplayFunc();
		SwapBuffers(thirdPersonHDC);
		wglShareLists(thirdPersonHGLRC, thirdPersonHGLRC);
		ThirdPersonDisplayFunc();
		SwapBuffers(firstPersonHDC);*/

		break;
	case GLUT_KEY_DOWN:
		xAngle = xAngle > -89 ? xAngle -= 1.0 : xAngle = -89;
		UpdateCamera();

		break;
	case GLUT_KEY_LEFT:
		yAngle += 1.0;
		UpdateCamera();
		break;
	case GLUT_KEY_RIGHT:
		yAngle -= 1.0;
		UpdateCamera();
		break;
	default:
		break;
	}
	Redisplay();
}

void SpecialFunc2(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_PAGE_UP:
		vFOV = vFOV + 1 <= 80 ? vFOV += 1.0 : vFOV = 80;
		//Redisplay();
		break;
	case GLUT_KEY_PAGE_DOWN:
		vFOV = vFOV - 1 >= 10 ? vFOV -= 1.0 : vFOV = 10;
		//Redisplay();
		break;
	case GLUT_KEY_UP:
		xAngle = xAngle < 89 ? xAngle += 1.0 : xAngle = 89;
		UpdateCamera();

		//glutPostRedisplay();
		/*
		wglMakeCurrent(firstPersonHDC, firstPersonHGLRC);
		FirstPersonDisplayFunc();
		SwapBuffers(thirdPersonHDC);
		wglShareLists(thirdPersonHGLRC, thirdPersonHGLRC);
		ThirdPersonDisplayFunc();
		SwapBuffers(firstPersonHDC);
		*/
		//Redisplay();
		break;
	case GLUT_KEY_DOWN:
		xAngle = xAngle > -89 ? xAngle -= 1.0 : xAngle = -89;

		//UpdateCamera();
		//Redisplay();
		break;
	case GLUT_KEY_LEFT:
		yAngle += 1.0;
		//UpdateCamera();
		//Redisplay();
		break;
	case GLUT_KEY_RIGHT:
		yAngle -= 1.0;
		UpdateCamera();
		//glutPostRedisplay();
		//Redisplay();
		break;
	default:
		break;
	}
	Redisplay();
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
		//Redisplay();
		break;
	default:
		break;
	}
	Redisplay();
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

	glutReshapeFunc(FirstPersonReshapeFunc);
	glutKeyboardFunc(KeyboardFunc);
	glutSpecialFunc(SpecialFunc);
	glutDisplayFunc(FirstPersonDisplayFunc);
	glutTimerFunc(1000 / 60, TimerFunc, 1000 / 60);

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