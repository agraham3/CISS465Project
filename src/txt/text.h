#ifndef TEXT_H
#define TEXT_H


#include <iostream>
#include <vector>
#include <fstream>
#include <string>


const char SEPARATOR = '~';

class NOFILE{};

std::vector<std::string> read_file_to_rows(const char file[]);
std::vector<std::string> get_cols(const std::string & row);
std::vector<std::vector<std::string> > get_pieces_of_file(const char file[]);

std::string combine_strings_to_form_row(const std::vector<std::string> & list);
std::string combine_strings_for_file(const std::vector<std::string> & list);
void write_string_to_file(const std::string & to_write, const char filename[]);
void put_data_to_file(const std::vector<std::vector<std::string> > & data, const char filename[]);


#endif
