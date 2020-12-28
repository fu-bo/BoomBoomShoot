#ifndef COMPUTER_HPP
#define COMPUTER_HPP

#include "ActiveObject.hpp"
#include "Bullet.hpp"

class Computer: public ActiveObject {
protected:
    int collision_style_;
public:
    bool flag_boss_;
public:
	// constructor
    Computer();
    // copy constructor
    Computer(const Computer & another);
    // destructor
    ~Computer();
    // copy operator
    Computer & operator=(const Computer & another);
    // clean up
    void CleanUp();
    // create based on input value
    void Create();
    // deep copy
    void Copy(const Computer & another);
    // Set the shoot gap
    bool SetShootGap(double gap);
    // Set collision style
    bool SetCollisionStyle(int collision_style);
    // set based on input value
    // void Set();
    // behavior of fire
    // fire_type can be 1, 2, 3, 4 
    bool Fire(double curr_time, double x, double y, std::vector<Bullet>& fire_bullet, const std::vector<Bullet>& sample_bullet);
    bool Fire(double curr_time, double angle, std::vector<Bullet>& fire_bullet, const std::vector<Bullet>& sample_bullet);
    // behavior of dispear
    // void Dispear();
    // behavior of showup
    // void ShowUp();
    // behavior of move, curr_time may not be used
    // boolean Move(double dt, int flag_direction);
    void Collide(double curr_time, GameObject& another);

};

#endif