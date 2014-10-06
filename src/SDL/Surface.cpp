#include "Surface.h"

Renderer::Renderer(SDL_Window * win, int _r=0, int _g=0, int _b=0, int a=255)
    : r(_r), g(_g), b(_b), alpha(a)
{
    renderer = SDL_CreateRenderer(win, -1, 0);
    SDL_SetRenderDrawColor(renderer, r, g, b, alpha);
}

Renderer::~Renderer()
{
    SDL_DestroyRenderer(renderer);
}

void Renderer::clear()
{
    SDL_RenderClear(renderer);
}

void Renderer::set_clear(int _r, int _g, int _b, int a=255)
{
    r = _r;
    g = _g;
    b = _b;
    alpha = a;
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
}


Surface::Surface(int w, int h, const char * t)
    : WIDTH(w), HEIGHT(h), title(t)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        Surface_ERROR e(std::string("SDL_Init_Error:") + SDL_GetError());
        throw e;
    }
    else
    {
        window = SDL_CreateWindow(t, SDL_WINDOWPOS_UNDEFINED,
                                  SDL_WINDOWPOS_UNDEFINED, w, h, 0);
        if (window == NULL)
        {
            WIDTH=-1;
            HEIGHT=-1;
            Surface_ERROR e(std::string("SDL_CreateWindowError") + SDL_GetError());
            throw e;
        }
    }
}


Surface::~Surface()
{
    SDL_DestroyWindow(window);
}
