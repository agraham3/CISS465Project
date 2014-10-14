#include "Image.h"

Image::Image(const std::string & file_name, Screen & screen)
{
    SDL_Surface * temp = IMG_Load(file_name.c_str());
    if (temp == NULL)
    {
        Image_error e(std::string("Failed to load image ") + file_name \
                      + std::string("! SDL_image ERROR: ") + IMG_GetError() \
                      + '\n');
        throw e;
                      
    }
    texture = SDL_CreateTextureFromSurface(screen.get_renderer(), temp);
    SDL_FreeSurface(temp);
}

Image::~Image()
{
    SDL_DestroyTexture(texture);
}
void Image::draw(Screen & screen, SDL_Rect * srcrect, SDL_Rect * destrect)
{
    SDL_RenderCopy(screen.get_renderer(), texture, srcrect, destrect);
}
