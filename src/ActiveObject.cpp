#include "ActiveObject.hpp"

ActiveObject::ActiveObject(){
	shoot_gap_ = 0.2;
	prev_shoot_time_ = 0;
	bullet_id_ = -1;
}

ActiveObject::ActiveObject(const ActiveObject & another): GameObject(another){
	shoot_gap_ = another.shoot_gap_;
	prev_shoot_time_ = another.prev_shoot_time_;
	bullet_id_ = another.bullet_id_;
}

ActiveObject & ActiveObject::operator=(const ActiveObject & another){
	Copy(another);
	return (*this);
}

ActiveObject::~ActiveObject(){
	CleanUp();
}

void ActiveObject::Copy(const ActiveObject & another){
	GameObject::Copy(another);
	shoot_gap_ = another.shoot_gap_;
	prev_shoot_time_ = another.prev_shoot_time_;
	bullet_id_ = another.bullet_id_;
}

void ActiveObject::CleanUp(){
	GameObject::CleanUp();
}

void ActiveObject::Create(){
	GameObject::Create();
}


void ActiveObject::Set(){}

// void ActiveObject::Fire(){}

void ActiveObject::SetBullet(int bullet_id){
	bullet_id_ = bullet_id;
}
