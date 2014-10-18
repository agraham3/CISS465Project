#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "includes.h"
#include "SDL.h"
#include "constants.h"

std::string to_string(int int_val);
std::string to_string(double double_val);
std::string to_string(float float_val);
std::string to_string(const std::vector<int> &x);

std::vector<int> get_ints(const std::string & s);
std::vector<std::string> get_parts(const std::string & s, char split = '|');
std::string get_name(const std::string & s);

int get_typed_char(std::string & s);
bool clicked(int x, int y, SDL_Rect subject);

bool in_array(int num, const std::vector<int> &x);
std::vector<int> generate_block_positions();

#endif

