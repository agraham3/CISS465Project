#include "functions.h"

std::string to_string(int val)
{
    char temp[100];
    sprintf(temp, "%d", val);
    std::string ret(temp);
    return ret;
}
