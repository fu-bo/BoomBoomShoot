#include "Game.hpp"
#include <time.h>
#include <algorithm>
#include <random>
#include <chrono>

void RenderRoom(void *void_ptr){
    Game* game_ptr = (Game*) void_ptr;

    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    game_ptr->DrawRoom();

    FsSwapBuffers();
}

void Game::OpenWindow(){
    FsOpenWindow(16,16,MyDraw::WIN_LENGTH,MyDraw::WIN_HEIGHT,1);
}

void Game::enemyAct(double curr_time, std::vector<Bullet>& bullet_list) {
    // enemy.Idle(curr_time);
    for(int i : room[room_id].enemy_id){
        enemy[i].Move(player.GetX(), player.GetY());
    }
    for(int i : room[room_id].obstacle_id){
        for(int j : room[room_id].enemy_id){
            if(enemy[j].state() > 0 && obstacle[i].state() > 0){
                if(GameObject::CheckCollision(enemy[j], obstacle[i])){
                    enemy[j].Collide(curr_time, obstacle[i]);
                }     
            }
        }
    }
    bool flag_enemy_fire = false;
    for(int i : room[room_id].enemy_id){
        if(enemy[i].Fire(curr_time, player.GetX(), player.GetY(), enemy_bullet, bullet_list) ){
            flag_enemy_fire = true;
        }
        // enemy[i].Fire(curr_time, 90, enemy_bullet, bullet_list); 
    }
    if(flag_enemy_fire){
        sound_player.Stop(sound_list[6]);
        sound_player.PlayOneShot(sound_list[6]);
    }
}

Game::Game(){
    terminate = false;
    curr_time = 0;
    start_time = FsSubSecondTimer();

    map_x = 5;
    map_y = 5;
    room_id = 0;
    room_x = 0;
    room_y = 0;
    next_id = 0;

    room_offset_x = 0;
    room_offset_y = 0;
    next_offset_x = 0;
    next_offset_y = 0;
    flag_switch_direction = -1;
    flag_player_not_check_collision_ = false;
    this->boss_num = 0;

    // Sound    

    sound_list[0].LoadWav("data/audio/bg2.wav"); //background(in game")
    sound_list[1].LoadWav("data/audio/menu.wav");    //background("menu")
    sound_list[2].LoadWav("data/audio/mouse_click.wav"); //mouse click
    sound_list[3].LoadWav("data/audio/win sound 2-3.wav");   //win
    sound_list[4].LoadWav("data/audio/lose.wav");    //lose
    sound_list[5].LoadWav("data/audio/laser1.wav");  //bullet1
    sound_list[6].LoadWav("data/sound/Click-SoundBible.com-1387633738.wav");  //bullet2
    sound_list[7].LoadWav("data/audio/cat.wav"); //bullet3
    sound_list[8].LoadWav("data/audio/bullet4.wav"); //bullet4
    sound_list[9].LoadWav("data/audio/hit.wav"); //bullet5
    sound_list[10].LoadWav("data/audio/door.wav");    //dooropen
    sound_list[11].LoadWav("data/audio/got_item.wav");    //got items
    sound_list[12].LoadWav("data/audio/item1.wav");   //items appear
    sound_list[13].LoadWav("data/audio/dead.wav");    //player got killed
    sound_list[14].LoadWav("data/audio/gruntsound.wav");  //player got hit
    sound_list[15].LoadWav("data/audio/moster-paind.wav/monster-deathd.wav"); //monster1("hit/dead")
    sound_list[16].LoadWav("data/audio/moster-paine.wav/monster-deathe.wav"); //monster2("hit/dead")
    sound_list[17].LoadWav("data/audio/moster-painr.wav/monster-deathr.wav"); //monster3(hit/dead)

    sound_player.Start();
    sound_player.PlayBackground(sound_list[0]);

}

Game::~Game(){
    sound_player.End();
}

