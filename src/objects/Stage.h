#ifndef STAGE_H
#define STAGE_H

#include "vec2d.h"
#include "SDL.h"
#include "includes.h"
#include "Screen.h"
#include "Image.h"

#define COLLIDE_TOP 0
#define COLLIDE_BOT 1
#define COLLIDE_LEFT 2
#define COLLIDE_RIGHT 3

struct Line
{
    vec2d a;
    vec2d b;
};
//TODO: move inline to not cpp file.
//NOTE: Keep "bool between(int, int)" in .h because it's small enough"

inline 
bool between(int a, int b, int c)
{
    return (a >= b && a <= c) || (a >= c && a <= b);
}

inline
bool x_crossings(SDL_Rect a, SDL_Rect b)
{
    return  (between(a.x, b.x, b.x + b.w) ||
             between(b.x, a.x, a.x + a.w));
}


inline
bool y_crossings(SDL_Rect a, SDL_Rect b)
{
    return  (between(a.y, b.y, b.y + b.h) ||
             between(b.y, a.y, a.y + a.h));
}


inline
bool collided_top(SDL_Rect moving_rect, SDL_Rect block)
{
    bool across = moving_rect.x > block.x && moving_rect.x + moving_rect.w < block.x + block.w;
    bool over = between(moving_rect.y + moving_rect.h, block.y, block.y + block.h) && moving_rect.y < block.y;
    return across && over;
}

inline
bool collided_bottom(SDL_Rect moving_rect, SDL_Rect block)
{
    
    bool across = moving_rect.x > block.x
        && moving_rect.x + moving_rect.w < block.x + block.w;
    bool under = between(moving_rect.y, block.y, block.y + block.h)
        && moving_rect.y > block.y;
    return across && under;
}

inline
bool collided_left(SDL_Rect moving_rect, SDL_Rect block)
{
    bool y_cross = y_crossings(moving_rect, block);
    bool left = between(block.x, moving_rect.x, moving_rect.x + moving_rect.w);
    return y_cross & left;
}


inline
bool collided_right(SDL_Rect moving_rect, SDL_Rect block)
{
    
    bool y_cross = y_crossings(moving_rect, block);
    bool right = between(moving_rect.x, block.x, block.x + block.w);
    return y_cross & right;
}


inline
int collided(SDL_Rect moving_rect, SDL_Rect block)
{
    if (collided_top(moving_rect, block)) return COLLIDE_TOP;
    if (collided_bottom(moving_rect, block)) return COLLIDE_BOT;
    if (collided_left(moving_rect, block)) return COLLIDE_LEFT;
    if (collided_right(moving_rect, block)) return COLLIDE_RIGHT;
    return -1;
}


class Stage
{
public:
    Stage(Screen & s, std::string image_file="assets/pic/bomberman_stage.jpg");
    int collision(const SDL_Rect & rect) const; //returns the index of collided block
    void draw(Screen & s);
    std::vector<SDL_Rect> get_blocks() const {return blocks;}
    std::vector<SDL_Rect> get_destructibles() {return destructibles;}
    std::vector<int> get_index() {return index;}
    void set_destructibles(const std::string & s);
    void remove(int k);
    int hit_destructible(const SDL_Rect & s, bool sub=false) const;
    void destroy_destructibles(const std::string & s);
    void hit_destructibles(const SDL_Rect & rect, std::vector<int> & d) const;
private:
    Line border[4];
    std::vector<SDL_Rect> blocks;
    std::vector<SDL_Rect> destructibles;
    std::vector<int> index;
    Image img;
    Image dest_img;
};

#endif
