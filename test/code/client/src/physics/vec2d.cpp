#include "vec2d.h"


// functions are in the order of the vec2d.h file

int vec2d::get_x() const
{
    return x;
}

int vec2d::get_y() const
{
    return y;
}

void vec2d::set_x(int c)
{
    x = c;
}

void vec2d::set_y(int c)
{
    y = c;
}

int vec2d::operator[](int c) const
{
    if (c % 2 == 0) return x;
    return y;
}

int & vec2d::operator[](int c)
{
    if (c % 2 == 0) return x;
    return y;
}

vec2d vec2d::operator+(vec2d v) const
{
    return vec2d(x + v[0], y + v[1]);
}

vec2d vec2d::operator-(vec2d v) const
{
    return vec2d(x - v[0], y - v[1]);
}

vec2d vec2d::operator*(int c) const
{
    return vec2d(x * c, y * c);
}

vec2d operator*(int c, vec2d v)
{
    return v * c;
}

std::ostream & operator<<(std::ostream & cout,
                          const vec2d & v)
{
    //cout << &v;
    cout << '<'  << v[0]
         << ", " << v[1]
         << '>';
    return cout;
}
