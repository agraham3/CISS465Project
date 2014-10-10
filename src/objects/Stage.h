#ifndef STAGE_H
#define STAGE_H

#include "vec2d.h"
#include "SDL.h"
#include "includes.h"

stuct Line
{
    vec2d a;
    vec2d b;
};

class Stage
{
public:
    Stage();
    bool collision(const SDL_Rect & rect);
    bool draw();
private:
    Line border[4];
    std::vector<SDL_Rect> Blocks;
};

#endif
