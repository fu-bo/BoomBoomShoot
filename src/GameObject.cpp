#include "GameObject.hpp"
#include "fssimplewindow.h"
#include "MyDraw.hpp"
#include "PngManager.hpp"
#include <vector>
#include <memory>
#include <math.h>

#include "stdio.h"

GameObject::GameObject() {
    png_ptr_ = nullptr;
    Reset();
}

GameObject::GameObject(const GameObject & another) {
    png_ptr_ = nullptr;
	Copy(another);
}

GameObject::~GameObject() {
	CleanUp();
}

GameObject & GameObject::operator=(const GameObject & another) {
	Copy(another);
	return (*this);
}

void GameObject::Reset(){
	png_ptr_ = nullptr;
	scale_ = 1;
    x_ = 0;
    y_ = 0;
    next_x_ = 0;
    next_y_ = 0;
    flip_leftright_ = 1;
    flip_updown_ = 1;
    angle_ = 0;
    moving_angle_ = rand() % 360;
    vx_ = 0;
    vy_ = 0;
    v_ = 20;
    direction_ = 1;
    moving_state_ = 0;
    fire_mode_ = 0;
    bullet_angle_ = 0;
    blocked_ = 0;
    state_ = 0;
    png_id_id_ = 0;
    hp_ = 10;
    hp_max_ = 10;
    png_ptr_ = nullptr;
    png_id_ = 0;
    type_ = -1;
    prev_time_ = 0;
    curr_time_ = 0;
    time_gap_ = 0.2;

    flag_blink_ = false;
    blink_prev_time_ = 0;
	flag_show_ = true;
	blink_time_gap_ = 0.2 * 1.11;
    blink_total_time_gap_ = blink_time_gap_ * 6;
	blink_total_prev_time_ = 0;
	flag_notmove_ = false;
	prev_notmove_time_ = 0;
	notmove_time_gap_ = 0.4;
	flag_notjumpstate_ = false;
	startdisappear_state_ = 0;
	disappear_state_ = 0;
    CleanUp();
}

void GameObject::CleanUp(){
	collision_circle_.clear();
	collision_rect_.clear();
	png_ptr_ = nullptr;
}

void GameObject::Create(){
	CleanUp();
}

void GameObject::Copy(const GameObject & another) {
	if(this == &another){
        return;
    }
    if(another.png_ptr_ == nullptr) {
        Reset();
        return;
    }
    Create();
	scale_ = another.scale_;
    x_ = another.x_;
    y_ = another.y_;
    next_x_ = another.next_x_;
    next_y_ = another.next_y_;
    flip_leftright_ = another.flip_leftright_;
    flip_updown_ = another.flip_updown_;
    angle_ = another.angle_;
    moving_angle_ = another.moving_angle_;
    vx_ = another.vx_;
    vy_ = another.vy_;
    v_ = another.v_;
    state_ = another.state_;
    moving_state_ = another.moving_state_;
    fire_mode_ = another.fire_mode_;
    bullet_angle_ = another.bullet_angle_;
    direction_ = another.direction_;
    blocked_ = another.blocked_;
    png_id_id_ = another.png_id_id_;
    state2pngid_ = another.state2pngid_;
    hp_ = another.hp_;
    hp_max_ = another.hp_max_;
    collision_circle_ = another.collision_circle_;
    collision_rect_ = another.collision_rect_;
    png_ptr_ = another.png_ptr_;
    png_id_ = another.png_id_;
    type_ = another.type_;
    prev_time_ = another.prev_time_;
    curr_time_ = another.curr_time_;

    time_gap_ = another.time_gap_;
    flag_blink_ = another.flag_blink_;
    blink_prev_time_ = another.blink_prev_time_;
	flag_show_ = another.flag_show_;
	blink_time_gap_ = another.blink_time_gap_;
    blink_total_time_gap_ = another.blink_total_time_gap_;
	blink_total_prev_time_ = another.blink_total_prev_time_;
	flag_notmove_ = another.flag_notmove_;
	prev_notmove_time_ = another.prev_notmove_time_;
	notmove_time_gap_ = another.notmove_time_gap_;
	flag_notjumpstate_ = another.flag_notjumpstate_;
	startdisappear_state_ = another.startdisappear_state_;
	disappear_state_ = another.disappear_state_;
    return;
}

