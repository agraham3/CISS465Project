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
            SDL_Rect block = {lefts[j], tops[i], 62, 52};
            blocks.push_back(block);
        }
    }
}
