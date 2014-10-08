#include "Surface.h"
void Surface::clear()
{
    SDL_RenderClear(renderer);
}

void Surface::update()
{
    SDL_RenderPresent(renderer);
}


void Surface::set_clear(int _r, int _g, int _b, int a=255)
{
    r = _r;
    g = _g;
    b = _b;
    alpha = a;
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
}


Surface::Surface(const char * t, int w, int h, int _r, int _g, int _b, int _a)
    : WIDTH(w), HEIGHT(h), title(t), r(_r), g(_g), b(_b), alpha(_a)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        Surface_ERROR e(std::string("SDL_Init_Error:") + SDL_GetError());
        throw e;
    }
    else
    {
        window = SDL_CreateWindow(t, SDL_WINDOWPOS_UNDEFINED,
                                  SDL_WINDOWPOS_UNDEFINED, w, h, SDL_WINDOW_SHOWN);
        if (window == NULL)
        {
            WIDTH=-1;
            HEIGHT=-1;
            Surface_ERROR e(std::string("SDL_CreateWindowError") + SDL_GetError());
            throw e;
        }
        else
        {   
            renderer = SDL_CreateRenderer(window, -1, 0);
            SDL_SetRenderDrawColor(renderer, r, g, b, alpha);
        }
    }
}


Surface::~Surface()
{
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
}
