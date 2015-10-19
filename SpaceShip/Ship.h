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
	GLfloat shininess[1] = { 100.f };
	GLfloat blue[4] = { 50 / 255.f, 50 / 255.f, 125 / 255.f, 1.f };
	GLfloat white[4] = { .5f, .5f, .5f, 1.f };
	GLfloat red[4] = { 135 / 255.0f, 50 / 255.f, 50 / 255.0f, 1.0f };
};

