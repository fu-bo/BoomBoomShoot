#ifndef ACTIVEOBJECT_HPP
#define ACTIVEOBJECT_HPP

#include "GameObject.hpp"

class ActiveObject: public GameObject {
protected:
	// previous shooting time
	double prev_shoot_time_;
	// shoot gap time
	double shoot_gap_;
	// bullet id
    int bullet_id_;
public:
	// constructor
    ActiveObject();
    // copy constructor
    ActiveObject(const ActiveObject & another);
    // destructor
    ~ActiveObject();
    // copy operator
    ActiveObject & operator=(const ActiveObject & another);
    // clean up
    void CleanUp();
    // create based on input value
    void Create();
    // deep copy
    void Copy(const ActiveObject & another);
    // set based on input value
    void Set();
    // get bullet_id_
    inline int bullet_id() {return bullet_id_;}
    // function of fireing, call Shoot() inside generate a shooting pattern
    // virtual void Fire();
    // set bullet id
    void SetBullet(int bullet_id);
};

#endif