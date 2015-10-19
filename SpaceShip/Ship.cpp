#include "Ship.h"
#include "GL\freeglut.h"

Ship::Ship()
{
	ship_handler = GLuint(-1);
}

Ship::Ship(double x, double y, double z)
{
	ship_handler = GLuint(-1);
	xPos = x;
	yPos = y;
	zPos = z;
}


Ship::~Ship()
{
	glDeleteLists(ship_handler, 1);
}

void Ship::DrawShip()
{
	if (ship_handler == -1)
	{
		ship_handler = glGenLists(1);
		glNewList(ship_handler, GL_COMPILE);
		glTranslated(xPos, yPos, zPos);
		glPushMatrix();
		DrawBody();
		glPopMatrix();

		glPushMatrix();
		glTranslated(1.25, -1, 0.0);
		DrawThruster();
		glPopMatrix();

		glPushMatrix();
		glTranslated(-1.25, -1, 0.0);
		glRotated(180, 0.0, 1.0, 0.0);
		DrawThruster();
		glPopMatrix();

		glPushMatrix();
		glTranslated(0.0, -1, 1.25);
		glRotated(90, 0.0, -1.0, 0.0);
		DrawThruster();
		glPopMatrix();

		glPushMatrix();
		glTranslated(0.0, -1, -1.25);
		glRotated(90, 0.0, 1.0, 0.0);
		DrawThruster();
		glPopMatrix();
		glEndList();
	}
	glCallList(ship_handler);
}

void Ship::DrawBody()
{
	glTranslated(0.0, .5, 0.0);
	glScaled(.75, 2.5, .75);
	glColor3d(1.0, 0.0, 0.0);

	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, blue);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, white );

	GLUquadricObj* quadric;
	quadric = gluNewQuadric();

	gluQuadricDrawStyle(quadric, GLU_FILL);
	gluSphere(quadric, 1, 40, 50);

	gluDeleteQuadric(quadric);
}

void Ship::DrawThruster()
{
	glRotated(90.0, 1.0, 0.0, 0.0);
	glColor3d(0.0, 0.0, 1.0);
	GLUquadricObj* quadric = gluNewQuadric();

	gluQuadricDrawStyle(quadric, GLU_FILL);
	gluSphere(quadric, .25, 30, 30);

	glColor3d(1.0, 1.0, .0);
	gluCylinder(quadric, .25, .25, 1.0, 30, 30);

	glColor3d(0.0, 1.0, 1.0);
	glTranslated(0.0, 0.0, 1.0);
	glScaled(1.0, 1.0, 1.75);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,red);

	gluSphere(quadric, .25, 30, 30);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, blue);

	glColor3d(1.0, 20 / 255.0, 14 / 255.0);
	glRotated(90, 0.0, -1.0, 0.0);
	glTranslated(-.35, 0.0, 0.0);
	glScaled(1.0, .5, 1.0);
	gluCylinder(quadric, .1, .3, 1.25, 30, 30);

	gluDeleteQuadric(quadric);
}

GLuint Ship::GetHandler()
{
	return ship_handler;
}