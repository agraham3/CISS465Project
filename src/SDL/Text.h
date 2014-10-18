#ifndef TEXT_H
#define TEXT_H

#include "includes.h"
#include "SDL.h"
#include "constants.h"
#include "Screen.h"

class Text
{
public:
    Text(Screen & s, SDL_rect loc, int fontsize=24, SDL_Color clr=BLACK, std::string m="")
        : message(m), color(clr), rect(loc)
    {
        font = TTF_OpenFont("Sans.ttf", fontsize);
    }
    ~Text()
    {
        SDL_DestroyTexture(texture);
        TTF_CloseFont(font);
    }
    void draw(Screen & s)
    {
        SDL_Surface * sm = TTF_RenderText_Solid(font, message.c_str(), color);
        SDL_Texture * tm = SDL_CreateTextureFromSurface(s.get_renderer(), sm);
        SDL_RenderCopy(s.get_renderer(), tm, NULL, &rect);
        SDL_FreeSurface(sm);
        SDL_DestroyTexture(tm);
    }
    std::string & get_message() {return message;}
    
private:
    SDL_Rect rect;
    std::string message;
    TTF_Font * font;
    SDL_Color color;
};

#endif
