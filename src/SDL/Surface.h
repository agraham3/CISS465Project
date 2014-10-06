#ifndef SURFACE_H
#define SURFACE_H
#include <SDL2/SDL.h>
#include "includes.h"

class Surface_ERROR
{
public:
    Surface_ERROR(const std::string & m)
        : message(m)
    {}
    std::string get_message() {return message;}
private:
    std::string message;
};

class Surface
{
public:
    Surface(int W, int H, const char * t);
    Surface(const char * t, int W, int H);
    ~Surface();
private:
    int WIDTH;
    int HEIGHT;
    std::string title;
    SDL_Window * window;
};

#endif
