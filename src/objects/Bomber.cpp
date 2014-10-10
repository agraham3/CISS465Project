#include "Bomber.h"

Bomber::Bomber(std::string image_file, Screen & s)
    : animation(NULL), img(Image(image_file, s))
{
    frame = 0;
    speed = 3;
    pos[0] = 100;
    pos[1] = 100;
    int x = 2;
    int y = 1;
    int w = 18;
    int h = 29;
    for (int i = 0; i < 6; ++i)
    {
        SDL_Rect a = {x, y, w, h};
        walk_down.push_back(a);
        if (i % 2 == 0) x += 18;
        else x += 19;
    }
    for (int i = 0; i < 6; ++i)
    {
        SDL_Rect a = {x, y, w, h};
        walk_right.push_back(a);
        if (i % 2 == 0) x += 17;
        else x += 17;
    }
    x = 2;
    y = 31;
    for (int i = 0; i < 6; ++i)
    {
        SDL_Rect a = {x, y, w, h};
        walk_up.push_back(a);
        if (i % 2 == 0) x += 18;
        else x += 19;
    }
    for (int i = 0; i < 6; ++i)
    {
        SDL_Rect a = {x, y, w, h};
        walk_left.push_back(a);
        if (i % 2 == 0) x += 17;
        else x += 17;
    }
    animation = &walk_down;
}
