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
    
    void draw(Screen & s)
    {
        SDL_Rect p;
        p.x = pos[0];
        p.y = pos[1];
        p.w = (*animation)[frame].w;
        p.h = (*animation)[frame].h;
        img.draw(s, &(*animation)[frame], &p);
    }
    void set_animation(int i = 0) {
        if (i == 0) animation = &walk_down;
        else if (i == 1) animation = &walk_right;
        else if (i == 2) animation = &walk_up;
        else if (i == 3) animation = &walk_left;
    }
    void reset_frame() {frame = 0;}
    void inc_frame() {frame++; if (frame > 5) reset_frame();}
    void move_up() {pos[1] -= speed;}
    void move_down() {pos[1] += speed;}
    void move_left() {pos[0] -= speed;}
    void move_right() {pos[0] += speed;}
private:
    std::vector<SDL_Rect> walk_down;
    std::vector<SDL_Rect> walk_right;
    std::vector<SDL_Rect> walk_up;
    std::vector<SDL_Rect> walk_left;
    int speed;
    int frame;
    std::vector<SDL_Rect> *animation;
    Image img;
    vec2d pos;
    
};

#endif
