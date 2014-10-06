#include "Surface.h"
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
