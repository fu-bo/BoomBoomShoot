#ifndef TOOL_HPP
#define TOOL_HPP

#include "GameObject.hpp"
class Player;

class Tool :public GameObject {
protected:
	// If able to be absorbed
	bool is_Absorb;
	// Type of tool
	int tool_id;
	// Floor friction (acceleration)
	// double Acc;

public:
	// Tool Constructor
	Tool();
	// Copy Constructor
	Tool(const Tool & another);
	// destructor
	~Tool();
	// Copy Operator
	Tool & operator=(const Tool & another);
	// clean up
	void CleanUp();
	// deep Copy
	void Copy(const Tool & another);
	// Set tool type
	bool SetToolId(int type);
	// Behavior based on type
	// void Behave(double curr_time);
	// behavior of stay in one position, curr_time may not be used
	// void Idle(double curr_time);
	// behavior of dispear, curr_time may not be used
	void Disappear(double curr_time, Player& player);
	// behavior of showup, curr_time may not be used
	void ShowUp(double curr_time, GameObject& another);
	// behavior of collide with other thigns, for example Stop() if another.type_ == TYPE_OBSTACLE
	/*void Collide(GameObject& another);
	// behavior of move
	bool Move(double dt, int flag_direction);
	bool Move(double dt);*/
};

#endif