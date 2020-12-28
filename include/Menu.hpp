#ifndef MENU_HPP
#define MENU_HPP

#include "GameObject.hpp"

class Menu
{
public:
	GameObject background;
	GameObject resume_button;
	GameObject restart_button;
	GameObject exit_button;
	GameObject win_button;
	void Run();
	void Draw();
	Menu();
	~Menu();
};

#endif