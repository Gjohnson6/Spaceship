#include "Ship.h"
#include "GL\freeglut.h"

Ship::Ship()
{
}


Ship::~Ship()
{
}

void Ship::drawShip()
{
	glPushMatrix();
	drawBody();
	glPopMatrix();

	glPushMatrix();
	glTranslated(1.5, -1, 0.0);
	drawThruster();
	glPopMatrix();

	glPushMatrix();
	glTranslated(-1.5, -1, 0.0);
	glRotated(180, 0.0, 1.0, 0.0);
	drawThruster();
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.0, -1, 1.5);
	glRotated(90, 0.0, -1.0, 0.0);
	drawThruster();
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.0, -1, -1.5);
	glRotated(90, 0.0, 1.0, 0.0);
	drawThruster();
	glPopMatrix();


}

void Ship::drawBody()
{
	glScaled(1.0, 2.5, 1.0);
	glBegin(GL_QUADS);
	glColor3d(1.0, 0.0, 0.0);

	GLUquadricObj* quadric;
	quadric = gluNewQuadric();

	glScaled(1.0, 1.0, 2.0);
	gluQuadricDrawStyle(quadric, GLU_FILL);
	gluSphere(quadric, 1, 40, 50);
	glEnd();

	gluDeleteQuadric(quadric);
}

void Ship::drawThruster()
{
	glRotated(90.0, 1.0, 0.0, 0.0);
	glBegin(GL_QUADS);
	glColor3d(0.0, 0.0, 1.0);
	glCullFace(GL_FRONT);
	GLUquadricObj* quadric = gluNewQuadric();

	gluQuadricDrawStyle(quadric, GLU_FILL);
	gluSphere(quadric, .25, 30, 30);

	glTranslated(0.0, 0.0, 0.0);

	glColor3d(1.0, 1.0, .0);
	gluCylinder(quadric, .25, .25, 1.0, 30, 30);

	glColor3d(0.0, 1.0, 1.0);
	glTranslated(0.0, 0.0, 1.0);
	glScaled(1.0, 1.0, 1.5);
	gluSphere(quadric, .25, 30, 30);


	glColor3d(1.0, 20 / 255.0, 147 / 255.0);
	glRotated(90, 0.0, -1.0, 0.0);
	glTranslated(-.5, 0.0, 0.0);
	glScaled(1.0, .5, 1.0);
	gluCylinder(quadric, .1, .3, 1.5, 30, 30);
	glEnd();

	gluDeleteQuadric(quadric);
}