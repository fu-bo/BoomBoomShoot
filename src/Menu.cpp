#include "Menu.hpp"

#include "PngManager.hpp"
#include <vector>
#include <string.h>
#include <memory>

Menu::Menu(){

	std::vector<std::string> file_names;
    std::vector<std::vector<int>> state2pngid;

    // Resume
    file_names.clear();    
    file_names.push_back("data/image/menu/resume1.png");
    file_names.push_back("data/image/menu/resume2.png");

    std::shared_ptr<PngManager> resume_png = std::make_shared<PngManager>(file_names);

    state2pngid.clear();
    state2pngid.push_back(std::vector<int>({0}));
    state2pngid.push_back(std::vector<int>({1}));

    resume_button.SetPng(resume_png, state2pngid, 0, 0, 0.2, 0.2);
    resume_button.SetState(1);
    resume_button.SetScale(0.3);
    resume_button.SetXY(7.5, 5.0);

    // Restart
    file_names.clear();    
    file_names.push_back("data/image/menu/restart1.png");
    file_names.push_back("data/image/menu/restart2.png");

    std::shared_ptr<PngManager> restart_png = std::make_shared<PngManager>(file_names);

    state2pngid.clear();
    state2pngid.push_back(std::vector<int>({0}));
    state2pngid.push_back(std::vector<int>({1}));

    restart_button.SetPng(restart_png, state2pngid, 0, 0, 0.2, 0.2);
    restart_button.SetState(1);
    restart_button.SetScale(0.3);
    restart_button.SetXY(7.5, 3.75);

    // Exit
    file_names.clear();    
    file_names.push_back("data/image/menu/exit1.png");
    file_names.push_back("data/image/menu/exit2.png");

    std::shared_ptr<PngManager> exit_png = std::make_shared<PngManager>(file_names);

    state2pngid.clear();
    state2pngid.push_back(std::vector<int>({0}));
    state2pngid.push_back(std::vector<int>({1}));

    exit_button.SetPng(exit_png, state2pngid, 0, 0, 0.15, 0.15);
    exit_button.SetState(1);
    exit_button.SetScale(0.3);
    exit_button.SetXY(7.5, 2.5);

    // Exit
    file_names.clear();    
    file_names.push_back("data/image/menu/gameover.png");
    file_names.push_back("data/image/menu/victory.png");

    std::shared_ptr<PngManager> win_png = std::make_shared<PngManager>(file_names);

    state2pngid.clear();
    state2pngid.push_back(std::vector<int>({0}));
    state2pngid.push_back(std::vector<int>({1}));

    win_button.SetPng(win_png, state2pngid);
    win_button.SetState(1);
    win_button.SetScale(0.3);
    win_button.SetXY(7.5, 6.0);

    // Background
    file_names.clear();    
    file_names.push_back("data/image/menu/background.png");

    std::shared_ptr<PngManager> background_png = std::make_shared<PngManager>(file_names);

    state2pngid.clear();
    state2pngid.push_back(std::vector<int>({0}));

    background.SetPng(background_png, state2pngid, 0, 0, 0.5, 0.5);
    background.SetState(1);
    background.SetScale(0.7);
    background.SetXY(7.5, 4.5);
}

Menu::~Menu(){}

void Menu::Draw(){
    background.Draw();
	resume_button.Draw();
	restart_button.Draw();
	exit_button.Draw();
    win_button.Draw();

    // resume_button.DrawCollision();
    // restart_button.DrawCollision();
    // exit_button.DrawCollision();

}