bool GameObject::SetPng(std::shared_ptr<PngManager> png_ptr, const std::vector<std::vector<int>>& state2pngid){
	// collision circle
	Create();
	png_ptr_ = png_ptr;
	state2pngid_ = state2pngid;
	return true;
}

bool GameObject::SetPng(std::shared_ptr<PngManager> png_ptr, const std::vector<std::vector<int>>& state2pngid, const std::vector<Circle> collision_circle){
	// collision circle
	Create();
	collision_circle_ = collision_circle;
	png_ptr_ = png_ptr;
	state2pngid_ = state2pngid;
	return true;
}

bool GameObject::SetPng(std::shared_ptr<PngManager> png_ptr, const std::vector<std::vector<int>>& state2pngid, const std::vector<Rect> collision_rect){
	// collision rectangle
	Create();
	collision_rect_ = collision_rect;
	png_ptr_ = png_ptr;
	state2pngid_ = state2pngid;
	return true;
}

bool GameObject::SetPng(std::shared_ptr<PngManager> png_ptr, const std::vector<std::vector<int>>& state2pngid, double x_ratio, double y_ratio, double ratio){
	// collision rectangle
	Create();
	png_ptr_ = png_ptr;
	state2pngid_ = state2pngid;

	collision_rect_.clear();
    double temp_wid = png_ptr->Wid(0) * MyDraw::PIXEL2METER;
    double temp_hei = png_ptr->Hei(0) * MyDraw::PIXEL2METER;
    collision_rect_.push_back(Rect(x_ratio * temp_wid, y_ratio * temp_hei, -ratio * temp_wid, ratio * temp_wid, -ratio * temp_hei, ratio * temp_hei) );
	return true;
}

bool GameObject::SetPng(std::shared_ptr<PngManager> png_ptr, const std::vector<std::vector<int>>& state2pngid, double x_ratio, double y_ratio, double wid_ratio, double hei_ratio){
	// collision rectangle
	Create();
	png_ptr_ = png_ptr;
	state2pngid_ = state2pngid;

	collision_rect_.clear();
	double temp_wid = png_ptr->Wid(0) * MyDraw::PIXEL2METER;
	double temp_hei = png_ptr->Hei(0) * MyDraw::PIXEL2METER;
	collision_rect_.push_back(Rect(x_ratio * temp_wid, y_ratio * temp_hei, -wid_ratio * temp_wid, wid_ratio * temp_wid, -hei_ratio * temp_hei, hei_ratio * temp_hei));
	return true;
}

bool GameObject::SetV(double v){
	v_ = v;
	return true;
}

bool GameObject::SetVxy(double vx, double vy){
	vx_ = vx;
	vy_ = vy;
	return true;
}

bool GameObject::SetScale(double scale){
	scale_ = scale;
	return true;
}

int GameObject::SetHp(double hp, double ratio){
    if(ratio < 0){
        hp_ = hp;
    }
    else{
        hp_ = hp_max_ * ratio;
    }
    if(hp_ > hp_max_){
        hp_ = hp_max_;
    }
    if(hp_ <= 0.1){
        hp_ = 0;
        if(startdisappear_state_ != disappear_state_){
        	SetState(startdisappear_state_, true);
        }
        else{
        	SetState(disappear_state_, true);
        }
    }
    return state_;
}

int GameObject::SetHpMax1(double hp_max){
    hp_max_ = hp_max;
    hp_ = hp_max_;
    return state_;
}

int GameObject::MinusHp(double hp_minus){
    SetHp(hp_ - hp_minus);
    return state_;
}

bool GameObject::SetXY(double x, double y){
	x_ = x;
	y_ = y;
	next_x_ = x;
	next_y_ = y;
	return true;
}

