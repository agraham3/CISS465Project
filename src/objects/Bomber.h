#ifndef BOMBER_H
#define BOMBER_H

#include "includes.h"
#include <cmath>
#include "vec2d.h"
#include "constants.h"
#include "Screen.h"
#include "Image.h"
#include "includes.h"
#include "Stage.h"



class Bomb
{
public:
    Bomb(vec2d p, Uint32 fuse_length, Screen & s, const std::string & file)
        : pos(p), time(SDL_GetTicks()), img(Image(file, s)), power(2),
          alive(true), fuse_length(time)
    {}
    
    
    void tick()
    {
        int k = SDL_GetTicks();
        if (k - time > fuse_length)
        {
            explode();
        }
    }

    //implement explosion
    void explode();
    
    
private:
    vec2d pos;
    Uint32 time;
    Image img;
    Uint32 fuse_length;
    bool alive;
    int power;
    int animation;
    std::vector<SDL_Rect> explosion;
};

class Bomber
{
public:
    Bomber(std::string image_file, Screen & s);
    Bomber();
    void draw(Screen & s);
    void set_animation(int i = 0);
    void reset_frame(int amount=-1, int frames=5);
    void inc_frame();
    void update();
    SDL_Rect get_rect();
    bool is_active();
    bool is_alive() {return alive;}
    void reposition(const Stage & s, int coll);
    int get_travel_distance() const;
    void move_up();
    void move_down();
    void move_left();
    void move_right();
    void set_pos(int x, int y) { pos[0] = x; pos[1] = y; }
    void set_frame(int f) {frame = f;}
    std::string send_info(const std::string & name);
    void set(const std::string & info);
    void drop_bomb(Screen & s);
    void stop();
    int get_direction() {return direction;}
    int get_speed() {return speed;}

    SDL_Texture * get_img() {return img.get_texture();}
    
private:
    std::vector<SDL_Rect> walk_down;
    std::vector<SDL_Rect> walk_right;
    std::vector<SDL_Rect> walk_up;
    std::vector<SDL_Rect> walk_left;
    int speed; // pixels per second
    int frame;
    int direction;
    Uint32 travel_time;
    Uint32 active_time;
    int vertical;
    int horizontal;
    Uint32 frame_timer;
    std::vector<SDL_Rect> * animation;
    std::vector<Bomb> active;
    Image img;
    vec2d pos;
    int bombtype;
    bool alive;
    int health;
    int lives;
    
};

#endif
