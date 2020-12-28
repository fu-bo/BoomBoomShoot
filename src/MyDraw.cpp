#include "MyDraw.hpp"
#include "fssimplewindow.h"
#include <stdio.h>
#include <stdlib.h>

bool MyDraw::FLIPY = true;
double MyDraw::METER2PIXEL = 50.0;
double MyDraw::PIXEL2METER = 1/MyDraw::METER2PIXEL;
int MyDraw::ORIGIN_X = 0;
int MyDraw::ORIGIN_Y = 50 * 9;
int MyDraw::WIN_HEIGHT = 50 * 9;
int MyDraw::WIN_LENGTH = 50 * 15;
double MyDraw::WIN_HEIGHT_METER = ((double) MyDraw::WIN_HEIGHT) * MyDraw::PIXEL2METER;
double MyDraw::WIN_LENGTH_METER = ((double) MyDraw::WIN_LENGTH) * MyDraw::PIXEL2METER;

double MyDraw::PI = 3.141592653589;
double MyDraw::DEG2RAD = MyDraw::PI/180;
double MyDraw::RAD2DEG = 180/MyDraw::PI;

bool MyDraw::myglVertex2d(const double x_m, const double y_m){
    double x_p = METER2PIXEL * x_m + (double)ORIGIN_X;
    double y_p;
    if(FLIPY){
        y_p = -METER2PIXEL * y_m + (double)ORIGIN_Y;
    }
    else{
        y_p = METER2PIXEL * y_m + (double)ORIGIN_Y;
    }
    glVertex2d(x_p, y_p);
    return ( (int)(x_p + 0.5) < WIN_LENGTH && (int)(x_p + 0.5) >= 0 && (int)(y_p + 0.5) < WIN_HEIGHT && (int)(y_p + 0.5) >= 0);
}

void MyDraw::setWindow(double wid, double hei){
    WIN_HEIGHT_METER = hei;
    WIN_LENGTH_METER = wid;
    WIN_HEIGHT = (int) (METER2PIXEL * hei + 0.5);
    WIN_LENGTH = (int) (METER2PIXEL * wid + 0.5);
}

bool MyDraw::Pixel2Meter(int x_p, int y_p, double& x_m, double& y_m){
    x_m = ((double) (x_p - ORIGIN_X)) * PIXEL2METER;
    if(FLIPY){
        y_m = - ((double) (y_p - ORIGIN_Y)) * PIXEL2METER;
    }
    else{
        y_m = ((double) (y_p - ORIGIN_Y)) * PIXEL2METER;
    }
    return ( x_p >= 0 && x_p < WIN_LENGTH && y_p >= 0 && y_p < WIN_HEIGHT );
}

unsigned long int MyDraw::next = 1;
int MyDraw::IntRand(void) // RAND_MAX assumed to be 32767
{
    next = next * 1103515245 + 12345;
    return (unsigned int)(next/65536) % 32768;
}

void MyDraw::Srand(unsigned int seed)
{
    next = seed;
}

double MyDraw::DoubleRand(double min_value, double max_value)
{
    double rand_value = ( ((double)(IntRand() % 32768)) / 32767) * (max_value - min_value) + min_value;
    return rand_value;
}