bool GameObject::SetAngle(double angle){
	angle_ = angle;
	return true;
}

bool GameObject::SetFlip(double flip_leftright, double flip_updown){
	flip_leftright_ = flip_leftright;
	flip_updown_ = flip_updown;
	return true;
}

bool GameObject::SetState(int state, bool flag_hardset){
	if(state <= state2pngid_.size()){
		if(!flag_notjumpstate_ || flag_hardset){
			if(state_ != state){
				state_ = state;
				png_id_id_ = 0;
			}
			return true;
		}
		return false;
	}
	else{
		return false;
	}
}

bool GameObject::SetType(int type){
	type_ = type;
	return true;
}

bool GameObject::SetDirection(int flag_direction) {
	direction_ = flag_direction;
	return true;
}

bool GameObject::SetMovingState(int moving_state) {
	moving_state_ = moving_state;
	return true;
}

bool GameObject::SetFireMode(int fire_mode) {
	fire_mode_ = fire_mode;
	return true;
};

bool GameObject::SetBulletAngle(double bullet_angle) {
	bullet_angle_ = bullet_angle;
	return true;
}

bool GameObject::SetBlocked(int new_blocked) {
	blocked_ = new_blocked;
	return true;
}

bool GameObject::SetDisappearState(int startdisappear_state, int disappear_state){
	startdisappear_state_ = startdisappear_state;
	disappear_state_ = disappear_state;
	return true;
}

bool GameObject::SetNotMove(double curr_time){
	flag_notmove_ = true;
	prev_notmove_time_ = curr_time;		
	return true;
}

double GameObject::GetX() {
	return x_;
}

double GameObject::GetY() {
	return y_;
}

bool GameObject::IsInside(double margin) const{
    return ( x_ < MyDraw::WIN_LENGTH_METER + margin && x_ >= 0 - margin && y_ < MyDraw::WIN_HEIGHT_METER + margin && y_ >= 0 - margin);
}

bool GameObject::IsInsideNext(double margin) const{
    return ( next_x_ < MyDraw::WIN_LENGTH_METER + margin && next_x_ >= 0 - margin && next_y_ < MyDraw::WIN_HEIGHT_METER + margin && next_y_ >= 0 - margin);
}

double GameObject::GetMinHp(const GameObject& g1, const GameObject& g2, double hp_minus){
    double hp_min = hp_minus;
    if(g1.hp_ < hp_min){
        hp_min = g1.hp_;
    }
    if(g2.hp_ < hp_min){
        hp_min = g2.hp_;
    }
    return hp_min;
}

bool GameObject::CheckCollision(GameObject& g1, double x2, double y2){
	int flag_next = 0;
	double r2 = 0;
	for(int i = 0; i < g1.collision_circle_.size(); i++){
		double x1, y1, r1;
		g1.GetCollisionCircle(i, x1, y1, r1, flag_next);
		if( (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2) < (r1 + r2) * (r1 + r2) ){
			return true;
		}
	}
	for(int i = 0; i < g1.collision_rect_.size(); i++){
		Rect rect1;
		g1.GetCollisionRect(i, rect1);
		if(Rect::CheckCollision(rect1, x2, y2, r2)){
			return true;
		}
	}
	return false;
}

