#ifndef TEXT_H
#define TEXT_H


#include <iostream>
#include <vector>
#include <fstream>
#include <string>

class NOFILE{};

std::vector<std::string> read_file_to_rows(const std::string & file);
#endif
