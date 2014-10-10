#ifndef TEXTURE_H
#define TEXTURE_H

#include "Screen.h"
#include "SDL_image.h"
#include "includes.h"
class Image_error
{
public:
    Image_error(const std::string & e)
        : error(e)
    {}
    std::string error;
};
class Image
{
public:
    Image(const std::string & image_name, Screen & screen);
    void draw(Screen & screen);
private:
    SDL_Texture * texture;
};

#endif
