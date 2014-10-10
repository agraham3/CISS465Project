#ifndef BOMBER_H
#define BOMBER_H

#include "includes.h"
#include "vec2d.h"
#include "Screen.h"
#include "Image.h"

class Bomber
{
public:
    Bomber(std::string image_file, Screen & s);
    void draw(Screen & s);
    void set_animation(int i = 0);
    void reset_frame(int amount=-1, int frames=5);
    void inc_frame(int num_frames=5);
    void update();
    bool is_active();
    void move_up();
    void move_down();
    void move_left();
    void move_right();
private:
    std::vector<SDL_Rect> walk_down;
    std::vector<SDL_Rect> walk_right;
    std::vector<SDL_Rect> walk_up;
    std::vector<SDL_Rect> walk_left;
    int speed;
    int frame;
    Uint32 stand_still;
    Uint32 frame_timer;
    std::vector<SDL_Rect> *animation;
    Image img;
    vec2d pos;
    
};

#endif
