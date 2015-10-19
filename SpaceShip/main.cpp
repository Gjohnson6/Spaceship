#include "GL\freeglut.h"
#include "Ship.h"
#include "glm\glm.hpp"
#include "glm\gtc\constants.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\matrix_inverse.hpp"
#include "glm\detail\func_geometric.hpp"
#include <iostream>
#include <string>

using namespace std;
using namespace glm;


GLuint firstPersonWindow;
GLuint thirdPersonWindow;
HDC firstPersonHDC;
HDC thirdPersonHDC;
HGLRC firstPersonHGLRC;
HGLRC thirdPersonHGLRC;

Ship ships[16];

int width = 512;//Width of the first person window in pixels
int height = 512;//Height of the first person window in pixels
int tpWidth = 1024;//Width of the third person window in pixels
int tpHeight = 512;//Height of the third person window in pixels
int lightPosition = 1;//Determines where the light is positioned.
double vFOV = 50;//Vertical field of view in the first person window
float xAngle = 0.0;//Angle of the camera around the X axis
float yAngle  = 90.0;//Angle of camera around the Y axis
float lightAngle = 90.0;//Angle of the orbiting light around the Y axis
bool wireFrameMode = true;//Boolean to determine if the models are rendered as wireframes
bool lighting = true; //Boolean to determine if lighting is on or off
bool flatShading = false;//Boolean to determine if shading is flat or smooth
string lightPositionNames[6] = { "Camera", "Center", "X Axis", "Y Axis", "Z Axis", "Orbiting" };

double cameraX = 0.0;//Camera's x position
double cameraY = 0.0;//Camera's Y position
double cameraZ = 15.0;//Camera's Z position

vec4 frustumCoordinates[8];//Coordinates of the corners of the frustum

void FirstPersonDisplayFunc();
void ThirdPersonDisplayFunc();

//Converts Degrees to radians
inline float DegreesToRadians(float degrees)
{
	return degrees / 180.0f * pi<float>();
}

//Displays a string on screen
inline void DisplayString(string stringToDisplay)
{
	const unsigned char* cstring = (const unsigned char*)stringToDisplay.c_str();

	glutStrokeString(GLUT_STROKE_ROMAN, cstring);
}

//Updates both the first person and third person windows
void Redisplay()
{
	wglMakeCurrent(firstPersonHDC, firstPersonHGLRC);
	FirstPersonDisplayFunc();
	wglShareLists(firstPersonHGLRC, thirdPersonHGLRC);
	SwapBuffers(firstPersonHDC);
	wglMakeCurrent(thirdPersonHDC, thirdPersonHGLRC);
	ThirdPersonDisplayFunc();
	SwapBuffers(thirdPersonHDC);
}

//Outputs any errors
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

//Draws the rockets in a grid pattern
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

