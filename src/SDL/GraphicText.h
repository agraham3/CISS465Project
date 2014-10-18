#ifndef GTEXT_H
#define GTEXT_H

#include "includes.h"
#include "SDL.h"
#include "constants.h"
#include "Screen.h"
#include "SDL_ttf.h"
#include "SDL_image.h"

class Text
{
public:
    Text(Screen & s, SDL_Rect loc, int fontsize=24, SDL_Color clr=GREEN, std::string m="");
    ~Text();
    std::string & get_message() {return message;}
    void draw(Screen & s);
    
private:
    SDL_Rect rect;
    std::string message;
    TTF_Font * font;
    SDL_Color color;
};

#endif
