#include "Bomber.h"

int Bomb::draw(Screen & s, Image & to_draw)    
{
    
    if (exp_remaining != -1)
    {
        SDL_Rect destrect;
        destrect.w = exp_rect.w * 6;
        destrect.h = exp_rect.h * 6;
        destrect.x = pos[0];
        destrect.y = pos[1];
        int k = to_draw.draw(s, &exp_rect, &destrect);
        return k;
    }
    SDL_Rect srcrect;
    srcrect.y = 0;
    srcrect.h = 13;
    srcrect.w = 16;
    srcrect.x = 0;
    if (time_left < fuse_length / 10)
        srcrect.x = 32;
    else if (time_left < fuse_length / 2)
        srcrect.x = 16;
    SDL_Rect destrect;
    destrect.w = srcrect.w;
    destrect.h = srcrect.h;
    destrect.x = pos[0];
    destrect.y = pos[1];
    return to_draw.draw(s, &srcrect, &destrect);
}


void Bomb::get_exp_rect() 
{
    int k = exp_time();
    if (k < EXPLOSION_TIME / 20)
    {
        exp_rect.x = 2;
        exp_rect.y = 4;
        exp_rect.h = 12;
        exp_rect.w = 13;
    }
    else if (k < EXPLOSION_TIME / 13)
    {
        exp_rect.x = 19;
        exp_rect.y = 2;
        exp_rect.w = 15;
        exp_rect.h = 15;
    }
    else if (k < EXPLOSION_TIME / 6)
    {
        exp_rect.x = 36;
        exp_rect.y = 2;
        exp_rect.w = 15;
        exp_rect.h = 15;
    }
    else if (k < EXPLOSION_TIME / 1.2)
    {
        exp_rect.x = 53;
        exp_rect.y = 2;
        exp_rect.w = 15;
        exp_rect.h = 15;
    }
    else if (k < EXPLOSION_TIME)
    {
        exp_rect.x = 70;
        exp_rect.y = 2;
        exp_rect.w = 15;
        exp_rect.h = 15;
    }
    else if (k >= EXPLOSION_TIME)
    {
        alive = false;
    }
}


void Bomb::explode()
{
    if (explosion_time == 0)
    {
        pos[0] -= 15 * 3;
        pos[1] -= 15 * 3;
        explosion_time = SDL_GetTicks();
    }
    else
    {
        int k = exp_time();
        if (k < EXPLOSION_TIME / 20)
        {
            exp_rect.x = 2;
            exp_rect.y = 4;
            exp_rect.h = 12;
            exp_rect.w = 13;
        }
        else if (k < EXPLOSION_TIME / 13)
        {
            exp_rect.x = 19;
            exp_rect.y = 2;
            exp_rect.w = 15;
            exp_rect.h = 15;
        }
        else if (k < EXPLOSION_TIME / 6)
        {
            exp_rect.x = 36;
            exp_rect.y = 2;
            exp_rect.w = 15;
            exp_rect.h = 15;
        }
        else if (k < EXPLOSION_TIME / 1.2)
        {
            exp_rect.x = 53;
            exp_rect.y = 2;
            exp_rect.w = 15;
            exp_rect.h = 15;
        }
        else if (k < EXPLOSION_TIME)
        {
            exp_rect.x = 70;
            exp_rect.y = 2;
            exp_rect.w = 15;
            exp_rect.h = 15;
        }
        else if (k >= EXPLOSION_TIME)
        {
            alive = false;
        }
    }
}

Bomber::Bomber(const std::string & image_file,
               const std::string & bomb_file, const std::string & exp_file,
               const std::string & arrow_name, Screen & s)
    : img(Image(image_file, s)), bomb_img(Image(bomb_file, s)), exp_img(Image(exp_file, s)), arrow(Image(arrow_name, s)), skull(Image("assets/pic/skull-crossbones.png", s))
{
    kills = 0;
    deaths = 0;
    drops = 0;
    sk.w = 18;
    sk.h = 14;
    losk.x = 0;
    losk.y = 0;
    losk.w = sk.w;
    losk.h = sk.h;
    health = MAX_HEALTH;
    alive = true;
    lives = MAX_LIVES;
    direction = -1;
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
    lives = MAX_LIVES;
}

