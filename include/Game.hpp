
#ifndef GAME_HPP
#define GAME_HPP

#include "GameObject.hpp"
#include "ActiveObject.hpp"
#include "Player.hpp"
#include "Computer.hpp"
#include "Bullet.hpp"
#include "Tool.hpp"
#include "Room.hpp"
#include "Menu.hpp"
#include "yspng.h"
#include "fssimplewindow.h"
#include "MyDraw.hpp"
#include "Circle.hpp"
#include "GameBar.hpp"
#include "PngManager.hpp"
#include <vector>
#include <string.h>
#include <memory>
#include "yssimplesound.h"

class Game
{
public:
	Player player;
	std::vector<Computer> enemy;
	std::vector<Computer> obstacle;
	std::vector<Computer> tile;
	std::vector<Bullet> player_bullet;
	std::vector<Bullet> enemy_bullet;
	std::vector<Tool> tool;
	std::vector<Computer> door;
	Menu menu;
	GameBar bar;
	std::vector<int> room_status;
	int boss_num;


    YsSoundPlayer::SoundData sound_list[18];
    YsSoundPlayer sound_player;

	// PngManager background_png = std::make_shared<PngManager>({"data/image/background/Dungeon1.png"});

	std::vector<Computer> enemy_list;
	std::vector<Computer> obstacle_list;
	std::vector<Computer> tile_list;
	std::vector<Bullet> bullet_list;
	std::vector<Tool> tool_list;
	std::vector<Computer> door_list;

	std::vector<Room> room;
	int map_x;
	int map_y;
	int room_id;
	int room_x;
	int room_y;
	int next_id;
	double room_offset_x;
	double room_offset_y;
	double next_offset_x;
	double next_offset_y;
	int flag_switch_direction;
	bool flag_runmenu;

	bool terminate;
	long long int start_time;
	double curr_time;

	bool flag_player_not_check_collision_;

	Game();
	~Game();
	void Initialize();
	void OpenWindow();
	void Run();
	void enemyAct(double curr_time, std::vector<Bullet>& bullet_list);
	void DrawRoom();

	inline bool Terminate() {return terminate;}
	inline int GetRoomId(int x, int y){
		if(x < 0 || x >= map_x || y < 0 || y >= map_y){
			return -1;
		}
		return (x + y * map_x);
	}
	void RunRoom();
	bool SwitchRoom(int dir);
	void RunMenu();
};

#endif