void Game::Initialize(){
    // Initialize program
    enemy.clear();
    obstacle.clear();
    tile.clear();
    player_bullet.clear();
    enemy_bullet.clear();
    tool.clear();
    door.clear();

    enemy_list.clear();
    obstacle_list.clear();
    tile_list.clear();
    bullet_list.clear();
    tool_list.clear();
    door_list.clear();  

    terminate = false;
    // start_time = FsSubSecondTimer();

    std::vector<std::string> file_names;
    this->boss_num = 0;

    // Background
    // file_names.push_back("data/image/background/Dungeon1.png");
    // background_png = std::make_shared<PngManager>(file_names);

    // Player
    player.Reset();
    file_names.clear();    
    file_names.push_back("data/image/elf/elf_m_idle_anim_f0.png");
    file_names.push_back("data/image/elf/elf_m_idle_anim_f1.png");
    file_names.push_back("data/image/elf/elf_m_idle_anim_f2.png");
    file_names.push_back("data/image/elf/elf_m_idle_anim_f3.png");

    file_names.push_back("data/image/elf/elf_m_run_anim_f0.png");
    file_names.push_back("data/image/elf/elf_m_run_anim_f1.png");
    file_names.push_back("data/image/elf/elf_m_run_anim_f2.png");
    file_names.push_back("data/image/elf/elf_m_run_anim_f3.png");

    std::shared_ptr<PngManager> elfm_png = std::make_shared<PngManager>(file_names);

    std::vector<std::vector<int>> elfm_state2pngid;
    elfm_state2pngid.push_back(std::vector<int>({0,1,2,3}));
    elfm_state2pngid.push_back(std::vector<int>({4,5,6,7}));

    std::vector<Circle> elfm_circle;
    elfm_circle.push_back(Circle(0 * MyDraw::PIXEL2METER, -0.1 * elfm_png->Hei(0) * MyDraw::PIXEL2METER, 0.4 * elfm_png->Wid(0) * MyDraw::PIXEL2METER));
    elfm_circle.push_back(Circle(0 * MyDraw::PIXEL2METER, -0.25 * elfm_png->Hei(0) * MyDraw::PIXEL2METER, 0.35 * elfm_png->Wid(0) * MyDraw::PIXEL2METER));

    // player.SetPng(elfm_png, elfm_state2pngid, 0, 0, 0.5);
    player.SetPng(elfm_png, elfm_state2pngid, elfm_circle);

    player.SetState(1);
    player.SetScale(2.5);
    player.SetXY(5.5, 4.5);
    player.SetV(5);
    player.SetHpMax1(20);
    flag_player_not_check_collision_ = true;

    file_names.clear();    

    // Enemy list
    std::vector<int> enemy_moving_flag;
    std::vector<int> enemy_shooting_flag;
    std::vector<int> enemy_bullet_flag;
    std::vector<std::shared_ptr<PngManager>> enemy_png_list;
    std::vector<std::vector<Circle>> enemy_circle_list;
    std::vector<Circle> one_enemy_circle;
    std::vector<std::vector<std::vector<int>>> enemy_state2pngid;
    std::vector<std::vector<int>> one_enemy_state2pngid;

    file_names.clear();
    file_names.push_back("data/image/enemy/skelet_idle_anim_f0.png");
    file_names.push_back("data/image/enemy/skelet_idle_anim_f1.png");
    file_names.push_back("data/image/enemy/skelet_idle_anim_f2.png");
    file_names.push_back("data/image/enemy/skelet_idle_anim_f3.png");

    file_names.push_back("data/image/enemy/skelet_run_anim_f0.png");
    file_names.push_back("data/image/enemy/skelet_run_anim_f1.png");
    file_names.push_back("data/image/enemy/skelet_run_anim_f2.png");
    file_names.push_back("data/image/enemy/skelet_run_anim_f3.png");
    enemy_moving_flag.push_back(0);
    enemy_shooting_flag.push_back(0);
    enemy_bullet_flag.push_back(7);

    enemy_png_list.push_back(  std::make_shared<PngManager>(file_names)  );
    one_enemy_state2pngid.clear();
    one_enemy_state2pngid.push_back(std::vector<int>({0,1,2,3}));
    one_enemy_state2pngid.push_back(std::vector<int>({4,5,6,7}));
    enemy_state2pngid.push_back(one_enemy_state2pngid);
    one_enemy_circle.clear();
    one_enemy_circle.push_back(Circle(0 * MyDraw::PIXEL2METER, -0.05 * elfm_png->Hei(0) * MyDraw::PIXEL2METER, 0.4 * elfm_png->Wid(0) * MyDraw::PIXEL2METER));
    //one_enemy_circle.push_back(Circle(0 * MyDraw::PIXEL2METER, -0.3 * elfm_png->Hei(0) * MyDraw::PIXEL2METER, 0.35 * elfm_png->Wid(0) * MyDraw::PIXEL2METER));
    enemy_circle_list.push_back(one_enemy_circle);

    file_names.clear();
    file_names.push_back("data/image/enemy/chort_idle_anim_f0.png");
    file_names.push_back("data/image/enemy/chort_idle_anim_f1.png");
    file_names.push_back("data/image/enemy/chort_idle_anim_f2.png");
    file_names.push_back("data/image/enemy/chort_idle_anim_f3.png");

    file_names.push_back("data/image/enemy/chort_run_anim_f0.png");
    file_names.push_back("data/image/enemy/chort_run_anim_f1.png");
    file_names.push_back("data/image/enemy/chort_run_anim_f2.png");
    file_names.push_back("data/image/enemy/chort_run_anim_f3.png");
    enemy_moving_flag.push_back(3);
    enemy_shooting_flag.push_back(1);
    enemy_bullet_flag.push_back(7);
    enemy_png_list.push_back(std::make_shared<PngManager>(file_names));
    one_enemy_state2pngid.clear();
    one_enemy_state2pngid.push_back(std::vector<int>({ 0,1,2,3 }));
    one_enemy_state2pngid.push_back(std::vector<int>({ 4,5,6,7 }));
    enemy_state2pngid.push_back(one_enemy_state2pngid);
    one_enemy_circle.clear();
    one_enemy_circle.push_back(Circle(0 * MyDraw::PIXEL2METER, -0.05 * elfm_png->Hei(0) * MyDraw::PIXEL2METER, 0.4 * elfm_png->Wid(0) * MyDraw::PIXEL2METER));
    //one_enemy_circle.push_back(Circle(0 * MyDraw::PIXEL2METER, -0.3 * elfm_png->Hei(0) * MyDraw::PIXEL2METER, 0.35 * elfm_png->Wid(0) * MyDraw::PIXEL2METER));
    enemy_circle_list.push_back(one_enemy_circle);

    file_names.clear();
    file_names.push_back("data/image/enemy/wizzard_idle_anim_f0.png");
    file_names.push_back("data/image/enemy/wizzard_idle_anim_f1.png");
    file_names.push_back("data/image/enemy/wizzard_idle_anim_f2.png");
    file_names.push_back("data/image/enemy/wizzard_idle_anim_f3.png");

    file_names.push_back("data/image/enemy/wizzard_run_anim_f0.png");
    file_names.push_back("data/image/enemy/wizzard_run_anim_f1.png");
    file_names.push_back("data/image/enemy/wizzard_run_anim_f2.png");
    file_names.push_back("data/image/enemy/wizzard_run_anim_f3.png");
    enemy_moving_flag.push_back(2);
    enemy_shooting_flag.push_back(2);
    enemy_bullet_flag.push_back(7);
    enemy_png_list.push_back(std::make_shared<PngManager>(file_names));
    one_enemy_state2pngid.clear();
    one_enemy_state2pngid.push_back(std::vector<int>({ 0,1,2,3 }));
    one_enemy_state2pngid.push_back(std::vector<int>({ 4,5,6,7 }));
    enemy_state2pngid.push_back(one_enemy_state2pngid);
    one_enemy_circle.clear();
    one_enemy_circle.push_back(Circle(0 * MyDraw::PIXEL2METER, -0.05 * elfm_png->Hei(0) * MyDraw::PIXEL2METER, 0.4 * elfm_png->Wid(0) * MyDraw::PIXEL2METER));
    //one_enemy_circle.push_back(Circle(0 * MyDraw::PIXEL2METER, -0.3 * elfm_png->Hei(0) * MyDraw::PIXEL2METER, 0.35 * elfm_png->Wid(0) * MyDraw::PIXEL2METER));
    enemy_circle_list.push_back(one_enemy_circle);

    file_names.clear();
    file_names.push_back("data/image/enemy/boss0.png");
    file_names.push_back("data/image/enemy/boss1.png");
    file_names.push_back("data/image/enemy/boss2.png");
    file_names.push_back("data/image/enemy/boss3.png");

    file_names.push_back("data/image/enemy/boss4.png");
    file_names.push_back("data/image/enemy/boss5.png");
    file_names.push_back("data/image/enemy/boss6.png");
    file_names.push_back("data/image/enemy/boss7.png");
    enemy_moving_flag.push_back(3);
    enemy_shooting_flag.push_back(3);
    enemy_bullet_flag.push_back(6);
    enemy_png_list.push_back(std::make_shared<PngManager>(file_names));
    one_enemy_state2pngid.clear();
    one_enemy_state2pngid.push_back(std::vector<int>({ 0,1,2,3 }));
    one_enemy_state2pngid.push_back(std::vector<int>({ 4,5,6,7 }));
    enemy_state2pngid.push_back(one_enemy_state2pngid);
    one_enemy_circle.clear();
    one_enemy_circle.push_back(Circle(0 * MyDraw::PIXEL2METER, -0.05 * elfm_png->Hei(0) * MyDraw::PIXEL2METER, 0.4 * elfm_png->Wid(0) * MyDraw::PIXEL2METER));
    //one_enemy_circle.push_back(Circle(0 * MyDraw::PIXEL2METER, -0.3 * elfm_png->Hei(0) * MyDraw::PIXEL2METER, 0.35 * elfm_png->Wid(0) * MyDraw::PIXEL2METER));
    enemy_circle_list.push_back(one_enemy_circle);

    for(int i = 0; i < enemy_png_list.size(); i++){
        Computer one_enemy;
        one_enemy.SetPng(enemy_png_list[i], enemy_state2pngid[i], enemy_circle_list[i]);
        one_enemy.SetState(0);
        one_enemy.SetXY(20, 20);
        one_enemy.SetScale(2.5);
        if(i == 3){
            one_enemy.SetScale(5);            
            one_enemy.flag_boss_ = true;
        }
        one_enemy.SetV(0.5);
        one_enemy.SetDirection(0);
        one_enemy.SetMovingState(0);
        one_enemy.SetBlocked(0);
        one_enemy.SetBullet(enemy_bullet_flag[i]);
        one_enemy.SetMovingState(enemy_moving_flag[i]);
        one_enemy.SetFireMode(enemy_shooting_flag[i]);
        one_enemy.SetShootGap(3);
        if(i <= 2){
            one_enemy.SetHpMax1((double)i*8);
        }
        else if(i == 3){
            one_enemy.SetHpMax1((double)40);
        }
        else{
            one_enemy.SetHpMax1((double)20);
        }
        enemy_list.push_back(one_enemy);
    }

    // Obstacle list
    std::vector<std::shared_ptr<PngManager>> obstacle_png_list;
    std::vector<double> obstacle_ratio;
    std::vector<std::vector<int>> obstacle_state2pngid;
    obstacle_state2pngid.push_back(std::vector<int>({0}));

    file_names.clear();
    file_names.push_back("data/image/obstacle/base.png");//0
    obstacle_png_list.push_back(std::make_shared<PngManager>(file_names));
    file_names.clear();
    file_names.push_back("data/image/obstacle/crate.png");//1
    obstacle_png_list.push_back(std::make_shared<PngManager>(file_names));
    file_names.clear();
    file_names.push_back("data/image/obstacle/box.png");//2
    obstacle_png_list.push_back(std::make_shared<PngManager>(file_names));
    file_names.clear();
    file_names.push_back("data/image/obstacle/coin.png");//3
    obstacle_png_list.push_back(std::make_shared<PngManager>(file_names));
    file_names.clear();
    file_names.push_back("data/image/obstacle/column.png");//4
    obstacle_png_list.push_back(std::make_shared<PngManager>(file_names));
    file_names.clear();
    file_names.push_back("data/image/obstacle/spike.png");//5
    obstacle_png_list.push_back(std::make_shared<PngManager>(file_names));

    file_names.clear();
    file_names.push_back("data/image/wall/wall.png");//6
    obstacle_png_list.push_back(std::make_shared<PngManager>(file_names));
    file_names.clear();
    file_names.push_back("data/image/wall/side_left.png");//7
    obstacle_png_list.push_back(std::make_shared<PngManager>(file_names));
    file_names.clear();
    file_names.push_back("data/image/wall/side_right.png");//8
    obstacle_png_list.push_back(std::make_shared<PngManager>(file_names));
    file_names.clear();
    file_names.push_back("data/image/wall/wall_hole.png");//9
    obstacle_png_list.push_back(std::make_shared<PngManager>(file_names));
    file_names.clear();
    file_names.push_back("data/image/wall/banner_red.png");//10
    obstacle_png_list.push_back(std::make_shared<PngManager>(file_names));
    file_names.clear();
    file_names.push_back("data/image/wall/banner_green.png");//11
    obstacle_png_list.push_back(std::make_shared<PngManager>(file_names));
    file_names.clear();
    file_names.push_back("data/image/wall/banner_yellow.png");//12
    obstacle_png_list.push_back(std::make_shared<PngManager>(file_names));
    file_names.clear();
    file_names.push_back("data/image/obstacle/sword.png");//13
    obstacle_png_list.push_back(std::make_shared<PngManager>(file_names));
    file_names.clear();
    file_names.push_back("data/image/obstacle/spear.png");//14
    obstacle_png_list.push_back(std::make_shared<PngManager>(file_names));
    obstacle_ratio.clear();

    for (int i = 0; i < obstacle_png_list.size(); i++) {
        Computer one_obstacle;
        one_obstacle.SetType(2);
        one_obstacle.SetState(1);
        one_obstacle.SetScale(1.3);
        if (i == 0|| i == 2) {
            one_obstacle.SetScale(1.3);
            one_obstacle.SetPng(obstacle_png_list[i], obstacle_state2pngid, 0, 0, 0.4);
        }
        else if (i == 13 || i == 14) {
            one_obstacle.SetScale(1.5);
            one_obstacle.SetPng(obstacle_png_list[i], obstacle_state2pngid, 0, 0, 0.4);
        }
        else if (i == 3) {
            one_obstacle.SetScale(2);
            one_obstacle.SetPng(obstacle_png_list[i], obstacle_state2pngid, 0, 0, 0.4);
        }
        else if (i == 4) {
            one_obstacle.SetScale(3.2);
            one_obstacle.SetPng(obstacle_png_list[i], obstacle_state2pngid, 0, -0.3, 0.5, 0.15);
        }
        else {
            one_obstacle.SetScale(3.2);
            one_obstacle.SetPng(obstacle_png_list[i], obstacle_state2pngid, 0, 0, 0.5);
        }
        
        one_obstacle.SetXY(20, 20);
        one_obstacle.SetV(10);
        one_obstacle.SetHpMax1(10);
        obstacle_list.push_back(one_obstacle);
    }
    
    std::vector<std::shared_ptr<PngManager>> tile_png_list;
    std::vector<std::vector<int>> tile_state2pngid;
    tile_state2pngid.push_back(std::vector<int>({ 0 }));

    file_names.clear();
    file_names.push_back("data/image/tile/floor_1.png");
    tile_png_list.push_back(std::make_shared<PngManager>(file_names));
    file_names.clear();
    file_names.push_back("data/image/tile/floor_2.png");
    tile_png_list.push_back(std::make_shared<PngManager>(file_names));
    file_names.clear();
    file_names.push_back("data/image/tile/floor_3.png");
    tile_png_list.push_back(std::make_shared<PngManager>(file_names));
    file_names.clear();
    file_names.push_back("data/image/tile/floor_4.png");
    tile_png_list.push_back(std::make_shared<PngManager>(file_names));
    file_names.clear();
    file_names.push_back("data/image/tile/floor_5.png");
    tile_png_list.push_back(std::make_shared<PngManager>(file_names));

    for (int i = 0; i < tile_png_list.size(); i++) {
        Computer one_tile;
        one_tile.SetType(2);
        one_tile.SetState(1);
        one_tile.SetScale(3.2);
        one_tile.SetPng(tile_png_list[i], tile_state2pngid);
        one_tile.SetXY(20, 20);
        one_tile.SetV(10);
        one_tile.SetHpMax1(20);
        tile_list.push_back(one_tile);

    }
    // Door list
    std::vector<std::shared_ptr<PngManager>> door_png_list;
    std::vector<double> door_ratio;
    std::vector<std::vector<std::vector<int>>> door_state2pngid;
    std::vector<std::vector<int>> one_door_state2pngid;

    file_names.clear();
    file_names.push_back("data/image/door/door_closed.png");
    file_names.push_back("data/image/door/door_open.png");
    door_png_list.push_back(std::make_shared<PngManager>(file_names));
    one_door_state2pngid.clear();
    one_door_state2pngid.push_back(std::vector<int>({ 0 }));
    one_door_state2pngid.push_back(std::vector<int>({ 1 }));
    door_state2pngid.push_back(one_door_state2pngid);

    door_ratio.clear();

    for (int i = 0; i < door_png_list.size(); i++) {
        Computer one_door;
        one_door.SetType(2);
        one_door.SetState(1);
        one_door.SetScale(1.8);
        if (i == 0) {
            door_ratio.push_back(0.1);
        }
        else {
            door_ratio.push_back(0.4);
        }
        one_door.SetPng(door_png_list[i], door_state2pngid[i], 0, 0, door_ratio[i]);
        one_door.SetXY(20, 20);
        one_door.SetV(10);
        one_door.SetHpMax1(20);
        door_list.push_back(one_door);

    }

    // Bullet
    std::vector<std::shared_ptr<PngManager>> bullet_png_list;
    std::vector<Circle> bullet_circle;
    std::vector<std::vector<std::vector<int>>> bullet_state2pngid;
    std::vector<std::vector<int>> one_bullet_state2pngid;

    file_names.clear();
    file_names.push_back("data/image/bullet_onion/bullet_onion1.png");
    file_names.push_back("data/image/bullet_onion/bullet_onion2.png");
    file_names.push_back("data/image/bullet_onion/bullet_onion3.png");
    file_names.push_back("data/image/bullet_onion/bullet_onion4.png");
    file_names.push_back("data/image/bullet_onion/bullet_onion5.png");
    file_names.push_back("data/image/bullet_onion/bullet_onion6.png");
    bullet_png_list.push_back(  std::make_shared<PngManager>(file_names)  );
    one_bullet_state2pngid.clear();
    one_bullet_state2pngid.push_back(std::vector<int>({0,1}));
    one_bullet_state2pngid.push_back(std::vector<int>({2,3,4,5}));
    bullet_state2pngid.push_back(one_bullet_state2pngid);

    file_names.clear();
    file_names.push_back("data/image/bullet/rocket.png");
    file_names.push_back("data/image/bullet/explosion/rocket1.png");
    file_names.push_back("data/image/bullet/explosion/rocket2.png");
    file_names.push_back("data/image/bullet/explosion/rocket3.png");
    file_names.push_back("data/image/bullet/explosion/rocket4.png");
    file_names.push_back("data/image/bullet/explosion/rocket5.png");
    file_names.push_back("data/image/bullet/explosion/rocket6.png");
    bullet_png_list.push_back(std::make_shared<PngManager>(file_names));
    one_bullet_state2pngid.clear();
    one_bullet_state2pngid.push_back(std::vector<int>({ 0 }));
    one_bullet_state2pngid.push_back(std::vector<int>({ 1,2,3,4,5,6 }));
    bullet_state2pngid.push_back(one_bullet_state2pngid);

    file_names.clear();
    file_names.push_back("data/image/bullet/cannonball.png");
    file_names.push_back("data/image/bullet/explosion/exp3.png");
    file_names.push_back("data/image/bullet/explosion/exp4.png");
    file_names.push_back("data/image/bullet/explosion/exp5.png");
    file_names.push_back("data/image/bullet/explosion/exp6.png");
    file_names.push_back("data/image/bullet/explosion/exp7.png");

    bullet_png_list.push_back(std::make_shared<PngManager>(file_names));
    one_bullet_state2pngid.clear();
    one_bullet_state2pngid.push_back(std::vector<int>({ 0 }));
    one_bullet_state2pngid.push_back(std::vector<int>({ 1,2,3,4,5 }));
    bullet_state2pngid.push_back(one_bullet_state2pngid);

    file_names.clear();
    file_names.push_back("data/image/bullet/microwave.png");
    file_names.push_back("data/image/bullet/microwave.png");
    file_names.push_back("data/image/bullet/microwave.png");
    file_names.push_back("data/image/bullet/microwave.png");
    file_names.push_back("data/image/bullet/microwave.png");
    file_names.push_back("data/image/bullet/microwave.png");
    file_names.push_back("data/image/bullet/microwave.png");
    file_names.push_back("data/image/bullet/microwave.png");
    file_names.push_back("data/image/bullet/microwave.png");
    bullet_png_list.push_back(std::make_shared<PngManager>(file_names));
    one_bullet_state2pngid.clear();
    one_bullet_state2pngid.push_back(std::vector<int>({ 0 }));
    one_bullet_state2pngid.push_back(std::vector<int>({ 1,2,3,4,5,6,7,8 }));
    bullet_state2pngid.push_back(one_bullet_state2pngid);

    file_names.clear();
    file_names.push_back("data/image/bullet/cat.png");
    file_names.push_back("data/image/bullet/cat1.png");
    file_names.push_back("data/image/bullet/cat2.png");
    file_names.push_back("data/image/bullet/cat3.png");
    file_names.push_back("data/image/bullet/cat4.png");
    bullet_png_list.push_back(std::make_shared<PngManager>(file_names));
    one_bullet_state2pngid.clear();
    one_bullet_state2pngid.push_back(std::vector<int>({ 0 }));
    one_bullet_state2pngid.push_back(std::vector<int>({ 1,2,3,4 }));
    bullet_state2pngid.push_back(one_bullet_state2pngid);

    file_names.clear();
    file_names.push_back("data/image/bullet/cup.png");
    file_names.push_back("data/image/bullet/explosion/exp0.png");
    file_names.push_back("data/image/bullet/explosion/exp1.png");
    file_names.push_back("data/image/bullet/explosion/exp2.png");
    bullet_png_list.push_back(std::make_shared<PngManager>(file_names));
    one_bullet_state2pngid.clear();
    one_bullet_state2pngid.push_back(std::vector<int>({ 0 }));
    one_bullet_state2pngid.push_back(std::vector<int>({ 1,2,3 }));
    bullet_state2pngid.push_back(one_bullet_state2pngid);

    file_names.clear();
    file_names.push_back("data/image/bullet/tomato0.png");
    file_names.push_back("data/image/bullet/tomato1.png");
    file_names.push_back("data/image/bullet/tomato2.png");
    file_names.push_back("data/image/bullet/tomato3.png");
    file_names.push_back("data/image/bullet/tomato4.png");
    file_names.push_back("data/image/bullet/tomato5.png");
    bullet_png_list.push_back(std::make_shared<PngManager>(file_names));
    one_bullet_state2pngid.clear();
    one_bullet_state2pngid.push_back(std::vector<int>({ 0,1 }));
    one_bullet_state2pngid.push_back(std::vector<int>({ 2,3,4,5 }));
    bullet_state2pngid.push_back(one_bullet_state2pngid);

    file_names.clear();
    file_names.push_back("data/image/bullet/bulleta.png");
    bullet_png_list.push_back(std::make_shared<PngManager>(file_names));
    one_bullet_state2pngid.clear();
    one_bullet_state2pngid.push_back(std::vector<int>({ 0 }));
    bullet_state2pngid.push_back(one_bullet_state2pngid);

    for(int i = 0; i < bullet_png_list.size(); i++){
        bullet_circle.clear();
        bullet_circle.push_back(Circle(0 * MyDraw::PIXEL2METER, 0 * bullet_png_list[i]->Hei(0) * MyDraw::PIXEL2METER, 0.4 * bullet_png_list[i]->Wid(0) * MyDraw::PIXEL2METER));

        Bullet one_bullet;
        one_bullet.SetPng(bullet_png_list[i], bullet_state2pngid[i], bullet_circle);
        one_bullet.SetState(0);
        one_bullet.SetScale(2);
        one_bullet.SetXY(20, 20);
        one_bullet.SetV(10);
        one_bullet.SetHpMax1(1);
        one_bullet.SetBulletId(i);
        one_bullet.SetRange(20);
        if(i != 7 ){
            one_bullet.SetDisappearState(2,0);
            one_bullet.SetScale(2);
        }
        if(i == 7 || i == 6){
            one_bullet.SetV(4);
        }
        bullet_list.push_back(one_bullet);
    }

    for(int i = 0; i < 100; i++){
        player_bullet.push_back(bullet_list[0]);
    }

    for(int i = 0; i < 200; i++){
        enemy_bullet.push_back(bullet_list[0]);
        // enemy_bullet.back().SetV(3);
    }

    // Tool list
    std::vector<std::shared_ptr<PngManager>> tool_png_list;
    std::vector<Circle> tool_circle;
    std::vector<std::vector<std::vector<int>>> tool_state2pngid;
    std::vector<std::vector<int>> one_tool_state2pngid;

    file_names.clear();
    file_names.push_back("data/image/tool_heart/heart1.png");
    file_names.push_back("data/image/tool_heart/heart2.png");
    tool_png_list.push_back(std::make_shared<PngManager>(file_names));
    one_tool_state2pngid.clear();
    one_tool_state2pngid.push_back(std::vector<int>({ 0,1 }));
    tool_state2pngid.push_back(one_tool_state2pngid);

    file_names.clear();
    file_names.push_back("data/image/bullet_onion/bullet_onion1.png");
    file_names.push_back("data/image/bullet_onion/bullet_onion2.png");
    file_names.push_back("data/image/bullet_onion/bullet_onion3.png");
    file_names.push_back("data/image/bullet_onion/bullet_onion4.png");
    file_names.push_back("data/image/bullet_onion/bullet_onion5.png");
    file_names.push_back("data/image/bullet_onion/bullet_onion6.png");
    tool_png_list.push_back(std::make_shared<PngManager>(file_names));
    one_tool_state2pngid.clear();
    one_tool_state2pngid.push_back(std::vector<int>({ 0,1 }));
    one_tool_state2pngid.push_back(std::vector<int>({ 2,3,4,5 }));
    tool_state2pngid.push_back(one_tool_state2pngid);

    file_names.clear();
    file_names.push_back("data/image/bullet/rocket.png");
    tool_png_list.push_back(std::make_shared<PngManager>(file_names));
    one_tool_state2pngid.clear();
    one_tool_state2pngid.push_back(std::vector<int>({ 0 }));
    tool_state2pngid.push_back(one_tool_state2pngid);

    file_names.clear();
    file_names.push_back("data/image/bullet/cannonball.png");
    tool_png_list.push_back(std::make_shared<PngManager>(file_names));
    one_tool_state2pngid.clear();
    one_tool_state2pngid.push_back(std::vector<int>({ 0 }));
    tool_state2pngid.push_back(one_tool_state2pngid);

    file_names.clear();
    file_names.push_back("data/image/bullet/microwave.png");
    tool_png_list.push_back(std::make_shared<PngManager>(file_names));
    one_tool_state2pngid.clear();
    one_tool_state2pngid.push_back(std::vector<int>({ 0 }));
    tool_state2pngid.push_back(one_tool_state2pngid);

    file_names.clear();
    file_names.push_back("data/image/bullet/cat.png");
    tool_png_list.push_back(std::make_shared<PngManager>(file_names));
    one_tool_state2pngid.clear();
    one_tool_state2pngid.push_back(std::vector<int>({ 0 }));
    tool_state2pngid.push_back(one_tool_state2pngid);

    file_names.clear();
    file_names.push_back("data/image/bullet/cup.png");
    tool_png_list.push_back(std::make_shared<PngManager>(file_names));
    one_tool_state2pngid.clear();
    one_tool_state2pngid.push_back(std::vector<int>({ 0 }));
    tool_state2pngid.push_back(one_tool_state2pngid);

    file_names.clear();
    file_names.push_back("data/image/bullet/tomato0.png");
    file_names.push_back("data/image/bullet/tomato1.png");
    file_names.push_back("data/image/bullet/tomato2.png");
    file_names.push_back("data/image/bullet/tomato3.png");
    file_names.push_back("data/image/bullet/tomato4.png");
    file_names.push_back("data/image/bullet/tomato5.png");
    tool_png_list.push_back(std::make_shared<PngManager>(file_names));
    one_tool_state2pngid.clear();
    one_tool_state2pngid.push_back(std::vector<int>({ 0 }));
    one_tool_state2pngid.push_back(std::vector<int>({ 1,2,3,4,5 }));
    tool_state2pngid.push_back(one_tool_state2pngid);

    for (int i = 0; i < tool_png_list.size(); i++) {
        tool_circle.clear();
        tool_circle.push_back(Circle(0 * MyDraw::PIXEL2METER, 0 * tool_png_list[i]->Hei(0) * MyDraw::PIXEL2METER, 0.4 * tool_png_list[i]->Wid(0) * MyDraw::PIXEL2METER));
        Tool one_tool;
        one_tool.SetPng(tool_png_list[i], tool_state2pngid[i], tool_circle);
        one_tool.SetState(0);
        one_tool.SetScale(2);
        one_tool.SetXY(20, 20);
        one_tool.SetV(10);
        one_tool.SetHpMax1(1);
        one_tool.SetToolId(i);
        if (i == 1 || i == 2) {
            one_tool.SetDisappearState(2, 0);
            one_tool.SetScale(2);
        }
        else if (i == 0) {
            one_tool.SetScale(1.5);
        }

        tool_list.push_back(one_tool);
    }

    // Initialize room    

    FILE *obs_map, *tool_map;
    std::vector<std::string> obs_files;
    std::vector<std::string> tool_files;
    obs_files.push_back("data/maps/map1.txt");
    tool_files.push_back("data/maps/map1.txt");
    obs_files.push_back("data/maps/map2.txt");
    tool_files.push_back("data/maps/map2.txt");
    obs_files.push_back("data/maps/map3.txt");
    tool_files.push_back("data/maps/map3.txt");
    obs_files.push_back("data/maps/map4.txt");
    tool_files.push_back("data/maps/map4.txt");
    obs_files.push_back("data/maps/map5.txt");
    tool_files.push_back("data/maps/map5.txt");
    obs_files.push_back("data/maps/map6.txt");
    tool_files.push_back("data/maps/map6.txt");
    obs_files.push_back("data/maps/map7.txt");
    tool_files.push_back("data/maps/map7.txt");
    obs_files.push_back("data/maps/map8.txt");
    tool_files.push_back("data/maps/map8.txt");
    obs_files.push_back("data/maps/map9.txt");
    tool_files.push_back("data/maps/map9.txt");//9
    obs_files.push_back("data/maps/map10.txt");
    tool_files.push_back("data/maps/map10.txt");//10
    obs_files.push_back("data/maps/map11.txt");
    tool_files.push_back("data/maps/map11.txt");//11
    obs_files.push_back("data/maps/map12.txt");
    tool_files.push_back("data/maps/map12.txt");//12
    obs_files.push_back("data/maps/map13.txt");
    tool_files.push_back("data/maps/map13.txt");//13
    obs_files.push_back("data/maps/map14.txt");
    tool_files.push_back("data/maps/map14.txt");//14
    obs_files.push_back("data/maps/map15.txt");
    tool_files.push_back("data/maps/map15.txt");//15
    obs_files.push_back("data/maps/map16.txt");
    tool_files.push_back("data/maps/map16.txt");//16
    obs_files.push_back("data/maps/map17.txt");
    tool_files.push_back("data/maps/map17.txt");//17
    obs_files.push_back("data/maps/map18.txt");
    tool_files.push_back("data/maps/map18.txt");//18
    obs_files.push_back("data/maps/map19.txt");
    tool_files.push_back("data/maps/map19.txt");//19
    obs_files.push_back("data/maps/map20.txt");
    tool_files.push_back("data/maps/map20.txt");//20
    obs_files.push_back("data/maps/map21.txt");
    tool_files.push_back("data/maps/map21.txt");//21
    obs_files.push_back("data/maps/map22.txt");
    tool_files.push_back("data/maps/map22.txt");//22
    obs_files.push_back("data/maps/map23.txt");
    tool_files.push_back("data/maps/map23.txt");//23
    obs_files.push_back("data/maps/map24.txt");
    tool_files.push_back("data/maps/map24.txt");//24
    obs_files.push_back("data/maps/map25.txt");
    tool_files.push_back("data/maps/map25.txt");//25

    std::vector<int> map_idx_list;
    for (int i = 0; i < 25; i++) {
        map_idx_list.push_back(i);
    }
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    shuffle(map_idx_list.begin(), map_idx_list.end(), std::default_random_engine(seed));

    room_status.clear();
    room.clear();
    room_id = 0;
    room_x = 0;
    room_y = 0;
    next_id = 0;  

    for(int i = 0; i < map_x; i++){
        for(int j = 0; j < map_y; j++){
            int map_idx = map_idx_list[i * 5 + j];
            obs_map = fopen(obs_files[map_idx].c_str(), "r");
            tool_map = fopen(tool_files[map_idx].c_str(), "r");
            room.push_back(Room());
            room.back().InitializeObs(*this,obs_map);
            room.back().InitializeTool(*this, tool_map);
            fclose(obs_map);
            fclose(tool_map);
            room_status.push_back(0);
        }
    }
    room_status[GetRoomId(room_x, room_y)] = 1;

    int bullet_id = 0;
    player.SetBullet(bullet_id);

    curr_time = (double) ( FsSubSecondTimer() - start_time ) / 1000 ;
}