bool GameObject::CheckCollision(GameObject& g1, GameObject& g2, int flag_next){
	for(int i = 0; i < g1.collision_circle_.size(); i++){
		for(int j = 0; j < g2.collision_circle_.size(); j++){
			double x1, y1, r1;
			double x2, y2, r2;
			g1.GetCollisionCircle(i, x1, y1, r1, flag_next);
			g2.GetCollisionCircle(j, x2, y2, r2, flag_next);
			if( (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2) < (r1 + r2) * (r1 + r2) ){
				return true;
			}
		}
	}
	for(int i = 0; i < g1.collision_rect_.size(); i++){
		for(int j = 0; j < g2.collision_rect_.size(); j++){
			Rect rect1, rect2;
			g1.GetCollisionRect(i, rect1);
			g2.GetCollisionRect(j, rect2);
			if(Rect::CheckCollision(rect1, rect2)){
				return true;
			}
		}
	}
	for(int i = 0; i < g1.collision_rect_.size(); i++){
		for(int j = 0; j < g2.collision_circle_.size(); j++){
			Rect rect1;
			double x2, y2, r2;
			g1.GetCollisionRect(i, rect1);
			g2.GetCollisionCircle(j, x2, y2, r2, flag_next);
			if(Rect::CheckCollision(rect1, x2, y2, r2)){
				return true;
			}
		}
	}
	for(int i = 0; i < g1.collision_circle_.size(); i++){
		for(int j = 0; j < g2.collision_rect_.size(); j++){
			Rect rect2;
			double x1, y1, r1;
			g1.GetCollisionCircle(i, x1, y1, r1, flag_next);
			g2.GetCollisionRect(j, rect2);
			if(Rect::CheckCollision(rect2, x1, y1, r1)){
				return true;
			}
		}
	}
	return false;
}

void GameObject::GetCollisionRect(int rect_id, Rect &rect, int flag_next) const{
    double sin_angle = sin(angle_ * MyDraw::DEG2RAD);
    double cos_angle = cos(angle_ * MyDraw::DEG2RAD);
    double rect_x = collision_rect_[rect_id].x_ * flip_leftright_ * scale_;
    double rect_y = collision_rect_[rect_id].y_ * flip_updown_ * scale_;
    double x, y;
    if(flag_next == 1){
        x = next_x_ + rect_x * cos_angle - rect_y * sin_angle;
        y = next_y_ + rect_x * sin_angle + rect_y * cos_angle;
    }
    else if (flag_next == 0){
        x = x_ + rect_x * cos_angle - rect_y * sin_angle;
        y = y_ + rect_x * sin_angle + rect_y * cos_angle;
    }
    else if (flag_next == 2){
        x = next_x_ + rect_x * cos_angle - rect_y * sin_angle;
        y = y_ + rect_x * sin_angle + rect_y * cos_angle;	
    }
    else if (flag_next == 3){
        x = x_ + rect_x * cos_angle - rect_y * sin_angle;
        y = next_y_ + rect_x * sin_angle + rect_y * cos_angle;
    }
    double rect_angle = collision_rect_[rect_id].angle_ * flip_leftright_ * flip_updown_ + angle_;
    double rect_left = collision_rect_[rect_id].left_ * flip_leftright_ * scale_;
    double rect_right = collision_rect_[rect_id].right_ * flip_leftright_ * scale_;
    double rect_up = collision_rect_[rect_id].up_ * flip_updown_ * scale_;
    double rect_down = collision_rect_[rect_id].down_ * flip_updown_ * scale_;
    rect.Set(x, y, rect_left, rect_right, rect_down, rect_up, rect_angle);
}

void GameObject::GetCollisionCircle(int circle_id, double& x, double& y, double& r, int flag_next) const{
    double sin_angle = sin(angle_ * MyDraw::DEG2RAD);
    double cos_angle = cos(angle_ * MyDraw::DEG2RAD);
    double circle_x = collision_circle_[circle_id].x_ * flip_leftright_ * scale_;
    double circle_y = collision_circle_[circle_id].y_ * flip_updown_ * scale_;
    if(flag_next == 1){
        x = next_x_ + circle_x * cos_angle - circle_y * sin_angle;
        y = next_y_ + circle_x * sin_angle + circle_y * cos_angle;
    }
    else if (flag_next == 0){
        x = x_ + circle_x * cos_angle - circle_y * sin_angle;
        y = y_ + circle_x * sin_angle + circle_y * cos_angle;
    }
    else if (flag_next == 2){
        x = next_x_ + circle_x * cos_angle - circle_y * sin_angle;
        y = y_ + circle_x * sin_angle + circle_y * cos_angle;   
    }
    else if (flag_next == 3){
        x = x_ + circle_x * cos_angle - circle_y * sin_angle;
        y = next_y_ + circle_x * sin_angle + circle_y * cos_angle;
    }
    r = scale_ * collision_circle_[circle_id].r_;
}


