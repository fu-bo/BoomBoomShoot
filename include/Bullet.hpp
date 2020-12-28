#ifndef BULLET_HPP
#define BULLET_HPP

#include "GameObject.hpp"

class Bullet: public GameObject {
protected:
	// int bullet id
	int bullet_id;
	// the moment the bullet is fired
	double fire_time;
	// the fired position x
	double fire_x;
	// the fired position y
	double fire_y;
	// attack
	double attack;
	// the range of the bullet
	double range;
public:
	// Bullet Constructor
	Bullet();
	// Copy Constructor
	Bullet(const Bullet & another);
	// destructor
	~Bullet();
	// Copy Operator
	Bullet & operator=(const Bullet & another);
	// clean up
	void CleanUp();
	// create
	void Create();
	// deep Copy
	void Copy(const Bullet & another);
	// set the values of the fields
	void Set();
	// check if the bullet collid with sth
	// bool CheckCollision(const GameObject & g);
	// the action that the bullet explodes
	// Check collision first
	// Check if it is out of range then
	// Call disappear in the end
	void Explode();
	// fly forward
	bool Move(double dt);
	// set bullet id
	void SetBulletId(int bullet_id);
	// set bullet range
	void SetRange(double range);
	// set shoot state
	bool Shoot(double curr_time, double x, double y, double angle, const Bullet & another);
	// return bullet_id
	inline int GetBulletId() const {return bullet_id;}
	// behavior of collision
	void Collide(double curr_time, GameObject& another);


private:
	// disappear
	void Disappear();
};

#endif