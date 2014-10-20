#include "GraphicText.h"


Text::Text(Screen & s, SDL_Rect loc, bool _secret, SDL_Color clr, std::string hider, int fontsize, std::string m)
    : message(m), color(clr), rect(loc), secret(_secret), text_hider(Image(hider, s))
{
    font = TTF_OpenFont("fonts/FreeSans.ttf", fontsize);
}

Text::~Text()
{
    TTF_CloseFont(font);
}

Text::Text(Screen & s, bool _secret, SDL_Color clr, std::string hider, int fontsize, std::string m)
    : message(m), color(clr), secret(_secret), text_hider(Image(hider, s))
{
    rect.x = 0;
    rect.y = 0;
    rect.h = 0;
    rect.w = 0;
    font = TTF_OpenFont("fonts/FreeSans.ttf", fontsize);
}


void Text::draw(Screen & s)
{
    if (secret)
    {
        SDL_Rect k = rect;
        k.w = 20;
        k.y += 28;
        k.h = 22;
        for (int i = 0; i < message.size(); ++i)
        {
            text_hider.draw(s, NULL, &k);
            k.x += 20;
        }
        return;
    }
    SDL_Surface * sm = TTF_RenderText_Solid(font, message.c_str(), color);
    SDL_Texture * tm = SDL_CreateTextureFromSurface(s.get_renderer(), sm);
    SDL_RenderCopy(s.get_renderer(), tm, NULL, &rect);
    SDL_FreeSurface(sm);
    SDL_DestroyTexture(tm);
}