void GameObject::DrawCollision() const{
	if(state_ > 0 && flag_show_) {
		Circle::Draw(36, x_, y_, 0.05);
		for(int i = 0; i < collision_circle_.size(); i++){
			double x, y, r;
			GetCollisionCircle(i, x, y, r);
			Circle::Draw(36, x, y, r);
		}
		for(int i = 0; i < collision_rect_.size(); i++){
			Rect rect;
			GetCollisionRect(i, rect);
			rect.Draw();
		}

	}
}

void GameObject::Draw(double offset_x, double offset_y, double alpha) {
	if(state_ > 0 && flag_show_) {
		png_id_ = state2pngid_[state_-1][png_id_id_];
		if(png_id_ < png_ptr_->Num()){
		    double sin_angle = sin(angle_ * MyDraw::DEG2RAD);
		    double cos_angle = cos(angle_ * MyDraw::DEG2RAD);

			double png_wid =  ((double) png_ptr_->Wid(png_id_) ) * MyDraw::PIXEL2METER * scale_ * flip_leftright_;
			double png_hei =  ((double) png_ptr_->Hei(png_id_) ) * MyDraw::PIXEL2METER * scale_ * flip_updown_;
			// left-up
		    double x1 = offset_x + x_ - png_wid/2 * cos_angle + png_hei/2 * (-sin_angle);
		    double y1 = offset_y + y_ - png_wid/2 * sin_angle + png_hei/2 * cos_angle;
		    // right-up
		    double x2 = offset_x + x_ + png_wid/2 * cos_angle + png_hei/2 * (-sin_angle);
		    double y2 = offset_y + y_ + png_wid/2 * sin_angle + png_hei/2 * cos_angle;
		    // right-down
		    double x3 = offset_x + x_ + png_wid/2 * cos_angle - png_hei/2 * (-sin_angle);
		    double y3 = offset_y + y_ + png_wid/2 * sin_angle - png_hei/2 * cos_angle;
		    // left-down
		    double x4 = offset_x + x_ - png_wid/2 * cos_angle - png_hei/2 * (-sin_angle);
		    double y4 = offset_y + y_ - png_wid/2 * sin_angle - png_hei/2 * cos_angle;

		    png_ptr_->Draw(png_id_, x1, y1, x2, y2, x3, y3, x4, y4, alpha);
		}
	}
}

void GameObject::Behave(double curr_time){
	if(state_ > 0){
		curr_time_ = curr_time;
		if(curr_time - prev_time_ > time_gap_) {
			prev_time_ = curr_time;
			png_id_id_++;
			if(png_id_id_ >= state2pngid_[state_-1].size()){
				png_id_id_ = 0;
				flag_notjumpstate_ = false;
				if(state_ == startdisappear_state_){
					SetState(disappear_state_, true); // hard set
				}
			}
		}
		if(flag_blink_){
			if(curr_time - blink_prev_time_ > blink_time_gap_){
				blink_prev_time_ = curr_time;
				flag_show_ = !flag_show_;
			}	
		}
		if(curr_time - blink_total_prev_time_ > blink_total_time_gap_){
			blink_total_prev_time_ = curr_time;
			flag_blink_ = false;
			flag_show_ = true;
		}
		if(curr_time - prev_notmove_time_ > notmove_time_gap_){
			prev_notmove_time_ = curr_time;
			flag_notmove_ = false;
			flag_show_ = true;
		}
		if(!flag_notmove_){
			x_ = next_x_;
			y_ = next_y_;
		}
	}
}

void GameObject::Idle(double curr_time){
	time_gap_ = 0.2;
	if(state_ > 0){
		SetState(1);
	}
}

