#ifndef MYDRAW_HPP
#define MYDRAW_HPP

#include <math.h>

class MyDraw{
public:
    static double METER2PIXEL;
    static double PIXEL2METER;
    static int ORIGIN_X;
    static int ORIGIN_Y;
    static int WIN_HEIGHT;
    static int WIN_LENGTH;
    static double WIN_HEIGHT_METER;
    static double WIN_LENGTH_METER;
    static bool FLIPY;

    static double PI;
    static double DEG2RAD;
    static double RAD2DEG;
    
    static bool myglVertex2d(const double x_m, const double y_m);
    static void setWindow(double wid, double hei);
    static bool Pixel2Meter(int x_p, int y_p, double& x_m, double& y_m);

    static unsigned long int next;
    // Seed the random function
    static void Srand(unsigned int seed);
    // Generate an integer random number
    static int IntRand(void);
    // Generate an double random number between min_value and max_value
    static double DoubleRand(double min_value, double max_value);

    static inline void Rotate(double x, double y, double angle, double& new_x, double& new_y){
        double sin_angle = sin(angle * MyDraw::DEG2RAD);
        double cos_angle = cos(angle * MyDraw::DEG2RAD);
        new_x = x * cos_angle - y * sin_angle;
        new_y = x * sin_angle + y * cos_angle;
    }
};

#endif


