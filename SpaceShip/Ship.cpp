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
	glScaled(1.0, 3.0, 1.0);
	glBegin(GL_QUADS);
	glColor3d(0.0, 0.0, 1.0);

	GLUquadricObj* quadric;
	quadric = gluNewQuadric();

	gluQuadricDrawStyle(quadric, GLU_FILL);
	gluSphere(quadric, 1, 40, 25);
	glEnd();
}