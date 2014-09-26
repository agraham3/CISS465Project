#include "text.h"

std::vector<std::string> read_file_to_string(char file[])
{
    std::ifstream f;
    f.open(file);
    if (!f.good())
    {
        std::cout << "Failed to open file!\n";
        NOFILE e;
        throw e;
    }
    std::vector<std::string> ret;
    while (f.good())
    {
        std::string temp;
        std::getline(f, temp);
        ret.push_back(temp);
    }
    return ret;
}
