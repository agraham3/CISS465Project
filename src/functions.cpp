#include "functions.h"

std::string to_string(int val)
{
    char temp[100];
    sprintf(temp, "%d", val);
    std::string ret(temp);
    return ret;
}

std::string to_string(double val)
{
    char temp[100];
    sprintf(temp, "%f", val);
    std::string ret(temp);
    return ret;
}

std::string to_string(float val)
{
    char temp[100];
    sprintf(temp, "%f", val);
    std::string ret(temp);
    return ret;
}


std::vector<int> get_ints(const std::string & s)
{
    std::vector<int> ret;
    std::string temp;
    for (int i = 0; i < s.size(); ++i)
    {
        if (s[i] == '|')
        {
            int to_push = 0;
            int product = 1;
            for (int j = 0; j < temp.size(); ++i)
            {
                to_push += int(temp[i] - '0') * product;
                product *= 10;
            }
            ret.push_back(to_push);
            temp.clear();
        }
        else
        {
            temp += s[i];
        }
    }
    return ret;
}