void Game::RunRoom(){
    FsRegisterOnPaintCallBack(RenderRoom, this);

    bar.smallmapmatrix = room_status;
    if(player.hp() < 20){
        bar.playerhpstate = (int)(player.hp()  + 0.5);
    }
    else{
        bar.playerhpstate = 20;
    }
    if(player.hp() < 0.1){
        flag_runmenu = true;
        sound_player.Stop(sound_list[4]);
        sound_player.PlayOneShot(sound_list[4]);
    }
    // for(int i = 0; i < bar.smallmapmatrix.size(); i++){
        // printf("%d ", bar.smallmapmatrix[i]);
    // }
    // printf("\n");

    // printf("obstacle.size() = %d\n", obstacle.size());
    double prev_time = curr_time;
    curr_time = (double) ( FsSubSecondTimer() - start_time ) / 1000;
    double dt = curr_time - prev_time;
    bool flag_player_in_collision = false;

    FsPollDevice();
    auto key=FsInkey();        
    bool flag_key_terminate = FSKEY_ESC==key;
    bool flag_key_up = 0!=FsGetKeyState(FSKEY_UP);
    bool flag_key_down = 0!=FsGetKeyState(FSKEY_DOWN);
    bool flag_key_right = 0!=FsGetKeyState(FSKEY_RIGHT);
    bool flag_key_left = 0!=FsGetKeyState(FSKEY_LEFT);
    bool flag_key_w = 0!=FsGetKeyState(FSKEY_W);
    bool flag_key_a = 0!=FsGetKeyState(FSKEY_A);
    bool flag_key_s = 0!=FsGetKeyState(FSKEY_S);
    bool flag_key_d = 0!=FsGetKeyState(FSKEY_D);
    bool flag_key_1 = 0!=FsGetKeyState(FSKEY_1);

    if(FSKEY_ESC == key){
        // Terminate
        this->flag_runmenu = true;
    }
    if(flag_key_w){
        player.Move(dt, 2);
    }
    if(flag_key_s){
        player.Move(dt, 3);
    }
    if(flag_key_d){
        player.Move(dt, 0);
    }
    if(flag_key_a){
        player.Move(dt, 1);
    }

    if(!(flag_key_w || flag_key_a || flag_key_s || flag_key_d)){
        player.Idle(curr_time);
    }

    int lb,mb,rb,mx,my;
    int mouse_event=FsGetMouseEvent(lb,mb,rb,mx,my);
    bool flag_play_fire = false;
    if(lb == 1){
        double mouse_x, mouse_y;
        MyDraw::Pixel2Meter(mx, my, mouse_x, mouse_y);
        flag_play_fire = player.Fire(curr_time, mouse_x, mouse_y, player_bullet, bullet_list);   
    }
    else if(flag_key_up){
        flag_play_fire = player.Fire(curr_time, 90, player_bullet, bullet_list);    
    }
    else if(flag_key_down){
        flag_play_fire = player.Fire(curr_time, 270, player_bullet, bullet_list);    
    }
    else if(flag_key_right){
        flag_play_fire = player.Fire(curr_time, 0, player_bullet, bullet_list);    
    }
    else if(flag_key_left){
        flag_play_fire = player.Fire(curr_time, 180, player_bullet, bullet_list);    
    }
    if(flag_play_fire){
        if(player.bullet_id() == 4){
            sound_player.Stop(sound_list[7]);
            sound_player.PlayOneShot(sound_list[7]);
        }
        else if (player.bullet_id() == 1){
            sound_player.Stop(sound_list[8]);
            sound_player.PlayOneShot(sound_list[8]);   
        }
        else{
            sound_player.Stop(sound_list[2]);
            sound_player.PlayOneShot(sound_list[2]);            
        }
    }

    if(FSKEY_1 == key){
        int bullet_id = (player.bullet_id() + 1) % bullet_list.size();
        player.SetBullet(bullet_id);
    }
    bool flag_door_open = false;
    if(FSKEY_2 == key){
        flag_door_open = true;
        sound_player.Stop(sound_list[10]);
        sound_player.PlayOneShot(sound_list[10]);
    }

    enemyAct(curr_time, bullet_list);
    // ================================

    for(int i = 0; i < player_bullet.size(); i++){
        player_bullet[i].Move(dt);
    }

    for(int i = 0; i < enemy_bullet.size(); i++){
        enemy_bullet[i].Move(dt);
    }

    // player - enemy
    for(int i : room[room_id].enemy_id){
        if(player.state() > 0 && enemy[i].state() > 0 && !player.NotAttackable()){
            if(GameObject::CheckCollision(player, enemy[i])){
                player.Collide(curr_time, enemy[i]);
                player.MinusHp(1);
                enemy[i].Collide(curr_time, player);
            }     
        }
    }
    // player - obstacle
    for(int i : room[room_id].obstacle_id){
        if(player.state() > 0 && obstacle[i].state() > 0){
            if(GameObject::CheckCollision(player, obstacle[i], 2)){
                flag_player_in_collision = true;
                if(!flag_player_not_check_collision_){
                    // player.Collide(curr_time, obstacle[i]);
                    player.StopX();
                }
            }
            if(GameObject::CheckCollision(player, obstacle[i], 3)){
                flag_player_in_collision = true;
                if(!flag_player_not_check_collision_){
                    // player.Collide(curr_time, obstacle[i]);
                    player.StopY();
                }
            }
        }
    }
    // player_bullet - enemy
    for(int i = 0; i < player_bullet.size(); i++){
        for(int j : room[room_id].enemy_id){
            if(player_bullet[i].state() > 0 && player_bullet[i].hp() > 0 && enemy[j].state() > 0){
                if(GameObject::CheckCollision(player_bullet[i], enemy[j])){
                    double min_hp = GameObject::GetMinHp(player_bullet[i], enemy[j]);
                    player_bullet[i].Collide(curr_time, enemy[j]);
                    player_bullet[i].SetHp(0);
                    enemy[j].MinusHp(min_hp);
                    enemy[j].Collide(curr_time, player_bullet[i]);
                    if(enemy[j].state() <= 0){
                        sound_player.Stop(sound_list[12]);
                        sound_player.PlayOneShot(sound_list[12]);
                        room[room_id].enemy_num--;
                        if(room[room_id].enemy_num <= 0){
                            sound_player.Stop(sound_list[10]);
                            sound_player.PlayOneShot(sound_list[10]);
                        }
                        if(enemy[j].flag_boss_){
                            this->boss_num--;
                            if(this->boss_num <= 0){
                                flag_runmenu = true;
                                sound_player.Stop(sound_list[3]);
                                sound_player.PlayOneShot(sound_list[3]);
                            }
                        }
                    }
                }
            }            
        }
    }
    // player_bullet - obstacle
    for(int i = 0; i < player_bullet.size(); i++){
        for(int j : room[room_id].obstacle_id){
            if( player_bullet[i].state() > 0 && player_bullet[i].hp() > 0 && obstacle[j].state() > 0 ){
                if(GameObject::CheckCollision(player_bullet[i], obstacle[j])){
                    double min_hp = GameObject::GetMinHp(player_bullet[i], obstacle[j]);
                    player_bullet[i].Collide(curr_time, obstacle[j]);
                    player_bullet[i].SetHp(0);
                    obstacle[j].MinusHp(min_hp);
                }
            }
        }
    }
    // enemy_bullet - player
    for(int i = 0; i < enemy_bullet.size(); i++){
        if(enemy_bullet[i].state() > 0 && enemy_bullet[i].hp() > 0 && player.state() > 0){
            if(GameObject::CheckCollision(enemy_bullet[i], player)){
                enemy_bullet[i].Collide(curr_time, player);
                enemy_bullet[i].SetHp(0);
                player.MinusHp(1);
                player.Collide(curr_time, enemy_bullet[i]);
            }
        }            
    }
    // enemy_bullet - obstacle
    for(int i = 0; i < enemy_bullet.size(); i++){
        for(int j : room[room_id].obstacle_id){
            if( enemy_bullet[i].state() > 0 && enemy_bullet[i].hp() > 0 && obstacle[j].state() > 0 ){
                if(GameObject::CheckCollision(enemy_bullet[i], obstacle[j])){
                    double min_hp = GameObject::GetMinHp(enemy_bullet[i], obstacle[j]);
                    enemy_bullet[i].Collide(curr_time, obstacle[j]);
                    enemy_bullet[i].SetHp(0);
                    obstacle[j].MinusHp(min_hp);
                }
            }
        }
    }
    // player - tool
    for(int i : room[room_id].tool_id){
        if(player.state() > 0 && tool[i].state() > 0){
            if(GameObject::CheckCollision(player, tool[i])){
                player.Collide(curr_time, tool[i]);
                tool[i].Disappear(curr_time, player);
            }     
        }
    }

    // player - door
    for(int j = 0; j < room[room_id].door_id.size(); j++ ){
        int i = room[room_id].door_id[j];
        if(player.state() > 0 && door[i].state() > 0){
            if(GameObject::CheckCollision(player, door[i])){
                flag_player_in_collision = true;
                if(!flag_player_not_check_collision_){
                    player.Collide(curr_time, door[i]);
                    if(door[i].state() == 2){
                        flag_switch_direction = j;
                    }
                }
            }     
        }
    }

    // enemy - door
    for(int i : room[room_id].door_id){
        for(int j : room[room_id].enemy_id){
            if(enemy[j].state() > 0 && door[i].state() > 0){
                if(GameObject::CheckCollision(enemy[j], door[i])){
                    enemy[j].Collide(curr_time, door[i]);
                }     
            }
        }
    }
    // enemy_bullet - door
    for(int i = 0; i < enemy_bullet.size(); i++){
        for(int j : room[room_id].door_id){
            if( enemy_bullet[i].state() > 0 && enemy_bullet[i].hp() > 0 && door[j].state() > 0 ){
                if(GameObject::CheckCollision(enemy_bullet[i], door[j])){
                    double min_hp = GameObject::GetMinHp(enemy_bullet[i], door[j]);
                    enemy_bullet[i].Collide(curr_time, door[j]);
                    enemy_bullet[i].SetHp(0);
                    door[j].MinusHp(min_hp);
                }
            }
        }
    }
    // player_bullet - door
    for(int i = 0; i < player_bullet.size(); i++){
        for(int j : room[room_id].door_id){
            if( player_bullet[i].state() > 0 && player_bullet[i].hp() > 0 && door[j].state() > 0 ){
                if(GameObject::CheckCollision(player_bullet[i], door[j])){
                    double min_hp = GameObject::GetMinHp(player_bullet[i], door[j]);
                    player_bullet[i].Collide(curr_time, door[j]);
                    player_bullet[i].SetHp(0);
                    door[j].MinusHp(min_hp);
                }
            }
        }
    }

    if(!flag_player_in_collision){
        flag_player_not_check_collision_ = false;
    }

    if(room[room_id].enemy_num == 0 || flag_door_open){
        for(int j = 0; j < room[room_id].door_id.size(); j++ ){
            int i = room[room_id].door_id[j];
            int temp_x, temp_y;
            switch (j) {
                case 0 : // left
                    temp_x = room_x - 1;
                    temp_y = room_y;
                    break;
                case 1 : // right
                    temp_x = room_x + 1;
                    temp_y = room_y;
                    break;
                case 2 : // down
                    temp_x = room_x;
                    temp_y = room_y - 1;
                    break;
                case 3 : // up
                    temp_x = room_x;
                    temp_y = room_y + 1;
                    break;
                default :
                    temp_x = -1000;
                    temp_y = -1000;
                    break;
            }
            int temp_id = GetRoomId(temp_x, temp_y);
            if(door[i].state() == 1 && temp_id >= 0){
                door[i].SetState(2);
            }
        }
    }

    player.Behave(curr_time);
    for(int i : room[room_id].enemy_id){
        enemy[i].Behave(curr_time);
    }
    for(int i : room[room_id].obstacle_id){
        obstacle[i].Behave(curr_time);
    }
    for(int i = 0; i < player_bullet.size(); i++){
        player_bullet[i].Behave(curr_time);
    }
    for(int i = 0; i < enemy_bullet.size(); i++){
        enemy_bullet[i].Behave(curr_time);            
    }
    for(int i : room[room_id].tool_id){
        tool[i].Behave(curr_time);
    }
    for(int i : room[room_id].door_id){
        door[i].Behave(curr_time);
    }

    FsPushOnPaintEvent();
    FsSleep(10);
}

