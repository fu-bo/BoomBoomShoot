#include "GameBar.hpp"
#include "PngManager.hpp"
#include "Circle.hpp"
#include <iostream>


GameBar::GameBar(){

	//player hp
	playerhpstate = 20;
	std::vector<std::string> file_names;
    file_names.push_back("data/image/bar/heart0.png");
	file_names.push_back("data/image/bar/heart25.png");
	file_names.push_back("data/image/bar/heart50.png");
	file_names.push_back("data/image/bar/heart75.png");
	file_names.push_back("data/image/bar/heart100.png");

    std::shared_ptr<PngManager> heart_png = std::make_shared<PngManager>(file_names);

    std::vector<std::vector<int>> heart_state2pngid;
	heart_state2pngid.push_back(std::vector<int>({ 0 }));
	heart_state2pngid.push_back(std::vector<int>({ 1 }));
	heart_state2pngid.push_back(std::vector<int>({ 2 }));
	heart_state2pngid.push_back(std::vector<int>({ 3 }));
	heart_state2pngid.push_back(std::vector<int>({ 4 }));

    std::vector<Circle> heart_circle;
	heart_circle.push_back(Circle(0, 0, 0.1));

    GameObject hp;
    hp.SetPng(heart_png, heart_state2pngid, heart_circle);

	//play hp background
    hp.SetState(1);
    hp.SetScale(2.0);

    for(int i = 0; i < 5; i++){
    	double heart_x = (double) 0.65*i+0.4;
	    hp.SetXY(heart_x, 8.3);
    	hp_bar.push_back(hp);
    	hp_null_bar.push_back(hp);
    }

	// int heartnum = playerhpstate / 4;
	// double halfblood = (playerhpstate - heartnum*4) / 4.0;
	// std::cout << halfblood << '\n';

	// for (int i = 0; i < heartnum; i++)
	// {
	// 	hp.SetState(5);
	// 	hp.SetScale(2.0);

	// 	double heart_x = (double) 0.65*i + 0.4;
	// 	hp.SetXY(heart_x, 8.3);
	// 	hp_bar.push_back(hp);
	// }

	// if (halfblood == 0.25)
	// {
	// 	hp.SetState(2);
	// 	hp.SetScale(2.0);
	// 	double heart_x = (double) 0.65*heartnum + 0.4;
	// 	hp.SetXY(heart_x, 8.3);
	// 	hp_bar.push_back(hp);
	// }
	// else if (halfblood == 0.50)
	// {
	// 	hp.SetState(3);
	// 	hp.SetScale(2.0);
	// 	double heart_x = (double) 0.65*heartnum + 0.4;
	// 	hp.SetXY(heart_x, 8.3);
	// 	hp_bar.push_back(hp);
	// }
	// else if (halfblood == 0.75)
	// {
	// 	hp.SetState(4);
	// 	hp.SetScale(2.0);
	// 	double heart_x = (double) 0.65*heartnum + 0.4;
	// 	hp.SetXY(heart_x, 8.3);
	// 	hp_bar.push_back(hp);
	// }

	//boss hp background
	bossstate = 1;
	bosshpstate = 8;

	if (bossstate == 1)
	{
		file_names.clear();
		std::vector<std::shared_ptr<PngManager>> bosshpbg_png;
		file_names.push_back("data/image/bar/health_bar_decoration.png");
		bosshpbg_png.push_back(std::make_shared<PngManager>(file_names));

		std::vector<std::vector<int>> bosshpbg_state2pngid;
		bosshpbg_state2pngid.push_back(std::vector<int>({ 0 }));

		std::vector<Circle> bosshpbg_circle;
		bosshpbg_circle.push_back(Circle(0, 0, 0.1));

		GameObject bosshpbg;

		for (int i = 0; i < bosshpbg_png.size(); i++)
		{
			bosshpbg.SetPng(bosshpbg_png[i], bosshpbg_state2pngid, bosshpbg_circle);
			bosshpbg.SetState(1);
			bosshpbg.SetScale(3.0);
			bosshpbg.SetXY(6.5, 4.5);

			double bosshpbg_y = 7;
			bosshpbg.SetXY(7, 8.3);
			bosshpbg_bar.push_back(bosshpbg);
		}

		//boss hp


		file_names.clear();
		file_names.push_back("data/image/bar/health_bar_10.png");
		file_names.push_back("data/image/bar/health_bar_20.png");
		file_names.push_back("data/image/bar/health_bar_30.png");
		file_names.push_back("data/image/bar/health_bar_40.png");
		file_names.push_back("data/image/bar/health_bar_50.png");
		file_names.push_back("data/image/bar/health_bar_60.png");
		file_names.push_back("data/image/bar/health_bar_70.png");
		file_names.push_back("data/image/bar/health_bar_80.png");
		file_names.push_back("data/image/bar/health_bar_90.png");
		file_names.push_back("data/image/bar/health_bar.png");


		std::shared_ptr<PngManager> bosshp_png = std::make_shared<PngManager>(file_names);

		std::vector<std::vector<int>> bosshp_state2pngid;
		bosshp_state2pngid.push_back(std::vector<int>({ 0 }));
		bosshp_state2pngid.push_back(std::vector<int>({ 1 }));
		bosshp_state2pngid.push_back(std::vector<int>({ 2 }));
		bosshp_state2pngid.push_back(std::vector<int>({ 3 }));
		bosshp_state2pngid.push_back(std::vector<int>({ 4 }));
		bosshp_state2pngid.push_back(std::vector<int>({ 5 }));
		bosshp_state2pngid.push_back(std::vector<int>({ 6 }));
		bosshp_state2pngid.push_back(std::vector<int>({ 7 }));
		bosshp_state2pngid.push_back(std::vector<int>({ 8 }));
		bosshp_state2pngid.push_back(std::vector<int>({ 9 }));
		bosshp_state2pngid.push_back(std::vector<int>({ 10 }));

		std::vector<Circle> bosshp_circle;
		bosshp_circle.push_back(Circle(0, 0, 0.1));


		GameObject bosshp;
		bosshp.SetPng(bosshp_png, bosshp_state2pngid, bosshp_circle);
		bosshp.SetState(bosshpstate);
		bosshp.SetScale(3.0);

		double bosshp_x = (double) 7.4;
		bosshp.SetXY(bosshp_x, 8.3);
		bosshp_bar.push_back(bosshp);



		//boss skull
		file_names.clear();
		file_names.push_back("data/image/bar/skull.png");

		std::shared_ptr<PngManager> bossskull_png = std::make_shared<PngManager>(file_names);

		std::vector<std::vector<int>> bossskull_state2pngid;
		bossskull_state2pngid.push_back(std::vector<int>({ 0 }));

		std::vector<Circle> bossskull_circle;
		bossskull_circle.push_back(Circle(0, 0, 0.1));

		GameObject bossskull;
		bossskull.SetPng(bossskull_png, bossskull_state2pngid, bossskull_circle);
		bossskull.SetState(1);
		bossskull.SetScale(2.0);

		double bossskull_x = (double)5.57;
		bossskull.SetXY(bossskull_x, 8.4);
		bossskull_bar.push_back(bossskull);
	}

	//small map
	smallmapmatrix = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 1}; //random map

	file_names.clear();
	file_names.push_back("data/image/bar/map1.png");
	file_names.push_back("data/image/bar/map2.png");
	file_names.push_back("data/image/bar/map3r.png");

	std::shared_ptr<PngManager> smallmap_png = std::make_shared<PngManager>(file_names);
	std::vector<std::vector<int>> smallmap_state2pngid;
	smallmap_state2pngid.push_back(std::vector<int>({ 0 }));
	smallmap_state2pngid.push_back(std::vector<int>({ 1 }));
	smallmap_state2pngid.push_back(std::vector<int>({ 2 }));

	std::vector<Circle> smallmap_circle;
	smallmap_circle.push_back(Circle(0, 0, 0.1));

	GameObject smallmap;
	smallmap.SetPng(smallmap_png, smallmap_state2pngid, smallmap_circle);

	for (int i = 0; i < smallmapmatrix.size(); i++)
	{
		double smallmap_x = (double)13.2 + (i % 5)*0.3;
		double smallmap_y = (double)7.5 + (i / 5)*0.3;

		if (smallmapmatrix[i] == 0)
		{
			smallmap.SetState(3);
			smallmap.SetScale(1);
			smallmap.SetXY(smallmap_x, smallmap_y);
			smallmap_bar.push_back(smallmap);
		}

		if (smallmapmatrix[i] == 1)
		{
			smallmap.SetState(1);
			smallmap.SetScale(1);
			smallmap.SetXY(smallmap_x, smallmap_y);
			smallmap_bar.push_back(smallmap);
		}

		if (smallmapmatrix[i] == 2)
		{
			smallmap.SetState(2);
			smallmap.SetScale(1);
			smallmap.SetXY(smallmap_x, smallmap_y);
			smallmap_bar.push_back(smallmap);
		}
	}

	//small map background
	file_names.clear();
	file_names.push_back("data/image/bar/mapback.png");

	std::shared_ptr<PngManager> mapback_png = std::make_shared<PngManager>(file_names);
	std::vector<std::vector<int>> mapback_state2pngid;
	mapback_state2pngid.push_back(std::vector<int>({ 0 }));

	std::vector<Circle> mapback_circle;
	mapback_circle.push_back(Circle(0, 0, 0.1));

	GameObject mapback;
	mapback.SetPng(mapback_png, mapback_state2pngid, mapback_circle);

	mapback.SetState(1);
	mapback.SetScale(0.9);

	double mapback_x = (double)13.8;
	mapback.SetXY(mapback_x, 8.1);
	mapback_bar.push_back(mapback);

}

GameBar::~GameBar(){}

void GameBar::Draw(){

	int heartnum = playerhpstate / 4;
	double halfblood = (playerhpstate - heartnum*4) / 4.0;
	// std::cout << halfblood << '\n';


	int j = 0;
	int blood = 1;
    for(int i = 0; i < playerhpstate; i++){
    	blood++;
    	hp_null_bar[j].Draw(1.0);
    	hp_bar[j].SetState(blood);
    	hp_bar[j].Draw(1.0);
    	if(blood > 4){
    		blood = 1;
    		j++;
    	}
    }


	for (int i = 0; i <mapback_bar.size(); i++) {
		mapback_bar[i].Draw(0.5);
	}

	for (int i = 0; i <smallmap_bar.size(); i++) {
		smallmap_bar[i].SetState(smallmapmatrix[i]);
		smallmap_bar[i].Draw(1.0);
	}

	// printf("%d\n", (int)smallmap_bar.size());

 //    for(int i = 0; i < smallmapmatrix.size(); i++){
 //        printf("%d ", smallmapmatrix[i]);
 //    }
 //    printf("\n");

}