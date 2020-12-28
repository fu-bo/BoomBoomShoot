#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "ActiveObject.hpp"
#include "Bullet.hpp"

class Player: public ActiveObject {
public:
	// constructor
    Player();
    // copy constructor
    Player(const Player & another);
    // destructor
    ~Player();
    // copy operator
    Player & operator=(const Player & another);
    // clean up
    void CleanUp();
    // create based on input value
    void Create();
    // deep copy
    void Copy(const Player & another);
    // set based on input value
    // void Set();
    // behavior of fire
    bool Fire(double curr_time, double x, double y, std::vector<Bullet>& fire_bullet, const std::vector<Bullet>& sample_bullet);
    bool Fire(double curr_time, double angle, std::vector<Bullet>& fire_bullet, const std::vector<Bullet>& sample_bullet);
    // behavior of dispear
    // void Dispear();
    // behavior of showup
    // void ShowUp();
    // behavior of move, curr_time may not be used
    // void Move(double dt, int flag_direction);
    // behavior of collision
    void Collide(double curr_time, GameObject& another);
};

#endif