void RenderSwitch(void *void_ptr){
    Game* game_ptr = (Game*) void_ptr;

    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    // Current room
    for (int i : game_ptr->room[game_ptr->room_id].tile_id) {
        game_ptr->tile[i].Draw(game_ptr->room_offset_x, game_ptr->room_offset_y);
    }
    for(int i : game_ptr->room[game_ptr->room_id].door_id){
        game_ptr->door[i].Draw(game_ptr->room_offset_x, game_ptr->room_offset_y);

    }
    // printf("(%.2f, %.2f)\n", game_ptr->room_offset_x, game_ptr->room_offset_y);
    for(int i : game_ptr->room[game_ptr->room_id].tool_id){
        game_ptr->tool[i].Draw(game_ptr->room_offset_x, game_ptr->room_offset_y);
    }
    for(int i : game_ptr->room[game_ptr->room_id].enemy_id){
        game_ptr->enemy[i].Draw(game_ptr->room_offset_x, game_ptr->room_offset_y);
    }
    for(int i : game_ptr->room[game_ptr->room_id].obstacle_id){
        game_ptr->obstacle[i].Draw(game_ptr->room_offset_x, game_ptr->room_offset_y);
    }

    // Next room
    for (int i : game_ptr->room[game_ptr->next_id].tile_id) {
        game_ptr->tile[i].Draw(game_ptr->next_offset_x, game_ptr->next_offset_y);
    }
    for(int i : game_ptr->room[game_ptr->next_id].door_id){
        game_ptr->door[i].Draw(game_ptr->next_offset_x, game_ptr->next_offset_y);
    }
    for(int i : game_ptr->room[game_ptr->next_id].tool_id){
        game_ptr->tool[i].Draw(game_ptr->next_offset_x, game_ptr->next_offset_y);
    }
    for(int i : game_ptr->room[game_ptr->next_id].enemy_id){
        game_ptr->enemy[i].Draw(game_ptr->next_offset_x, game_ptr->next_offset_y);
    }
    for(int i : game_ptr->room[game_ptr->next_id].obstacle_id){
        game_ptr->obstacle[i].Draw(game_ptr->next_offset_x, game_ptr->next_offset_y);
    }

    // Draw player
    game_ptr->player.Draw();


    for(int i = 0; i < game_ptr->player_bullet.size(); i++){
        game_ptr->player_bullet[i].Draw();
    }
    for(int i = 0; i < game_ptr->enemy_bullet.size(); i++){
        game_ptr->enemy_bullet[i].Draw();
    }
    FsSwapBuffers();
}

