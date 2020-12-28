#ifndef RECT_HPP
#define RECT_HPP

class Rect
{
public:
	double x_;
	double y_;
	double up_;
	double down_;
	double left_;
	double right_;
	double angle_;
	Rect();
	Rect(double x, double y, double left, double right, double up, double down, double angle = 0);
	~Rect();
	void Set(double x, double y, double left, double right, double up, double down, double angle = 0);
	static bool CheckCollision(const Rect& r1, const Rect& r2);
	static bool CheckCollision(const Rect& r1, double x, double y, double r);
	void Draw() const;
};

#endif