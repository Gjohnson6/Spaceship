#pragma once
#include "GL\freeglut.h"
class Ship
{
public:
	Ship();
	Ship(double x, double y, double z);
	~Ship();
	void DrawShip();
	GLuint GetHandler();
private:
	void DrawBody();
	void DrawThruster();
	GLuint ship_handler;
	double xPos = 0.0;
	double yPos = 0.0;
	double zPos = 0.0;
};