bool Game::SwitchRoom(int direction){
    int next_x, next_y;
    double room_offset_xrange[2] = {0};
    double room_offset_yrange[2] = {0};
    double next_offset_xrange[2] = {0};
    double next_offset_yrange[2] = {0};
    double player_x = player.x();
    double player_y = player.y();
    double player_destination_x = player_x;
    double player_destination_y = player_y;
    switch (direction) {
        case 0 : // left
            next_x = room_x - 1;
            next_y = room_y;
            room_offset_xrange[1] = -MyDraw::WIN_LENGTH_METER;
            next_offset_xrange[0] = MyDraw::WIN_LENGTH_METER;
            player_destination_x = 13.5;
            break;
        case 1 : // right
            next_x = room_x + 1;
            next_y = room_y;
            room_offset_xrange[1] = MyDraw::WIN_LENGTH_METER;
            next_offset_xrange[0] = -MyDraw::WIN_LENGTH_METER;
            player_destination_x = 1.5;
            break;
        case 2 : // down
            next_x = room_x;
            next_y = room_y - 1;
            room_offset_yrange[1] = -MyDraw::WIN_HEIGHT_METER;
            next_offset_yrange[0] = MyDraw::WIN_HEIGHT_METER;
            player_destination_y = 8.5;
            break;
        case 3 : // up
            next_x = room_x;
            next_y = room_y + 1;
            room_offset_yrange[1] = MyDraw::WIN_HEIGHT_METER;
            next_offset_yrange[0] = -MyDraw::WIN_HEIGHT_METER;
            player_destination_y = 1.5;
            break;
        default :
            next_x = -1;
            next_y = -1;
            break;
    } 
    next_id = GetRoomId(next_x, next_y);
    if(next_id < 0){
        return false;
    }

    FsRegisterOnPaintCallBack(RenderSwitch, this);

    const int frame_num = 20;
    double dx = (room_offset_xrange[1] - room_offset_xrange[0]) / ((double) frame_num);
    double dy = (room_offset_yrange[1] - room_offset_yrange[0]) / ((double) frame_num);
    double player_dx = (player_destination_x - player_x) / ((double) frame_num);
    double player_dy = (player_destination_y - player_y) / ((double) frame_num);
    player.SetXY(player_destination_x, player_destination_y);

    for(int i = 0; i <= frame_num; i++){
        FsPollDevice();
        double double_i = (double) i;
        room_offset_x = room_offset_xrange[0] + dx * double_i;
        room_offset_y = room_offset_yrange[0] + dy * double_i;
        next_offset_x = next_offset_xrange[0] + dx * double_i;
        next_offset_y = next_offset_yrange[0] + dy * double_i;
        // player.SetXY(player_x + player_dx * double_i, player_y + player_dy * double_i);

        player.Behave(curr_time);

        FsPushOnPaintEvent();
        FsSleep(5);
    }

    curr_time = (double) ( FsSubSecondTimer() - start_time ) / 1000;
    player.SetNotMove(curr_time);


    room_status[room_id] = 2;
    room_status[next_id] = 1;

    room_id = next_id;
    room_x = next_x;
    room_y = next_y;
    flag_switch_direction = -1;
    flag_player_not_check_collision_ = true;



    return true;
}

