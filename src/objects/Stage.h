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

class Stage
{
public:
    Stage(Screen & s, std::string image_file="assets/pic/bomberman_stage.jpg");
    bool collision(const SDL_Rect & rect);
    bool draw();
private:
    Line border[4];
    std::vector<SDL_Rect> blocks;
    Image img;
};

#endif
