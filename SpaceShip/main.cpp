#include "GL\freeglut.h"
#include "Ship.h"


GLuint firstPersonWindow;
GLuint thirdPersonWindow;

int width = 512;
int height = 512;
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
void KeyboardFunc(unsigned char c, int x, int y)
{
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

void DisplayFunc()
{
	int elapsed_time = glutGet(GLUT_ELAPSED_TIME);

	glClearColor(160 / 255.0f, 32 / 255.0f, 240 / 255.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(50.0, width / (double)height, 1.0, 10.0);

	gluLookAt(0.0, 0.0, 4.5, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	glViewport(0, 0, width, height);

	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	Ship ship = Ship();
	ship.drawShip();

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
	glutDisplayFunc(DisplayFunc);
	glutMainLoop();

}