//Finds the frustum coordinates of the first person window's camera and draws the frustum
void DrawFrustum()
{
	int coordNum = 0;
	for (float x = -1; x <= 1; x += 2)
	{
		for (float y = -1; y <= 1; y += 2)
		{
			for (float z = -1; z <= 1; z +=2)
			{				
				mat4 fpmv = lookAt(vec3(cameraX, cameraY, cameraZ), vec3(0.0, 0.0, 0.0), vec3(0.0, 1.0, 0.0));

				mat4 fpp = perspective((float)DegreesToRadians((float)vFOV), width / (float)height, 3.0f, 22.5f);
				
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
	glBegin(GL_LINES);

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
	
	//Draw spheres at corners
	GLUquadricObj* quadric;
	quadric = gluNewQuadric();
	glColor3d(1.0, 1.0, 1.0);
	for (int vertex = 0; vertex < 8; vertex++)//Go through each vertex in frustumCoordinates
	{
		glPushMatrix();
		glTranslated(frustumCoordinates[vertex].x, frustumCoordinates[vertex].y, frustumCoordinates[vertex].z);//Translate to that vertex
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		gluSphere(quadric, .1, 10, 10);//Draw a sphere
		glPopMatrix();
	}

	gluDeleteQuadric(quadric);
	glPopMatrix();	
}

void orbitLight(GLfloat* lightPos)
{
	lightAngle = (lightAngle + (360 / (1000/60.f) / 100.f));//Takes 10 seconds to complete an orbit. Divided by 60 to account for the fact that this should be called 60 times a second
	lightPos[0] = float(15.0 * cos(DegreesToRadians(0)) * cos(DegreesToRadians(lightAngle)));
	lightPos[2] = float(15.0 * cos(DegreesToRadians(0)) * sin(DegreesToRadians(lightAngle)));
}

void configureLighting()
{
	if (lighting)
	{
		GLfloat ambientColor[] = { 0.2f, 0.2f, 0.2f, 1.0f };
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);
		GLfloat lightPos[4] = { 0, 0, 0, 1 };

		switch (lightPosition)
		{
		case 1://Camera
			lightPos[0] = float(cameraX);
			lightPos[1] = float(cameraY);
			lightPos[2] = float(cameraZ);
			lightAngle = 90.f;
			break;
		case 2://Center
			lightPos[0] = 0;
			lightPos[1] = 0;
			lightPos[2] = 0;
			lightAngle = 90.f;
			break;
		case 3://x
		case 4://y
		case 5://z
			lightPos[lightPosition - 3] = 15;
			lightPos[(lightPosition - 2) % 3] = 0;
			lightPos[(lightPosition - 1) % 3] = 0;
			lightAngle = 90.f;
			break;
		case 6://Orbiting
			orbitLight(lightPos);
			break;
		default:
			break;
		}
		GLfloat lightAmb[] = { 0.f, 0.f, 0.f, 1.f };
		GLfloat lightDif[] = { 1.f, 1.f, 1.f, 1.f };
		GLfloat lightSpec[] = {1.f, 1.f, 1.f, 1.f };
		GLfloat lightSpot[] = { 0 };
		glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
		glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDif);
		glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpec);
		glLightfv(GL_LIGHT0, GL_SPOT_EXPONENT, lightSpot);
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);

		glShadeModel(flatShading ? GL_FLAT : GL_SMOOTH);
		
	}
	else
	{
		glDisable(GL_LIGHTING);
		glDisable(GL_LIGHT0);
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

void FirstPersonDisplayFunc()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	GLReturnedError("Entering DisplayFunc");
	glClearColor(51 / 255.0f, 51 / 255.0f, 51 / 255.0f, 1.0f);

	//Draw Rockets
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(vFOV, width / (double)height, 3.0, 22.5);

	glPolygonMode(GL_FRONT_AND_BACK, wireFrameMode ?  GL_FILL : GL_LINE);
	glViewport(0, 0, width, height);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_NORMALIZE);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(cameraX, cameraY, cameraZ, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	
	configureLighting();
	glRotated(45, 0.0, 1.0, 0.0);

	DrawMany();	
	
	//Draw Text
	glDisable(GL_LIGHTING);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	glOrtho(0, width, 0, height, 0, 100);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPushMatrix();
	glColor3d(1.0, 1.0, 1.0);
	glRasterPos2i(20, 20);
	glTranslated(5.0, 140.0, 0.0);
	glRasterPos2d(2.0, 2.0);
	glScaled(.12, .12, .12);
	DisplayString("l - toggles lighting\n1-6 - changes light position Current: " + lightPositionNames[lightPosition - 1] + "\nf - toggles flat shading\nw - toggles wireframe \nx - exits\nLeft / Right; Up / Down; Page Up / Page Down\n\n");
	glScaled(2.5, 2.5, 2.5);
	DisplayString("Perspective View");
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	wglShareLists(firstPersonHGLRC, thirdPersonHGLRC);
	glutSwapBuffers();
	glFlush();
}

//Third Person Window Functions
void DrawThirdPerson()
{
	glPolygonMode(GL_FRONT_AND_BACK, wireFrameMode ? GL_FILL : GL_LINE);
	configureLighting();
	glRotated(45, 0.0, 1.0, 0.0);
	DrawMany();
	glDisable(GL_LIGHTING);//Disable lighting so it doesn't affect anything but the rockets
	glRotated(-45, 0.0, 1.0, 0.0);
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
	glLineWidth(0.1f);
	glCullFace(GL_FRONT);
	glColor3d(153/255.0f, 153/255.0f, 153/255.0f);
	gluSphere(quadric, 15, 40, 20);
	glPopMatrix();
	glCullFace(GL_BACK);
	DrawFrustum();



	gluDeleteQuadric(quadric);
}

