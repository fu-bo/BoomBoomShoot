#include "Player.hpp"

#include <stdio.h>

Player::Player(){
	type_ = 0;
}

Player::Player(const Player & another): ActiveObject(another){}

Player & Player::operator=(const Player & another){
	Copy(another);
	return (*this);
}

Player::~Player(){
	CleanUp();
}

void Player::Copy(const Player & another){
	ActiveObject::Copy(another);
}

void Player::CleanUp(){
	ActiveObject::CleanUp();
}

void Player::Create(){
	ActiveObject::Create();
}

// void Player::Set(){}

bool Player::Fire(double curr_time, double angle, std::vector<Bullet>& fire_bullet, const std::vector<Bullet>& sample_bullet){
	// printf("%d, %.2f, %.2f, %.2f, %.2f\n", state(), curr_time - prev_shoot_time_, curr_time, prev_shoot_time_, shoot_gap_);
	if(state() > 0 && curr_time - prev_shoot_time_ > shoot_gap_){
		for(int i = 0; i < fire_bullet.size(); i++){
			if(fire_bullet[i].Shoot(curr_time, x_ + MyDraw::DoubleRand(-0.3, 0.3), y_ + + MyDraw::DoubleRand(-0.3, 0.3), angle + MyDraw::DoubleRand(-5, 5), sample_bullet[bullet_id_])){
				prev_shoot_time_ = curr_time;
				return true;
			}
		}
	}
	return false;
}

bool Player::Fire(double curr_time, double x, double y, std::vector<Bullet>& fire_bullet, const std::vector<Bullet>& sample_bullet){
	if(state() > 0 && curr_time - prev_shoot_time_ > shoot_gap_){
		double angle = atan2(y - y_, x - x_) * MyDraw::RAD2DEG;
		angle =  ( (double) ((int) (( (angle + 360) / 45) + 0.5)) ) * 45;
		return Fire(curr_time, angle, fire_bullet, sample_bullet);
		
	}
	return false;
}

void Player::Collide(double curr_time, GameObject& another){
	switch(another.type()) {
	    case 1: { // Enemy
			flag_notmove_ = true;
			flag_blink_ = true;
			blink_total_prev_time_ = curr_time;
			prev_notmove_time_ = curr_time;	    	
	    	break;
	    }
	    case 2: { // Obstacle


	    	next_x_ = x_;
	    	next_y_ = y_;
	    	break;
	    }
	    case 3: { // Bullet
			flag_notmove_ = true;
			flag_blink_ = true;
			blink_total_prev_time_ = curr_time;
			prev_notmove_time_ = curr_time;	  
	    	break;
	    }
	    default:{
	    	break;
	    }
	}

}