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



class Surface
{
public:
    Surface(const char * t, int W, int H, int _r=0, int _g=0, int _b=0, int _a=255);
    ~Surface();
    void clear();
    void update();
    void set_clear(int _r, int _g, int _b, int a);
    SDL_Window * get_window() const {return window;}
private:
    int WIDTH;
    int HEIGHT;
    std::string title;
    SDL_Window * window;
    SDL_Renderer * renderer;
    SDL_Texture * texture;
    int r, g, b, alpha;
};

#endif
