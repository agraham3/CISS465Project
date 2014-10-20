#include "Stage.h"

Stage::Stage(Screen & s, std::string image_file)
    :img(Image(image_file, s)), dest_img(Image("assets/pic/blocks.png", s))
{
    std::vector<int> lefts;
    lefts.push_back(111);
    for (int i = 0; i < 5; ++i)
    {
        lefts.push_back(lefts[lefts.size() - 1] + 120);
    }
    std::vector<int> tops;
    tops.push_back(90);
    for (int i = 0; i < 4; ++i)
    {
        tops.push_back(tops[tops.size() - 1] + 100);
    }

    for (int i = 0; i < tops.size(); ++i)
    {
        for (int j = 0; j < lefts.size(); ++j)
        {
            SDL_Rect block = {lefts[j], tops[i], 62, 32};
            blocks.push_back(block);
        }
    }
    SDL_Rect left_block = {0, 0, 50, 600};
    blocks.push_back(left_block);
    SDL_Rect top_block = {0, 0, 840, 21};
    blocks.push_back(top_block);
    SDL_Rect right_block = {840, 0, 30, 600};
    blocks.push_back(right_block);
    SDL_Rect bot_block = {0, 590, 840, 21};
    blocks.push_back(bot_block);
}


void Stage::draw(Screen & s)
{
    img.draw(s, NULL, NULL);
    for (int i = 0; i < destructibles.size(); ++i)
    {
        dest_img.draw(s, NULL, &destructibles[i]);
    }
}


int Stage::collision(const SDL_Rect & rect) const
{
    for (int i = 0; i < blocks.size(); ++i)
    {
        int collide = collided(rect, blocks[i]);
        if (collide != -1)
        {
            return i * 10 + collide;
        }
    }
    return -1;
}

int Stage::hit_destructible(const SDL_Rect & rect, bool sub) const
{
    for (int i = 0; i < destructibles.size(); ++i)
    {
        SDL_Rect x = destructibles[i];
        if (sub)
        {
            x.h -= 20;
        }
        int collide = collided(rect, x);
        if (collide != -1)
        {
            return i * 10 + collide;
        }
    }
    return -1;
}

void Stage::hit_destructibles(const SDL_Rect & rect, std::vector<int> & d) const
{
    for (int i = 0; i < destructibles.size(); ++i)
    {
        int collide = collided(rect, destructibles[i]);
        if (collide != -1)
        {
            if (!in_array(index[i], d))
            {
                d.push_back(index[i]);
            }
        }
    }
}

void Stage::set_destructibles(const std::string & s)
{
    std::vector<int> spots = get_ints(s);
    destructibles.resize(0);
    index.resize(0);
    for (int i = 0; i < spots.size(); ++i)
    {
        SDL_Rect to_add = {49 + (61 * (spots[i] % 13)), 40 + (50 * (spots[i] / 13)), 62, 52};
        index.push_back(spots[i]);
        destructibles.push_back(to_add);
    }
    return;
}

void Stage::destroy_destructibles(const std::string & s)
{
    std::vector<int> victims = get_ints(s);
    for (int i = 0; i < victims.size(); ++i)
    {
        remove(victims[i]);
    }
}

void Stage::remove(int k)
{
    for (int i = 0; i < index.size(); ++i)
    {
        if (index[i] == k)
        {
            index.erase(index.begin() + i);
            destructibles.erase(destructibles.begin() + i);
            return;
        }
    }
}
