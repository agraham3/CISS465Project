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
    Image()
        :texture(NULL)
    {}
    ~Image();
    void draw(Screen & screen, SDL_Rect * src=NULL, SDL_Rect * dest=NULL);
private:
    SDL_Texture * texture;
};

#endif