bool GameObject::Move(double dt, int flag_direction){
	time_gap_ = 0.1;
	if(state_ > 0){
		if(flag_direction == 0){
			next_x_ = x_ + v_ * dt;
			SetState(2);
			flip_leftright_ = 1;
		}
		else if(flag_direction == 1){
			next_x_ = x_ - v_ * dt;
			SetState(2);
			flip_leftright_ = -1;
		}
		else if(flag_direction == 2){
			next_y_ = y_ + v_ * dt;
			SetState(2);
		}
		else if(flag_direction == 3){
			next_y_ = y_ - v_ * dt;
			SetState(2);
		}
		bool flag_inside = IsInsideNext();
		if(!flag_inside){
			next_x_ = x_;
			next_y_ = y_;
		}
		return flag_inside;
	}
	return false;
}

bool GameObject::Move(double dt){
	time_gap_ = 0.2;
	if(state_ > 0){
		SetState(2);
		return IsInside();
	}
	return false;
}

bool GameObject::Move(double player_x, double player_y) {
	double dt = 0.05;
	time_gap_ = 0.1;
	// moving model 0 : move straight forward. When collision happens,
	// turn around
	if (moving_state_ == 0) {
		if (blocked_ == 1) {
			switch (direction_) {
				case 0 : 
					SetDirection(1);
					break;
				case 1 : 
					SetDirection(0);
					break;
				case 2 : 
					SetDirection(3);
					break;
				case 3 : 
					SetDirection(2);
					break;
				default :
					break;
			} 
			SetBlocked(0);
		}
		if(state_ > 0){
			if(direction_ == 0){
				next_x_ = x_ + v_ * dt;
				SetState(2);
				flip_leftright_ = 1;
			}
			else if(direction_ == 1){
				next_x_ = x_ - v_ * dt;
				SetState(2);
				flip_leftright_ = -1;
			}
			else if(direction_ == 2){
				next_y_ = y_ + v_ * dt;
				SetState(2);
			}
			else if(direction_ == 3){
				next_y_ = y_ - v_ * dt;
				SetState(2);
			}
			bool flag_inside = IsInsideNext();
			if(!flag_inside){
				next_x_ = x_;
				next_y_ = y_;
			}
			return flag_inside;
		}
	} else if (moving_state_ == 1) {
		// moving toward the player
		// will not avoid the obstacles
		double dx = player_x - x_;
		double dy = player_y - y_;
		double hypotenuse = pow(dx * dx + dy * dy, 0.5);
		double siny = dy / hypotenuse;
		double sinx = dx / hypotenuse;
		next_x_ = x_ + v_ * dt * sinx;
		next_y_ = y_ + v_ * dt * siny;
	} else if (moving_state_ == 2) {
		SetState(1);
		// moving when health is half
		if (hp_ <= hp_max_ / 2) {
			SetMovingState(1);
		}
	} else if (moving_state_ == 3) {
		// moving to a random direction after one hit obstacle
		if (blocked_ == 1) {
			moving_angle_ = rand() % 360;
			SetBlocked(0);
		} else {
			next_x_ = x_ + v_ * dt * sin(moving_angle_ * 3.14159/180);
			next_y_ = y_ + v_ * dt * cos(moving_angle_ * 3.14159/180);
		}
	}
	if(next_x_ - x_ < 0){
		flip_leftright_ = -1;
	}
	else{
		flip_leftright_ = 1;
	}
	return false;
}

void GameObject::Disappear(){}

void GameObject::ShowUp(double curr_time){}

void GameObject::Stop(double curr_time){
	next_x_ = x_;
	next_y_ = y_;
}

void GameObject::Collide(GameObject& another){}

bool GameObject::NotAttackable() const{
	return flag_blink_;
}

double GameObject::hp() const {
	return hp_;
}

double GameObject::hp_max() const {
	return hp_max_;
}

double GameObject::x() const {
	return x_;
}

double GameObject::y() const {
	return y_;
}

double GameObject::vx() const {
	return vx_;
}

double GameObject::vy() const {
	return vy_;
}

int GameObject::type() const {
	return type_;
}


