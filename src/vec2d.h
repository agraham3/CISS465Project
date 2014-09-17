#ifndef VEC2D_H
#define VEC2D_H

// Created by: A-squared
// vec2d is a 2 dimentional vector

#include <iostream>

class vec2d
{
public:
    vec2d(int x0 = 0, int y0 = 0)
        : x(x0), y(y0)
    {}

    int get_x() const;
    int get_y() const;

    void set_x(int);
    void set_y(int);

    int operator [](int) const;
    int & operator [](int);

    vec2d operator+(vec2d) const;
    vec2d operator-(vec2d) const;
    vec2d operator*(int) const;
    
private:
    int x, y;
};

vec2d operator*(int, vec2d);
std::ostream & operator<<(std::ostream &, const vec2d &);

#endif
