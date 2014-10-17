#include "Bomber.h"

int Bomb::draw(Screen & s, Image & to_draw)    
{
    Uint32 k = SDL_GetTicks() - time;
    SDL_Rect srcrect;
    srcrect.y = 0;
    srcrect.h = 13;
    srcrect.w = 16;
    srcrect.x = 0;
    if (k > fuse_length / 3)
        srcrect.x = 16;
    else if (k > fuse_length / 1.5)
        srcrect.x = 32;
    SDL_Rect destrect;
    destrect.w = srcrect.w;
    destrect.h = srcrect.h;
    destrect.x = pos[0];
    destrect.y = pos[1];
    return to_draw.draw(s, &srcrect, &destrect);
}


Bomber::Bomber(const std::string & image_file,
               const std::string & bomb_file, Screen & s)
    : img(Image(image_file, s)), bomb_img(Image(bomb_file, s))
{
    health = 2;
    alive = true;
    lives = 3;
    direction = 0;
    active_time = SDL_GetTicks();
    horizontal = 0;
    vertical = 0;
    frame = 0;
    speed = 180;
    pos[0] = 100;
    pos[1] = 100;
    int x = 2;
    int y = 1;
    int w = 18;
    int h = 27;
    for (int i = 0; i < 6; ++i)
    {
        SDL_Rect a = {x, y, w, h};
        walk_down.push_back(a);
        if (i % 2 == 0) x += 18;
        else x += 19;
    }
    for (int i = 0; i < 6; ++i)
    {
        SDL_Rect a = {x, y, w, h};
        walk_right.push_back(a);
        if (i % 2 == 0) x += 17;
        else x += 17;
    }
    x = 2;
    y = 31;
    for (int i = 0; i < 6; ++i)
    {
        SDL_Rect a = {x, y, w, h};
        walk_up.push_back(a);
        if (i % 2 == 0) x += 18;
        else x += 19;
    }
    for (int i = 0; i < 6; ++i)
    {
        SDL_Rect a = {x, y, w, h};
        walk_left.push_back(a);
        if (i % 2 == 0) x += 17;
        else x += 17;
    }
    animation = &walk_down;
}

Bomber::Bomber()
{
    direction = 0;
    frame = 0;
}

int Bomber::draw(Screen & s)
{
    SDL_Rect p;
    p.x = pos[0];
    p.y = pos[1];
    p.w = (*animation)[frame].w;
    p.h = (*animation)[frame].h;
    if (img.draw(s, &(*animation)[frame], &p) != 0)
        return -1;
    for (int i = 0; i < active_bomb.size(); ++i)
    {
        if (active_bomb[i].draw(s, bomb_img) != 0)
            return -1;
    }
    return 0;
}


void Bomber::set_animation(int i)
{
    if (i == 0) animation = &walk_down;
    else if (i == 1) animation = &walk_right;
    else if (i == 2) animation = &walk_up;
    else if (i == 3) animation = &walk_left;
}


void Bomber::reset_frame(int amount, int frames)
{
    if (amount==-1)frame = 0;
    frame = amount % frames;
}


void Bomber::inc_frame()
{
    Uint32 temp = SDL_GetTicks();
    if (temp - frame_timer > 100)
    {
        frame += (temp - frame_timer) / 100;
        frame_timer = temp;
    }
    if (frame >= animation->size())
        reset_frame(frame,animation->size());
}


void Bomber::update()
{
    for (int i = 0; i < active_bombs.size(); ++i)
    {
        active_bomb[i].tick();
    }
    if (!is_active())
    {
        pos[1] += vertical;
        pos[0] += horizontal;
        stop();
    }
    else
    {
        if (abs(vertical) > 0 || abs(horizontal) > 0)
        {
            travel_time = SDL_GetTicks();
            pos[1] += vertical;
            pos[0] += horizontal;
            horizontal = vertical = 0;
        }
    }
}

void Bomber::stop()
{
    frame = 0;
    frame_timer = SDL_GetTicks();
    travel_time = SDL_GetTicks();
}

SDL_Rect Bomber::get_rect()
{
    SDL_Rect v = {pos[0], pos[1], (*animation)[frame].w, (*animation)[frame].h};
    return v;
}


bool Bomber::is_active()
{
    return (SDL_GetTicks() - active_time < 100);
}


void Bomber::reposition(const Stage & s, int coll)
{
    int block = coll / 10;
    int direction = coll % 10;
    std::vector<SDL_Rect> blocks = s.get_blocks();
    switch(direction)
    {
        case COLLIDE_TOP:
            pos[1] = blocks[block].y - (*animation)[frame].h - 1;
            break;
        case COLLIDE_BOT:
            pos[1] = blocks[block].y + blocks[block].h + 1;
            break;
        case COLLIDE_LEFT:
            pos[0] = blocks[block].x - (*animation)[frame].w - 1;
            break;
        case COLLIDE_RIGHT:
            pos[0] = blocks[block].x + blocks[block].w + 1;
            break;
    }
    
}


int Bomber::get_travel_distance() const
{
    Uint32 now = SDL_GetTicks();
    return speed * (now - travel_time) / 1000;
}


void Bomber::move_up()
{
    direction = 2;
    vertical = -get_travel_distance();
    active_time = SDL_GetTicks();
}


void Bomber::move_down()
{
    direction = 0;
    vertical = get_travel_distance();
    active_time = SDL_GetTicks();
}


void Bomber::move_left()
{
    direction = 3;
    horizontal = -get_travel_distance();
    active_time = SDL_GetTicks();
}


void Bomber::move_right()
{
    direction = 1;
    horizontal = get_travel_distance();
    active_time = SDL_GetTicks();
}


std::string Bomber::send_info(const std::string & name)
{
    std::string ret = "bmr:";
    std::string k = (alive ? "1" : "0");
    ret += name + "|";
    ret += k + '|';
    ret += to_string(pos[0]) + '|';
    ret += to_string(pos[1]) + '|';
    ret += to_string(frame) + '|';
    ret += to_string(direction) + '|';
    return ret;
}

void Bomber::set(const std::string & s)
{
    std::vector<int> v = get_ints(s);
    if (v[0] == 0) alive = false;
    else alive = true;
    pos[0] = v[1];
    pos[1] = v[2];
    frame = v[3];
    direction = v[4];
    set_animation(v[4]);
}

void Bomber::drop_bomb()
{
    Bomb drop(pos[0], pos[1], TIME_FOR_BOMB_EX[bombtype]);
    active_bomb.push_back(drop);
}
