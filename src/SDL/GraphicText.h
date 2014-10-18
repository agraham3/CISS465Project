#ifndef GTEXT_H
#define GTEXT_H

#include "includes.h"
#include "SDL.h"
#include "constants.h"
#include "Screen.h"
#include "SDL_ttf.h"
#include "Image.h"

class Text
{
public:
    Text(Screen & s, SDL_Rect loc, bool _secret=false,  SDL_Color clr=BLACK, std::string hider="assets/pic/passwordHide.png", int fontsize=24, std::string m="");
    ~Text();
    std::string & get_message() {return message;}
    std::string get_message() const {return message;}
    void draw(Screen & s);
    void set_message(const std::string & m) {message = m;}
    
private:
    SDL_Rect rect;
    Image text_hider;
    bool secret;
    std::string message;
    TTF_Font * font;
    SDL_Color color;
};

#endif
