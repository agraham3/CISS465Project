#ifndef TEXTURE_H
#define TEXTURE_H

#include "Screen.h"
#include "includes.h"

class Image
{
public:
    Image(const std::string & image_name);
    void draw(Screen & screen);
private:
    SDL_Texture * texture;
};

#endif
