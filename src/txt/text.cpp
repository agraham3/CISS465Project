#include "text.h"

std::string read_file_to_string(char file[])
{
    std::ifstream f;
    f.open(file);
    if (!f.good())
    {
        std::cout << "Failed to open file!\n";
        return "";
    }
    std::string ret;
    while (f.good())
    {
        std::string temp;
        std::getline(f, temp);
        ret += temp + '\n';
    }
}
