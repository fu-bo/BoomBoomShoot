#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP

class Circle;
class PngManager;
#include "fssimplewindow.h"
#include "MyDraw.hpp"
#include "Circle.hpp"
#include "Rect.hpp"
#include <vector>
#include <memory>
#include <math.h>

class GameObject {
private:
    // state of the object: > 0 means exist
    int state_;
protected:
    // scale of the object
    double scale_;
    // x
    double x_;
    // y
    double y_;
    // next x if no collision
    double next_x_;
    // next y if no collision
    double next_y_;
    // flag of whether flip the png (left to right), 1: not flip, -1: flip
    double flip_leftright_;
    // flag of whether flip the png (up to down), 1: not flip, -1: flip
    double flip_updown_;
    // the rotation angle of the png, in degree
    double angle_;
    // velocity in x direction
    double vx_;
    // velocity in x direction
    double vy_;
    // velocity sqrt(vx^2 + vy^2)
    double v_;
    // moving direction for Computer
    int direction_;
    // moving state for Computer
    int moving_state_;
    // fire type
    int fire_mode_;
    // moving angle for object
    double moving_angle_;
    // bullet angle
    double bullet_angle_;
    // if the Computer is bloacking. 0: not blocked
    int blocked_;
    // id of png id for state2pngid_[state_]
    int png_id_id_;
    // specify which state correspond to which pngs
    std::vector<std::vector<int>> state2pngid_;
    // current hp
    double hp_;
    // maximum hp
    double hp_max_;
    // collision circle of an object
    std::vector<Circle> collision_circle_;
    // collision rectangle of an object
    std::vector<Rect> collision_rect_;
    // image manager
    std::shared_ptr<PngManager> png_ptr_;
    // current image id
    int png_id_;
    // game object type, e.g. player = 0, computer = 1, obstacle = 2, bullet = 3 #define TYPE_OBSTACLE 2
    int type_;
    // previous behave time
    double prev_time_;
    // current time
    double curr_time_;
    // time gap
    double time_gap_;
    //
    bool flag_blink_;
    // 
    double blink_prev_time_;
    //
    bool flag_show_;
    //
    double blink_time_gap_;
    // 
    double blink_total_time_gap_;
    //
    double blink_total_prev_time_;
    // attact flag
    bool flag_notmove_;
    // previous attacked time
    double prev_notmove_time_;
    // not move time
    double notmove_time_gap_;
    // not jump state
    bool flag_notjumpstate_;
    // start to disappear state
    int startdisappear_state_;
    // disappera state
    int disappear_state_;

public:
    // constructor
    GameObject();
    // copy constructor
    GameObject(const GameObject & another);
    // destructor
    ~GameObject();
    // copy operator
    GameObject & operator=(const GameObject & another);
    // clean up
    void CleanUp();
    // create based on input value
    void Create();
    // set initial value
    void Reset();
    // deep copy
    void Copy(const GameObject & another);
    // set based on input value
    bool SetPng(std::shared_ptr<PngManager> png_ptr, const std::vector<std::vector<int>>& state2pngid);
    bool SetPng(std::shared_ptr<PngManager> png_ptr, const std::vector<std::vector<int>>& state2pngid, const std::vector<Circle> collision_circle);
    bool SetPng(std::shared_ptr<PngManager> png_ptr, const std::vector<std::vector<int>>& state2pngid, const std::vector<Rect> collision_rect);
    bool SetPng(std::shared_ptr<PngManager> png_ptr, const std::vector<std::vector<int>>& state2pngid, double x_ratio, double y_ratio, double ratio);
    bool SetPng(std::shared_ptr<PngManager> png_ptr, const std::vector<std::vector<int>>& state2pngid, double x_ratio, double y_ratio, double wid_ratio, double hei_ratio);
    // set velocity
    bool SetV(double v);
    // set x, y velocity
    bool SetVxy(double vx, double vy);
    // set scale
    bool SetScale(double scale);
    // set hp or based on ratio if it is >= 0
    int SetHp(double hp, double ratio = -1);
    // set hp max
    int SetHpMax1(double hp_max);
    // lose hp
    int MinusHp(double hp_minus);
    // set x y position
    bool SetXY(double x, double y);
    // set angle
    bool SetAngle(double angle);
    // set flip upside down or left side right
    bool SetFlip(double flip_leftright, double flip_updown);
    // set state
    bool SetState(int state, bool flag_hardset = false);
    // set type
    bool SetType(int type);
    // set the initial direction of the computer
    bool SetDirection(int flag_direction);
    // set the moving state for the computer
    bool SetMovingState(int moving_state);
    // set the mode for firing
    bool SetFireMode(int fire_mode);
    // set the initial bullet angle for computer
    bool SetBulletAngle(double bullet_angle);
    // set the Computer is Blocked or not
    bool SetBlocked(int new_blocked);
    // set disappear state
    bool SetDisappearState(int startdisappear_state, int disappear_state);
    // set not move for a second
    bool SetNotMove(double curr_time);
    // get x position
    double GetX();
    // get y position
    double GetY();
    // check whether the object is inside
    bool IsInside(double margin = 0.0) const;
    // check whether the object's next position is inside
    bool IsInsideNext(double margin = 0.0) const;
    // get minimum hp of two objects and one limitation value
    static double GetMinHp(const GameObject& g1, const GameObject& g2, double hp_minus = 100000000);
    // check collision based on collision circle
    static bool CheckCollision(GameObject& g1, GameObject& g2, int flag_next = 1);
    static bool CheckCollision(GameObject& g1, double x2, double y2);
    // return a collision rectangle
    void GetCollisionRect(int rect_id, Rect &rect, int flag_next = 1) const;
    // return a collision circle
    void GetCollisionCircle(int circle_id, double& x, double& y, double& r, int flag_next = 1) const;
    // draw the current png
    inline void Draw(double alpha = 1) { Draw(0, 0, alpha);}
    void Draw(double offset_x, double offset_y, double alpha = 1);
    // draw collision space
    void DrawCollision() const;
    // behavior of nothing happens, organizes all the behaviors using if or switch
    virtual void Behave(double curr_time);
    // behavior of stay in one position, curr_time may not be used
    virtual void Idle(double curr_time);
    // behavior of dispear, curr_time may not be used
    virtual void Disappear();
    // behavior of showup, curr_time may not be used
    virtual void ShowUp(double curr_time);
    // behavior of stop by other things, curr_time may not be used
    virtual void Stop(double curr_time);
    // behavior of collide with other thigns, for example Stop() if another.type_ == TYPE_OBSTACLE
    virtual void Collide(GameObject& another);
    // behavior of move
    virtual bool Move(double dt, int flag_direction);
    virtual bool Move(double dt);
    // Used by Computer
    virtual bool Move(double player_x, double player_y);
    inline void StopX() {next_x_ = x_;}
    inline void StopY() {next_y_ = y_;}

    // return hp
    double hp() const;
    // return hp_max
    double hp_max() const;
    // return state
    inline int state() const {return state_;}
    // return x
    double x() const;
    // return y
    double y() const;
    // return vx
    double vx() const;
    // return vy
    double vy() const;
    // return type_
    int type() const;
    // return flag_blink_
    bool NotAttackable() const;
};

#endif