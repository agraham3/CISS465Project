#ifndef STAGE_H
#define STAGE_H

#include "vec2d.h"
#include "SDL.h"
#include "includes.h"
#include "Screen.h"
#include "Image.h"

struct Line
{
    vec2d a;
    vec2d b;
};
inline 
bool between(int a, int b, int c)
{
    return (a >= b && a <= c) || (a >= c && a <= b);
}


inline
bool collided(SDL_Rect & a, SDL_Rect b)
{
    return ((between(a.x, b.x, b.x + b.w) || between(a.x + a.w, b.x, b.x + b.w))
            && (between(a.y, b.y, b.y + b.h) || between(a.y + a.h, b.y, b.y + b.h)));
}


class Stage
{
public:
    Stage(Screen & s, std::string image_file="assets/pic/bomberman_stage.jpg");
    int collision(const SDL_Rect & rect); //returns the index of collided block
    void draw(Screen & s);
    std::vector<SDL_Rect> get_blocks() {return blocks;}
private:
    Line border[4];
    std::vector<SDL_Rect> blocks;
    Image img;
};

#endif