int Bomber::draw(Screen & s, int okay)
{
    SDL_Rect p;
    p.x = pos[0];
    p.y = pos[1];
    p.w = (*animation)[frame].w;
    p.h = (*animation)[frame].h;
    for (int i = 0; i < active_bomb.size(); ++i)
    {
        if (!active_bomb[i].is_exploding())
        {
            if (active_bomb[i].draw(s, bomb_img) != 0)
                return -1;
        }
        else
        {
            if (active_bomb[i].draw(s, exp_img) != 0)
                return -1;
        }
    }
    
    if (okay != 2 || is_alive()) //don't draw if not main player and is dead
    {
        if (img.draw(s, &(*animation)[frame], &p) != 0)
            return -1;
        
        SDL_Rect a = {0, 0, 10, 12};
        p.y -= 13;
        p.x += 3;
        p.w = 10;
        p.h = 12;
        if (is_alive())
            if (arrow.draw(s, &a, &p) != 0)
                return -1;
    }
    
    if (okay != 2 && !is_alive()) //draw skull to show self is dead
    {
        sk.x = pos[0];
        sk.y = pos[1] - 17;
        skull.draw(s, &losk, &sk);
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


void Bomber::update(bool enemy)
{
    if (!is_alive() && !enemy)
    {
        if (get_lives() > 0)
        {
            Uint32 resp = SDL_GetTicks() - time_died;
            if (resp > RESPAWN_TIME)
            {
                alive = true;
                --lives;
                health = MAX_HEALTH;
            }
        }
    }
    std::vector<int> to_erase;
    for (int i = 0; i < active_bomb.size(); ++i)
    {
        if (!active_bomb[i].is_alive())
        {
            to_erase.push_back(i);
        }
        else
        {
            active_bomb[i].update();
        }
        active_bomb[i].tick();
    }
    for (int i = 0; i < to_erase.size(); ++i)
    {
        active_bomb.erase(active_bomb.begin() + i);
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


void Bomber::reposition(const std::vector<SDL_Rect> & blocks, int coll, bool up)
{
    int block = coll / 10;
    int direction = coll % 10;
    switch(direction)
    {
        case COLLIDE_TOP:
            pos[1] = blocks[block].y - (*animation)[frame].h - 1;
            break;
        case COLLIDE_BOT:
            pos[1] = blocks[block].y + blocks[block].h + 1;
            if (up) pos[1] -= 22;
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
    ret += to_string((int)active_bomb.size()) + '|';
    for (int i = 0; i < active_bomb.size(); ++i)
    {
        ret += to_string(active_bomb[i].get_pos()[0]) + '|';
        ret += to_string(active_bomb[i].get_pos()[1]) + '|';
        ret += to_string(active_bomb[i].get_time_left()) + '|';
        ret += to_string(active_bomb[i].exp_time()) + '|';
    }
    ret += to_string(lives) + '|';
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
    active_bomb.resize(v[5]);
    int i = 6;
    for (int j = 0; j < active_bomb.size(); ++j)
    {
        active_bomb[j].set_x(v[i]);
        ++i;
        active_bomb[j].set_y(v[i]);
        ++i;
        active_bomb[j].set_time_left(v[i]);
        ++i;
        active_bomb[j].set_exp(v[i]);
        ++i;
    }
    lives = v[v.size() - 1];
}


void Bomber::drop_bomb()
{
    if (active_bomb.size() < MAX_BOMBS_PER_PLAYER && is_alive())
    {

        drops++;
        Bomb drop(pos[0], pos[1], TIME_FOR_BOMB_EX[bombtype]);
        active_bomb.push_back(drop);
    }
}


bool Bomber::collide(const SDL_Rect & danger) const 
{
    SDL_Rect self_rect = {pos[0], pos[1], (*animation)[frame].w, (*animation)[frame].h};
    return (collided(self_rect, danger) != -1);
}



int Bomber::any_collisions(const std::vector<Bomb> & dangers) const
{
    for (int i = 0; i < dangers.size(); ++i)
    {
        if (dangers[i].is_exploding())
        {
            if (collide(dangers[i].get_rect()))
            {
                return i;
            }
        }
    }
    return -1;
}
