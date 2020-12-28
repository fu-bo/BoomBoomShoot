#ifndef CIRCLE_HPP
#define CIRCLE_HPP

class Circle
{
public:
	double x_;
	double y_;
	double r_;
	Circle();
	Circle(double x, double y, double r);
	~Circle();
	void Set(double x, double y, double r);
	static void Draw(int num, double x, double y, double r);
};

#endif