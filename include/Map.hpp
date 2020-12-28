#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include "fssimplewindow.h"
#include "GameObject.hpp"
using namespace std;

class Map{
protected:
    // width of the whole map
    int width;
    // height of the whole map
    int height;
    // row position of current room
    int curr_row;
    // column position of current room
    int curr_col;
    // room map to define whether there is a room in one position
    vector<vector<int>> rooms;
public:
    Map();
    ~Map();
    void CleanUp();

    // change room 
    void RoomStep(int dir);
    // animation of room changes in the game
    void Animation(int c);
    // draw box
    void DrawRect(int x1, int y1, int x2, int y2, int r, int g, int b);
};

Map::Map() {
    width = 5;
    height = 5;
}

Map::~Map() {
    CleanUp();
}

void Map::CleanUp() {
    width = 0;
    height = 0;
    curr_row = 0;
    curr_col = 0;
}

void Map::Animation(int c) {
    int num_box_row = 800 / 20;
    int num_box_col = 600 / 20;
    for (int i = 0; i < num_box_row; i++) {
        for (int j = 0; j < num_box_col; j++) {
            int cc = (i + j) * c * 464 % 255;
            DrawRect(i * 20, j * 20, (i + 1) * 20, (j + 1) * 20, cc, cc, cc);
        }
    }
}

void Map::RoomStep(int dir) {
    //up
    if (dir == 0) {
        curr_row = curr_row - 1;
    }
    //down
    if (dir == 1) {
        curr_row = curr_row + 1;
    }
    //left
    if (dir == 2) {
        curr_row = curr_col - 1;
    }
    //right
    if (dir == 3) {
        curr_row = curr_col + 1;
    }
}

void Map::DrawRect(int x1, int y1, int x2, int y2, int r, int g, int b) {
    
    glBegin(GL_QUADS);
    
    //printf("%d ", color);
    glColor3ub(r, g, b);
    glVertex2i(x1, y1);
    glVertex2i(x2, y1);
    glVertex2i(x2, y2);
    glVertex2i(x1, y2);
    glEnd();
}






class Room : public GameObject {
protected:
    // rows of one room
    int rows;
    // columns of one room
    int columns;
    // player row position in this room
    int curr_row;
    // player column position in the room
    int curr_col;
    // content in the room
    vector<vector<int>> positions;
    // enemy
    int num_enemy;
    vector<Computer> enemy;
    // tools
    int num_tools;
    vector<Computer> tools;
    // obtacles
    int num_obstacles;
    vector<Computer> obtacles;
public:
    Room();
    ~Room();
    Room(const Room &from);
    const Room &operator=(const Room &from);
    void Initialize();
    void Draw();
    void CleanUp();
    
};

Room::Room() {
    initialize();
}

Room::~Room() {
    CleanUp();
}

void Room::Initialize() {
    // the room range
    rows = 9;
    columns = 13;
    positions.resize(rows);
    for (int i = 0; i < positions.size(); i++) {
        positions[i].resize(columns);
    }
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            positions[i][j] = 0;
        }
    }

    // enemy
    num_enemy = 3;
    for (int i = 0; i < num_enemy; i++) {
        srand((unsigned)time(NULL));
        float rs = rand() % 9 + 0.5;
        float cs = rand() % 13 + 0.5;
        enemy.push_back(enemy_list[0]);
        enemy[i].SetState(1);
        enemy[i].SetScale(2.5);
        enemy[i].SetXY(rs, cs);
        enemy[i].SetV(5);
        enemy[i].SetHpMax1(20);
    }
    
    // obstacle
    num_obstacles = 3;
    for (int i = 0; i < num_obstacles; i++) {
        srand((unsigned)time(NULL));
        float rs = rand() % 9 + 0.5;
        float cs = rand() % 13 + 0.5;
        obtacles.push_back(obtacles_list[0]);
        obtacles[i].SetState(1);
        obtacles[i].SetScale(1.3);
        obtacles[i].SetXY(rs, cs);
        obtacles[i].SetHpMax1(10000);
    }

    // tools
    num_tools = 3;
    for (int i = 0; i < num_tools; i++) {
        srand((unsigned)time(NULL));
        int num = rand() % tool_list.size();
        tools.push_back(tool_list[num]);
        tools[i].SetState(1);
        srand((unsigned)time(NULL));
        float rs = rand() % 9 + 0.5;
        float cs = rand() % 13 + 0.5;
        tools[i].SetXY(rs, cs);
    }
    return;
}

void Room::Draw() {

}

void Room::CleanUp() {
    rows = 0;
    columns = 0;
    curr_row = 0;
    curr_col = 0;
}