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
