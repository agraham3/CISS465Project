#include "GraphicText.h"


Text::Text(Screen & s, SDL_Rect loc, int fontsize, SDL_Color clr, std::string m)
    : message(m), color(clr), rect(loc)
{
    font = TTF_OpenFont("fonts/FreeSans.ttf", fontsize);
}

Text::~Text()
{
    TTF_CloseFont(font);
}



void Text::draw(Screen & s)
{
    SDL_Surface * sm = TTF_RenderText_Solid(font, message.c_str(), color);
    SDL_Texture * tm = SDL_CreateTextureFromSurface(s.get_renderer(), sm);
    SDL_RenderCopy(s.get_renderer(), tm, NULL, &rect);
    SDL_FreeSurface(sm);
    SDL_DestroyTexture(tm);
}

