#include "Room.hpp"
#include <vector>
#include "Computer.hpp"
#include "Bullet.hpp"
#include "Tool.hpp"
#include "Game.hpp"
#include <time.h>

Room::Room(){
    this->enemy_num = 0;
}

Room::~Room() {}

double margin_x = 0.5, margin_y = 0.5;

void Room::Load(FILE *fp) {
    char str[255];
    char *pix = new char[wid * hei + 1];
    if (nullptr != fgets(str, 255, fp))
    {
        for (int i = 0; i < hei; ++i)
        {
            if (nullptr != fgets(str, 255, fp))
            {
                for (int j = 0; j < strlen(str); ++j)
                {
                    pix[wid*i + j] = str[j];
                }
            }
        }
    }
    for (int i = 0; i < hei; ++i) {
        for (int j = 0; j < wid; ++j) {
            int idx_pix = wid * i + j;
            int idx_obj = wid * (hei - i - 1) + j;
            if (pix[idx_pix] >= '0' && pix[idx_pix] <= '9') {
                Obj_idx[idx_obj] = pix[idx_pix] - '0' - 1;
            }
            else {
                Obj_idx[idx_obj] = pix[idx_pix] - '0' - 8;
            }
        }
    }
    delete[] pix;
}

void Room::InitializeObs(Game& game, FILE *fp){
    std::vector<Computer>& tile = game.tile;
    std::vector<Computer>& enemy = game.enemy;
    std::vector<Computer>& obstacle = game.obstacle;
    std::vector<Computer>& door = game.door;

    std::vector<Computer>& enemy_list = game.enemy_list;
    std::vector<Computer>& obstacle_list = game.obstacle_list;
    std::vector<Computer>& door_list = game.door_list;
    std::vector<Computer>& tile_list = game.tile_list;

    int tile_id2map[9 * 15];
    for (int i = 0; i < 9 * 15; ++i) {
        tile_id2map[i] = MyDraw::IntRand() % 5;
    }
    for (int i = 0; i < 9 * 15; i++) {
        tile.push_back(tile_list[tile_id2map[i]]);
        double temp_y = margin_y + (double)(i / 15);
        double temp_x = margin_x + double(i % 15);
        tile.back().SetXY(temp_x, temp_y);
        tile.back().SetState(1);
        tile.back().SetHpMax1(10000);
        this->tile_id.push_back(tile.size() - 1);
    }
    // Door
    // left
    door.push_back(door_list[0]);
    door.back().SetXY(0.5, 4.5);
    door.back().SetState(1);
    door.back().SetAngle(90);
    door.back().SetHpMax1(10000);
    this->door_id.push_back(door.size() - 1);
    // right
    door.push_back(door_list[0]);
    door.back().SetXY(14.5, 4.5);
    door.back().SetState(1);
    door.back().SetAngle(270);
    door.back().SetHpMax1(10000);
    this->door_id.push_back(door.size() - 1);
    // down
    door.push_back(door_list[0]);
    door.back().SetXY(7.5, 0.5);
    door.back().SetState(1);
    door.back().SetHpMax1(10000);
    this->door_id.push_back(door.size() - 1);
    // up
    door.push_back(door_list[0]);
    door.back().SetXY(7.5, 8.5);
    door.back().SetState(1);
    door.back().SetHpMax1(10000);
    this->door_id.push_back(door.size() - 1);

    int hole_idx[] = { 3,4 };
    int banner_idx[] = { 5,6,8,9 };
    int weapon_idx[] = { 1,2,12,13 };
    for (int i = 1; i < 14; i++) {
        if (i == 7) {
            continue;
        }
        else {
            obstacle.push_back(obstacle_list[6]);
            double temp_x = margin_x + (double)i;
            obstacle.back().SetXY(temp_x, 0.5);
            obstacle.back().SetState(1);
            obstacle.back().SetHpMax1(10000);
            this->obstacle_id.push_back(obstacle.size() - 1);
        }
        
    }
    for (int i = 1; i < 14; i++) {
        if (i == 7) {
            continue;
        }
        else {
            obstacle.push_back(obstacle_list[6]);
            double temp_x = margin_x + (double)i;
            obstacle.back().SetXY(temp_x, 8.5);
            obstacle.back().SetState(1);
            obstacle.back().SetHpMax1(10000);
            this->obstacle_id.push_back(obstacle.size() - 1);
        }
    }
    for (int i = 0; i < 9; i++) {
        if (i == 4) {
            continue;
        }
        else {
            obstacle.push_back(obstacle_list[7]);
            double temp_y = margin_y + (double)i;
            obstacle.back().SetXY(0.5, temp_y);
            obstacle.back().SetState(1);
            obstacle.back().SetHpMax1(10000);
            this->obstacle_id.push_back(obstacle.size() - 1);
        }
        
    }
    for (int i = 0; i < 9; i++) {
        if (i == 4) {
            continue;
        }
        else {
            obstacle.push_back(obstacle_list[8]);
            double temp_y = margin_y + (double)i;
            obstacle.back().SetXY(14.5, temp_y);
            obstacle.back().SetState(1);
            obstacle.back().SetHpMax1(10000);
            this->obstacle_id.push_back(obstacle.size() - 1);
        }

    }
    for (int i : hole_idx) {
        obstacle.push_back(obstacle_list[9]);
        double temp_x = margin_x + (double)i;
        obstacle.back().SetXY(temp_x, 8.5);
        obstacle.back().SetState(1);
        obstacle.back().SetHpMax1(10000);
        this->obstacle_id.push_back(obstacle.size() - 1);
    }
    for (int i : banner_idx) {
        int banner_col = MyDraw::IntRand() % 3 + 10;
        obstacle.push_back(obstacle_list[banner_col]);
        double temp_x = margin_x + (double)i;
        obstacle.back().SetXY(temp_x, 8.5);
        obstacle.back().SetState(1);
        obstacle.back().SetHpMax1(10000);
        this->obstacle_id.push_back(obstacle.size() - 1);
    }
    for (int i : weapon_idx) {
        int weapon_type = MyDraw::IntRand() % 2 + 13;
        obstacle.push_back(obstacle_list[weapon_type]);
        double temp_x = margin_x + (double)i;
        obstacle.back().SetXY(temp_x, 8.5);
        obstacle.back().SetState(1);
        obstacle.back().SetHpMax1(10000);
        this->obstacle_id.push_back(obstacle.size() - 1);
    }

    if (fp == NULL)
    {
        printf("Fail to open file.\n");
    }
    else
    {
        Load(fp);
    }

    for (int i = 0; i < 7 * 13; ++i) {
        if (Obj_idx[i] >= 0) {
            if (Obj_idx[i] >= 5) {
                enemy.push_back(enemy_list[Obj_idx[i] - 5]);
                double temp_y = margin_y + 8 - double(i / 13 + 1);
                double temp_x = margin_x + double(i % 13 + 1);
                enemy.back().SetXY(temp_x, temp_y);
                enemy.back().SetState(1);
                this->enemy_id.push_back(enemy.size() - 1);
                this->enemy_num++;
                if(enemy.back().flag_boss_){
                    game.boss_num++;
                }
            }
            else {
                obstacle.push_back(obstacle_list[Obj_idx[i]+1]);
                double temp_y = margin_y + 8-double(i / 13 + 1);
                double temp_x = margin_x + double(i % 13 + 1);
                obstacle.back().SetXY(temp_x, temp_y);
                obstacle.back().SetState(1);
                if (Obj_idx[i] > 1) {
                    obstacle.back().SetHpMax1(10000);
                }
                this->obstacle_id.push_back(obstacle.size() - 1);
            }
        }
    }
}

void Room::InitializeTool(Game& game, FILE *fp) {
    std::vector<Tool>& tool = game.tool;
    std::vector<Tool>& tool_list = game.tool_list;
    if (fp == NULL)
    {
        printf("Fail to open file.\n");
    }
    else
    {
        Load(fp);
    }
    for (int i = 0; i < 7 * 13; ++i) {
        if (Obj_idx[i] >= 0 && Obj_idx[i] < 3) {
            int tool_type = MyDraw::IntRand() % 7;
            tool.push_back(tool_list[tool_type]);
            double temp_y = margin_y + 8 - double(i / 13 + 1);
            double temp_x = margin_x + double(i % 13 + 1);
            tool.back().SetXY(temp_x, temp_y);
            tool.back().SetState(1);
            this->tool_id.push_back(tool.size() - 1);
        }

    }
}