#include "Stage.h"

Stage::Stage(Screen & s, std::string image_file)
    :img(Image(image_file, s))
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
}


int Stage::collision(const SDL_Rect & rect)
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
