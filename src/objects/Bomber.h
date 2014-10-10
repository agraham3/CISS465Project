#ifndef BOMBER_H
#define BOMBER_H

#include "includes.h"
#include "vec2d.h"
#include "Screen.h"
#include "Image.h"

class Bomber
{
public:
    Bomber(std::string image_file, Screen & s, SDL_Rect * a=NULL)
        : animation(a), img(Image(image_file, s))
    {
        pos[0] = 100;
        pos[1] = 100;
    }
    void draw(Screen & s)
    {
        SDL_Rect p;
        p.x = pos[0];
        p.y = pos[1];
        p.w = animation->w;
        p.h = animation->h;
        img.draw(s, animation, &p);
    }
private:
    SDL_Rect *animation;
    Image img;
    vec2d pos;
    
};

#endif
