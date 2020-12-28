#include <stdlib.h>
#include <stdio.h>
#include "Tool.hpp"
#include "Player.hpp"

Tool::Tool() {
	is_Absorb = 0;
	tool_id = 0; // 0. blood; 1. cat; 2. cup; 3. rocket 
}
Tool::Tool(const Tool & another) : GameObject(another) {
	tool_id = another.tool_id;
	is_Absorb = another.is_Absorb;
}

Tool & Tool::operator=(const Tool & another) {
	Copy(another);
	return (*this);
}

Tool::~Tool() {
	CleanUp();
}

void Tool::Copy(const Tool & another) {
	GameObject::Copy(another);
	tool_id = another.tool_id;
	is_Absorb = another.is_Absorb;
}

void Tool::CleanUp() {
	GameObject::CleanUp();
}

/*void Tool::Idle(double curr_time) {
	time_gap_ = 0.2;
	if (state() > 0) {
		SetState(1);
	}
}*/

bool Tool::SetToolId(int tool_id){
	this->tool_id = tool_id;
	return true;
}


void Tool::ShowUp(double curr_time, GameObject& another) {
	SetXY(another.x(), another.y());
	SetState(1);
}

void Tool::Disappear(double curr_time, Player& player) {
	if (tool_id == 0) {
		if(player.hp() < player.hp_max() - 0.4){
			player.MinusHp(-4);
			SetState(disappear_state_);
		}
	}
	else {
		SetState(disappear_state_);
		player.SetBullet(tool_id - 1);
	}
}

