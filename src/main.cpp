// created by: A-squared 
#include <iostream>
#include "vec2d.h"
#include "text.h"

int main()
{
    std::vector<std::vector<std::string> > info = get_pieces_of_file("test.txt");
    for (int i = 0; i < info.size(); ++i)
    {
        for (int j = 0; j < info[i].size(); ++j)
        {
            std::cout << info[i][j] << ' ';
        }
        std::cout << std::endl;
    }

    put_data_to_file(info, "transfer.txt");
    return 0;
}