void Game::Run(){
    this->RunRoom();
    if(flag_switch_direction >= 0 && flag_switch_direction < 4){
        this->SwitchRoom(flag_switch_direction);
    }
    if(flag_runmenu){
        this->RunMenu();
    }
}

void Game::DrawRoom(){
    // background_png.Draw(0, 0, MyDraw::WIN_HEIGHT_METER,
    //                         MyDraw::WIN_LENGTH_METER, MyDraw::WIN_HEIGHT_METER,
    //                         MyDraw::WIN_LENGTH_METER, -2,
    //                         0, -2);
    for (int i : this->room[this->room_id].tile_id) {
        this->tile[i].Draw();
        // this->obstacle[i].DrawCollision();
    } 
    for(int i : this->room[this->room_id].tool_id){
        this->tool[i].Draw();
    }
    for (int i : this->room[this->room_id].door_id) {
        this->door[i].Draw();
        //this->door[i].DrawCollision();
    }
    for(int i : this->room[this->room_id].enemy_id){
        this->enemy[i].Draw();
        //this->enemy[i].DrawCollision();
    }
    this->player.Draw();
    //this->player.DrawCollision();
    for (int i : this->room[this->room_id].obstacle_id) {
        this->obstacle[i].Draw();
        // this->obstacle[i].DrawCollision();
    }
    for(int i = 0; i < this->player_bullet.size(); i++){
        this->player_bullet[i].Draw();
    }
    for(int i = 0; i < this->enemy_bullet.size(); i++){
        this->enemy_bullet[i].Draw();
    }
    this->bar.Draw();
}

