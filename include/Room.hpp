#ifndef ROOM_HPP
#define ROOM_HPP

#include <vector>
#include <stdio.h>

class Game;

class Room
{
protected:
	void Load(FILE *fp);
	int Obj_idx[256];
	const int wid = 13, hei = 7;
public:
	std::vector<int> enemy_id;
	std::vector<int> obstacle_id;
	std::vector<int> tile_id;
	std::vector<int> tool_id;
	std::vector<int> door_id;

	int enemy_num;

	Room();
	~Room();
	void InitializeObs(Game& game, FILE *fp);
	void InitializeTool(Game& game, FILE *fp);
	
};


#endif

