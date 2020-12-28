#include "Rect.hpp"
#include "MyDraw.hpp"
#include <math.h>
#include "fssimplewindow.h"
#include <stdio.h>

Rect::Rect(double x, double y, double left, double right, double down, double up, double angle){
	x_ = x;
	y_ = y;
	if(left < right){
		left_ = left;
		right_ = right;
	}
	else{
		right_ = left;
		left_ = right;
	}
	if(down < up){
		up_ = up;
		down_ = down;
	}
	else{
		up_ = down;
		down_ = up;
	}
	angle_ = angle;
}

Rect::Rect(){}

Rect::~Rect(){}

void Rect::Set(double x, double y, double left, double right, double down, double up, double angle){
	x_ = x;
	y_ = y;
	if(left < right){
		left_ = left;
		right_ = right;
	}
	else{
		right_ = left;
		left_ = right;
	}
	if(down < up){
		up_ = up;
		down_ = down;
	}
	else{
		up_ = down;
		down_ = up;
	}
	angle_ = angle;
}

bool Rect::CheckCollision(const Rect& r1, const Rect& r2){
	double x1[4];
	double y1[4];
	MyDraw::Rotate(r1.left_, r1.down_, r1.angle_, x1[0], y1[0]);
	MyDraw::Rotate(r1.left_, r1.up_, r1.angle_, x1[1], y1[1]);
	MyDraw::Rotate(r1.right_, r1.up_, r1.angle_, x1[2], y1[2]);
	MyDraw::Rotate(r1.right_, r1.down_, r1.angle_, x1[3], y1[3]);
	double x2[4];
	double y2[4];
	MyDraw::Rotate(r2.left_, r2.down_, r2.angle_, x2[0], y2[0]);
	MyDraw::Rotate(r2.left_, r2.up_, r2.angle_, x2[1], y2[1]);
	MyDraw::Rotate(r2.right_, r2.up_, r2.angle_, x2[2], y2[2]);
	MyDraw::Rotate(r2.right_, r2.down_, r2.angle_, x2[3], y2[3]);

	double minus_x = r2.x_ - r1.x_;
	double minus_y = r2.y_ - r1.y_;

	double x1_aligned[4];
	double y1_aligned[4];
	double x2_aligned[4];
	double y2_aligned[4];
	for(int i= 0; i < 4; i++){
		MyDraw::Rotate(minus_x + x2[i], minus_y + y2[i], -r1.angle_, x2_aligned[i], y2_aligned[i]);
		MyDraw::Rotate(-minus_x + x1[i], -minus_y + y1[i], -r2.angle_, x1_aligned[i], y1_aligned[i]);
		if( (x2_aligned[i] > r1.left_) && (x2_aligned[i] < r1.right_) && (y2_aligned[i] > r1.down_) && (y2_aligned[i] < r1.up_) ){
			return true;
		}
		if( (x1_aligned[i] > r2.left_) && (x1_aligned[i] < r2.right_) && (y1_aligned[i] > r2.down_) && (y1_aligned[i] < r2.up_) ){
			return true;
		}
	}
	return false;
}

bool Rect::CheckCollision(const Rect& r1, double x, double y, double r){
	double x_aligned, y_aligned;
	MyDraw::Rotate(x - r1.x_, y - r1.y_, -r1.angle_, x_aligned, y_aligned);
	if( (x_aligned > r1.left_ - r) && (x_aligned < r1.right_ + r) && (y_aligned > r1.down_) && (y_aligned < r1.up_) ){
		return true;
	}
	if( (x_aligned > r1.left_) && (x_aligned < r1.right_) && (y_aligned > r1.down_ - r) && (y_aligned < r1.up_ + r) ){
		return true;
	}
	double dr = r * r;
	double dx_left = (x_aligned - r1.left_) * (x_aligned - r1.left_);
	double dx_right = (x_aligned - r1.right_) * (x_aligned - r1.right_);
	double dy_down = (y_aligned - r1.down_) * (y_aligned - r1.down_);
	double dy_up = (y_aligned - r1.up_) * (y_aligned - r1.up_);
	if ( (dx_left + dy_down < dr) || (dx_left + dy_up < dr) || (dx_right + dy_down < dr) || (dx_right + dy_up < dr) ){
		return true;
	}
	return false;
}

void Rect::Draw() const{
	double x[4];
	double y[4];
	MyDraw::Rotate(left_, down_, angle_, x[0], y[0]);
	MyDraw::Rotate(left_, up_, angle_, x[1], y[1]);
	MyDraw::Rotate(right_, up_, angle_, x[2], y[2]);
	MyDraw::Rotate(right_, down_, angle_, x[3], y[3]);
	// Draw line
	glColor3ub(0, 0, 0);
	glBegin(GL_LINE_LOOP);
	for(int i = 0; i < 4; i++){
		MyDraw::myglVertex2d(x_ + x[i], y_ + y[i]);
	}
	glEnd();
	// Draw color
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glColor4ub(255, 255, 255, 100);
	glBegin(GL_QUADS);
	for(int i = 0; i < 4; i++){
		MyDraw::myglVertex2d(x_ + x[i], y_ + y[i]);
	}
	glEnd();
	glDisable(GL_BLEND);
}


