#include "text.h"

const char SEPARATOR = '~';

std::vector<std::string> read_file_to_rows(const char file[])
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
        if (temp == "\0")
        {
            continue;
        }
        ret.push_back(temp);
    }
    return ret;
}


std::vector<std::string> get_cols(const std::string & row)
{
    std::vector<std::string> ret;
    std::string temp;
    for (int i = 0; i < row.size(); ++i)
    {
        if (row[i] == SEPARATOR)
        {
            ret.push_back(temp);
            temp.clear();
        }
        else if (row[i] != ' ')
        {
            temp += row[i];
        }
    }
    ret.push_back(temp);
    return ret;
}

std::vector<std::vector<std::string> > get_pieces_of_file(const char file[])
{
    std::vector<std::string> row = read_file_to_rows(file);
    std::vector<std::vector<std::string> > ret;
    for (int i = 0; i < row.size(); ++i)
    {
        ret.push_back(get_cols(row[i]));
    }
    return ret;
}

std::string combine_strings_to_form_row(const std::vector<std::string> & list)
{
    std::string ret;
    for (int i = 0; i < list.size(); ++i)
    {
        ret += list[i] + SEPARATOR;
    }
    return ret;
}

std::string combine_strings_for_file(const std::vector<std::string> & list)
{
    std::string ret;
    for (int i = 0; i < list.size(); ++i)
    {
        ret += list[i] + '\n';
    }
    return ret;
}

void write_string_to_file(const std::string & to_write, const char filename[])
{
    std::fstream f;
    f.open(filename);
    std::cout << f.good() << std::endl;
    f << to_write;
    f.close();
    return;
}

void put_data_to_file(const std::vector<std::vector<std::string> > & data,
                      const char filename[])
{
    std::vector<std::string> medium;
    for (int i = 0; i < data.size(); ++i)
    {
        medium.push_back(combine_strings_to_form_row(data[i]));
    }
    std::string to_write = combine_strings_for_file(medium);
    write_string_to_file(to_write, filename);
    return;
}
