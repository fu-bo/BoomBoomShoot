#ifndef GAMEBAR_HPP
#define GAMEBAR_HPP

#include <vector>
#include "GameObject.hpp"

class GameBar
{
protected:
	std::vector<GameObject> hp_bar;
	std::vector<GameObject> hp_null_bar;
	std::vector<GameObject> bosshpbg_bar;
	std::vector<GameObject> bosshp_bar;
	std::vector<GameObject> bossskull_bar;
	std::vector<GameObject> smallmap_bar;
	std::vector<GameObject> mapback_bar;

public:
	int bossstate;
	int bosshpstate;
	int playerhpstate;
	std::vector<int> smallmapmatrix;
	GameBar();
	~GameBar();
	void Draw();
};

#endif