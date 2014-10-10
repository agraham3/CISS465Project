#ifndef SCREEN_H
#define SCREEN_H
#include "SDL.h"
#include "SDL_image.h"
#include "includes.h"

class Screen_ERROR
{
public:
    Screen_ERROR(const std::string & m)
        : message(m)
    {}
    std::string get_message() {return message;}
private:
    std::string message;
};



class Screen
{
public:
    Screen(const char * t, int W, int H, int _r=0, int _g=0, int _b=0, int _a=255);
    ~Screen();
    void clear();
    void update();
    void set_clear(int _r, int _g, int _b, int a);
    SDL_Window * get_window() const {return window;}
    SDL_Renderer * get_renderer() const {return renderer;}
private:
    int WIDTH;
    int HEIGHT;
    std::string title;
    SDL_Window * window;
    SDL_Renderer * renderer;
    std::vector<SDL_Texture *> textures;
    int r, g, b, alpha;
};


#endif