void ThirdPersonDisplayFunc()
{
	GLReturnedError("Entering ThirdPersonDisplayFunc");
	glClearColor(51 / 255.0f, 51 / 255.0f, 51 / 255.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	double hScale = tpWidth / (double)1024;
	double vScale = tpHeight / (double)512;

	glOrtho(-(25/vScale) * hScale, 25/vScale * hScale,  -(25), 25, 0, 100);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_NORMALIZE);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//Z Axis Perspective
	glViewport(0, 0, tpWidth/2, tpHeight);
	glPushMatrix();
	gluLookAt(0.0, 0.0, 20.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	
	DrawThirdPerson();
	
	glPopMatrix();

	//Draw Text
	glPushMatrix();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(0, tpWidth/2, 0, tpHeight, 0, 100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glColor3d(1.0, 1.0, 1.0);
	glRasterPos2i(20, 20);
	glTranslated(0, 5.0, 0.0);
	glScaled(.15, .15, .15);
	DisplayString("Z Axis View");
	glPopMatrix();
	
	//X Axis Perspective
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-(25 / vScale) * hScale, 25 / vScale * hScale, -(25), 25, 0, 100);
	glMatrixMode(GL_MODELVIEW);

	glViewport(tpWidth / 2, 0, tpWidth / 2, tpHeight);
	gluLookAt(20.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	DrawThirdPerson();

	//Draw Text
	glPushMatrix();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(0, tpWidth / 2, 0, tpHeight, 0, 100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glColor3d(1.0, 1.0, 1.0);
	glRasterPos2i(20, 20);
	glTranslated(0, 5.0, 0.0);
	glScaled(.15, .15, .15);
	DisplayString("X Axis View");
	glPopMatrix();


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
}

//Update camera's position based on the x and y angles
void UpdateCamera()
{
	cameraX = 15.0 * cos(DegreesToRadians(xAngle)) * cos(DegreesToRadians(yAngle));
	cameraY = 15.0 * sin(DegreesToRadians(xAngle));
	cameraZ = 15.0 * cos(DegreesToRadians(xAngle)) * sin(DegreesToRadians(yAngle));
}

//Handle special keys
void SpecialFunc(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_PAGE_UP:
		vFOV = vFOV < 80 ? vFOV += 1.0 : vFOV = 80;//Limit vFOV to <= 80 degrees
		break;
	case GLUT_KEY_PAGE_DOWN:
		vFOV = vFOV > 10 ? vFOV -= 1.0 : vFOV = 10;//Limit vFOV to >= 10 degrees
		break;
	case GLUT_KEY_UP:
		xAngle = xAngle < 89 ? xAngle += 1.0 : xAngle = 89;//Limit xAngle rotation to <= 89 degrees
		UpdateCamera();
		break;
	case GLUT_KEY_DOWN:
		xAngle = xAngle > -89 ? xAngle -= 1.0 : xAngle = -89;//Limit xAngle rotation to >= -89 degrees
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

//Handle alphanumeric keys
void KeyboardFunc(unsigned char c, int x, int y)
{
	switch (c)
	{
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
		lightPosition = (int)c - 48;
		break;

	case 27:
	case 'x':
		glutLeaveMainLoop();
		break;
	case 'l':
		lighting = !lighting;
		break;
	case 'f':
		flatShading = !flatShading;
		break;
	case 'w':
		wireFrameMode = !wireFrameMode;
		break;
	default:
		break;
	}
	Redisplay();
}

void MouseFunc(int x, int y, int z, int w)//Fixes flickering issues when clicking on whatever window isn't active
{
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
	glutMouseFunc(MouseFunc);
	glutTimerFunc(1000 / 144, TimerFunc, 1000 / 144);

	glutInitWindowPosition(width, height);
	glutInitWindowSize(tpWidth, tpHeight);
	thirdPersonWindow = glutCreateWindow("Third Person");
	thirdPersonHDC = wglGetCurrentDC();
	thirdPersonHGLRC = wglCreateContext(firstPersonHDC);

	glutReshapeFunc(ThirdPersonReshapeFunc);
	glutKeyboardFunc(KeyboardFunc);
	glutSpecialFunc(SpecialFunc);
	glutMouseFunc(MouseFunc);
	glutDisplayFunc(ThirdPersonDisplayFunc);

	glutMainLoop();
}