void RenderMenu(void *void_ptr){
    Game* game_ptr = (Game*) void_ptr;

    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    game_ptr->DrawRoom();
    game_ptr->menu.Draw();

    FsSwapBuffers();
}

void Game::RunMenu(){
    // sound_player.Stop(sound_list[1]);
    // sound_player.PlayOneShot(sound_list[1]);

    FsRegisterOnPaintCallBack(RenderMenu, this);
    bool flag_menu_terminate = false;
    this->flag_runmenu = false;
    while(!flag_menu_terminate){

        FsPollDevice();
        auto key=FsInkey();        

        int lb,mb,rb,mx,my;
        int mouse_event=FsGetMouseEvent(lb,mb,rb,mx,my);
        double mouse_x, mouse_y;
        MyDraw::Pixel2Meter(mx, my, mouse_x, mouse_y);

        int flag_win_state = 0;

        if(player.state() <= 0){
            flag_win_state = 1;
            menu.win_button.SetState(flag_win_state);
        }
        else if(boss_num <= 0){
            flag_win_state = 2;
            menu.win_button.SetState(flag_win_state);
        }
        else{
            menu.win_button.SetState(flag_win_state);
        }

        if (flag_win_state == 0){
            if(GameObject::CheckCollision(menu.resume_button, mouse_x, mouse_y)){
                menu.resume_button.SetState(2);
            }
            else{
                menu.resume_button.SetState(1);
            }
        }
        else{
            menu.resume_button.SetState(0);
        }
        if(GameObject::CheckCollision(menu.restart_button, mouse_x, mouse_y)){
            menu.restart_button.SetState(2);
        }
        else{
            menu.restart_button.SetState(1);
        }
        if(GameObject::CheckCollision(menu.exit_button, mouse_x, mouse_y)){
            menu.exit_button.SetState(2);
        }
        else{
            menu.exit_button.SetState(1);
        }

        if(lb == 1){
            if(menu.resume_button.state() == 2){
                flag_menu_terminate = true;
            }
            else if(menu.restart_button.state() == 2){
                flag_menu_terminate = true;
                this->Initialize();
                return;
            }
            else if(menu.exit_button.state() == 2){
                flag_menu_terminate = true;
                this->terminate = true;
            }
        }

        FsPushOnPaintEvent();
        FsSleep(10);
    }
}
