#include "Computer.hpp"

#include <stdio.h>

Computer::Computer(){
	type_ = 1;
	collision_style_ = 0;
	flag_boss_ = false;
}

Computer::Computer(const Computer & another): ActiveObject(another){
	collision_style_ = another.collision_style_;
    flag_boss_ = another.flag_boss_;
}

Computer & Computer::operator=(const Computer & another){
    Copy(another);
    return (*this);
}

Computer::~Computer(){
    CleanUp();
}

void Computer::Copy(const Computer & another){
    ActiveObject::Copy(another);
    collision_style_ = another.collision_style_;
    flag_boss_ = another.flag_boss_;
}

void Computer::CleanUp(){
    ActiveObject::CleanUp();
}

void Computer::Create(){
    ActiveObject::Create();
}

bool Computer::SetShootGap(double gap) {
	shoot_gap_ = gap;
	return true;
}

bool Computer::SetCollisionStyle(int collision_style){
	collision_style_ = collision_style;
	return true;
}

bool Computer::Fire(double curr_time, double angle, std::vector<Bullet>& fire_bullet, const std::vector<Bullet>& sample_bullet){
	bool flag_fire = false;
	if (bullet_id_ == -1) return flag_fire;
	if (fire_mode_ == 0) {
		// fire toward the given angle
		if(state() > 0 && curr_time - prev_shoot_time_ > shoot_gap_) {
			for(int i = 0; i < fire_bullet.size(); i++){
				if(fire_bullet[i].Shoot(curr_time, x_ + MyDraw::DoubleRand(-0.3, 0.3), y_ + + MyDraw::DoubleRand(-0.3, 0.3), angle + MyDraw::DoubleRand(-5, 5), sample_bullet[bullet_id_])){
					prev_shoot_time_ = curr_time;
					flag_fire = true;
					break;
				}
			}			
		}
	} else if (fire_mode_ == 1) {
		// fire like a clock
		angle = bullet_angle_ > 360 ? bullet_angle_ - 360 : bullet_angle_;
		SetBulletAngle(angle + 20);
		if(state() > 0 && curr_time - prev_shoot_time_ > shoot_gap_) {
			for(int i = 0; i < fire_bullet.size(); i++){
				if(fire_bullet[i].Shoot(curr_time, x_ + MyDraw::DoubleRand(-0.3, 0.3), y_ + + MyDraw::DoubleRand(-0.3, 0.3), angle + MyDraw::DoubleRand(-5, 5), sample_bullet[bullet_id_])){
					prev_shoot_time_ = curr_time;
					flag_fire = true;
					break;
				}
			}			
		}
	} else if (fire_mode_ == 2) {
		// fire 2 bullets with contradictory direction
		int shoot_angle[2];
		int shoot_time = 0;
		int shoot_limit = 2;
		shoot_angle[0] = angle;
		shoot_angle[1] = angle + 180;
		if(state() > 0 && curr_time - prev_shoot_time_ > shoot_gap_) {
			for(int i = 0; i < fire_bullet.size(); i++){
				int cur_angle = shoot_angle[shoot_time];
				if(fire_bullet[i].Shoot(curr_time, x_ + MyDraw::DoubleRand(-0.3, 0.3), y_ + + MyDraw::DoubleRand(-0.3, 0.3), cur_angle + MyDraw::DoubleRand(-5, 5), sample_bullet[bullet_id_])){
					shoot_time++;
					if(shoot_time >= shoot_limit){
						flag_fire = true;
						break;
					}
				}
			}
			prev_shoot_time_ = curr_time;
		}
	} else if (fire_mode_ == 3) {
		// fire a circle
		int shoot_angle[12];
		int shoot_time = 0;
		int shoot_limit = 12;
		angle = 0;
		for (int i = 1; i < 12; i++) {
			angle += 30;
			shoot_angle[i] = angle;
		}
		if(state() > 0 && curr_time - prev_shoot_time_ > shoot_gap_) {
			for(int i = 0; i < fire_bullet.size(); i++){
				int cur_angle = shoot_angle[shoot_time];
				if(fire_bullet[i].Shoot(curr_time, x_ + MyDraw::DoubleRand(-0.3, 0.3), y_ + + MyDraw::DoubleRand(-0.3, 0.3), cur_angle + MyDraw::DoubleRand(-5, 5), sample_bullet[bullet_id_])){
					shoot_time++;
					if(shoot_time >= shoot_limit){
						flag_fire = true;
						break;
					}
				}
			}
			prev_shoot_time_ = curr_time;
		}
	} 
	return flag_fire;
}

bool Computer::Fire(double curr_time, double x, double y, std::vector<Bullet>& fire_bullet, const std::vector<Bullet>& sample_bullet){
	if(state() > 0 && curr_time - prev_shoot_time_ > shoot_gap_){
		double angle = atan2(y - y_, x - x_) * MyDraw::RAD2DEG;
		angle =  ( (double) ((int) (( (angle + 360) / 45) + 0.5)) ) * 45;
		return Fire(curr_time, angle, fire_bullet, sample_bullet);
	}
	return false;
}

void Computer::Collide(double curr_time, GameObject& another){
	switch(another.type()) {
	    case 0: { // Enemy
	    	SetState(2);
	    	flag_notjumpstate_ = true;	
	    	break;
	    }
	    case 2: { // Obstacle
	    	next_x_ = x_;
	    	next_y_ = y_;
	    	// Set that it is blocked
	    	SetBlocked(1);
	    	break;
	    }
	    case 3: { // Bullet
	    	notmove_time_gap_ = 0.1;
	    	SetState(2);
	    	flag_notjumpstate_ = true;
	    	if(collision_style_ == 0){
		    	SetNotMove(curr_time);	    		
	    	}
	    	break;
	    }
	    default:{
	    	break;
	    }
	}

}