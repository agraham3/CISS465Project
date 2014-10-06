#ifndef SURFACE_H
#define SURFACE_H
#include "SDL.h"
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

class Renderer
{
public:
    Renderer(SDL_Window * win, int _r, int _g, int _b, int a);
    ~Renderer();
    void clear();
    void set_clear(int _r, int _g, int _b, int a);
private:
    int r, g, b, alpha;
    SDL_Renderer * renderer;
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
