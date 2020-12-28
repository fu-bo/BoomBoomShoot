#include "Bullet.hpp"
#include <math.h>
#include "MyDraw.hpp"

Bullet::Bullet(){
	type_ = 3;
	bullet_id = -1;
	attack = 1;
	range = 3;
	fire_time = 0;
	fire_x = 0;
	fire_y = 0;
}

Bullet::Bullet(const Bullet & another): GameObject(another){
	type_ = 3;
	bullet_id = another.bullet_id;
	attack = another.attack;
	range = another.range;
	fire_time = another.fire_time;
	fire_x = another.fire_x;
	fire_y = another.fire_y;
}

Bullet & Bullet::operator=(const Bullet & another){
	Copy(another);
	return (*this);
}

Bullet::~Bullet(){
	CleanUp();
}

void Bullet::Copy(const Bullet & another){
	GameObject::Copy(another);
	bullet_id = another.bullet_id;
	attack = another.attack;
	fire_time = another.fire_time;
	range = another.range;
	fire_x = another.fire_x;
	fire_y = another.fire_y;
}

void Bullet::CleanUp(){
	GameObject::CleanUp();
}

void Bullet::Create(){
	GameObject::Create();
}

bool Bullet::Move(double dt){
	time_gap_ = 0.1;
	if(state() > 0){
		// angle_ = atan2(vy_, vx_) * MyDraw::RAD2DEG;
		next_x_ = x_ + vx_ * dt;
		next_y_ = y_ + vy_ * dt;
		bool flag_outofrange = (x_ - fire_x) * (x_ - fire_x) + (y_ - fire_y) * (y_ - fire_y) > range;
	    if(!IsInside(2) || flag_outofrange){
	    	vx_ = 0;
	    	vy_ = 0;
	        SetState(startdisappear_state_);
	        return false;
	    }
	    return true;
	}
	return false;
}

void Bullet::SetBulletId(int bullet_id){
	this->bullet_id = bullet_id;
}

void Bullet::SetRange(double range){
	this->range = range;
}

bool Bullet::Shoot(double curr_time, double x, double y, double angle, const Bullet & another){
    bool flag_shoot = false;
    if(this->state() == 0){
    	if(this->bullet_id != another.bullet_id){
    		Copy(another);
    	}
    	if(cos(angle * MyDraw::DEG2RAD) > 0){
    		this->flip_updown_ = 1;
    	}
    	else{
    		this->flip_updown_ = -1;
    	}
        SetState(1);
    	this->fire_time = curr_time;
		this->fire_x = x;
		this->fire_y = y;
        this->x_ = x;
        this->y_ = y;
        this->angle_ = angle;
        this->vx_ = v_ * cos(angle * MyDraw::DEG2RAD);
        this->vy_ = v_ * sin(angle * MyDraw::DEG2RAD);
        this->hp_ = hp_max_;
        flag_shoot = true;
    }
    return flag_shoot;
}

void Bullet::Collide(double curr_time, GameObject& another){
	switch(another.type()) {
	    case 1: { // Enemy
	    	vx_ = 0;
	    	vy_ = 0;  	
	    	break;
	    }
	    case 2: { // Obstacle
	    	vx_ = 0;
	    	vy_ = 0;
	    }
	    default:{
	    	break;
	    }
	}

}

void Bullet::Set(){}

void Bullet::Explode(){}

void Bullet::Disappear(){}



