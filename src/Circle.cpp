#include "Circle.hpp"
#include "fssimplewindow.h"
#include "MyDraw.hpp"
#include <math.h>

Circle::Circle() {
	x_ = 0;
	y_ = 0;
	r_ = 0;
}

/*
para x coodinate
para y coodinate
para r radius
*/
Circle::Circle(double x, double y, double r) {
	x_ = x;
	y_ = y;
	r_ = r;
}

Circle::~Circle(){}

/*
para x coodinate
para y coodinate
para r radius
*/
void Circle::Set(double x, double y, double r) {
	x_ = x;
	y_ = y;
	r_ = r;
}

void Circle::Draw(int num, double x, double y, double r){
	// Draw line
	glColor3ub(0, 0, 0);
	glBegin(GL_LINE_LOOP);
	double d_angle = 2 * MyDraw::PI / ((double)num);
	for(double angle = 0; angle < 2*MyDraw::PI; angle += d_angle){
	    MyDraw::myglVertex2d(x + r * cos(angle), y + r * sin(angle));
	}
	glEnd();
	// Draw color
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glColor4ub(255, 255, 255, 100);
	glBegin(GL_TRIANGLE_FAN);
	for(double angle = 0; angle < 2*MyDraw::PI; angle += d_angle){
	    MyDraw::myglVertex2d(x + r * cos(angle), y + r * sin(angle));
	}
	glEnd();
	glDisable(GL_BLEND);
}

