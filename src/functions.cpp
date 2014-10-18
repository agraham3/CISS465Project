#include "functions.h"

std::string to_string(int val)
{
    char temp[100];
    sprintf(temp, "%d", val);
    std::string ret(temp);
    return ret;
}

std::string to_string(double val)
{
    char temp[100];
    sprintf(temp, "%f", val);
    std::string ret(temp);
    return ret;
}

std::string to_string(float val)
{
    char temp[100];
    sprintf(temp, "%f", val);
    std::string ret(temp);
    return ret;
}


std::vector<int> get_ints(const std::string & s)
{
    std::vector<int> ret;
    std::string temp;
    for (int i = 0; i < s.size(); ++i)
    {
        if (s[i] == '|')
        {
            int to_push = 0;
            int product = 1;
            ret.push_back(atoi(temp.c_str()));
            temp.clear();
        }
        else
        {
            temp += s[i];
        }
    }
    return ret;
}

std::string get_name(const std::string & s)
{
    std::string ret;
    for (int i = 0; s[i] != '|'; ++i)
    {
        ret += s[i];
    }
    return ret;
}


int get_typed_char(std::string & ret)
{
    SDL_Event event;
    static int v = 0;
    static bool holdl = false;
    static bool holdr = false;
    while (SDL_PollEvent(&event))
    {
        switch(event.type)
        {
            case SDL_QUIT:
                return -1;
                break;
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym)
            {
                case LSHIFT:
                    holdl = true;
                    break;
                case RSHIFT:
                    holdr = true;
                    break;
                case A:
                    ret += ('a' + v);
                    break;
                case B:
                    ret += ('b' + v);
                    break;
                case C:
                    ret += ('c' + v);
                    break;
                case D:
                    ret += ('d' + v);
                    break;
                case E:
                    ret += ('e' + v);
                    break;
                case F:
                    ret += ('f' + v);
                    break;
                case G:
                    ret += ('g' + v);
                    break;
                case H:
                    ret += ('h' + v);
                    break;
                case I:
                    ret += ('i' + v);
                    break;
                case J:
                    ret += ('j' + v);
                    break;
                case K:
                    ret += ('k' + v);
                    break;
                case L:
                    ret += ('l' + v);
                    break;
                case M:
                    ret += ('m' + v);
                    break;
                case N:
                    ret += ('n' + v);
                    break;
                case O:
                    ret += ('o' + v);
                    break;
                case P:
                    ret += ('p' + v);
                    break;
                case Q:
                    ret += ('q' + v);
                    break;
                case R:
                    ret += ('r' + v);
                    break;
                case S:
                    ret += ('s' + v);
                    break;
                case T:
                    ret += ('t' + v);
                    break;
                case U:
                    ret += ('u' + v);
                    break;
                case V:
                    ret += ('v' + v);
                    break;
                case W:
                    ret += ('w' + v);
                    break;
                case X:
                    ret += ('x' + v);
                    break;
                case Y:
                    ret += ('y' + v);
                    break;
                case Z:
                    ret += ('z' + v);
                    break;
                case SPACE:
                    ret += ' ';
                    break;
                case ZERO:
                    ret += '0';
                    break;
                case ONE:
                    ret += '1';
                    break;
                case TWO:
                    ret += '2';
                    break;
                case THREE:
                    ret += '3';
                    break;
                case FOUR:
                    ret += '4';
                    break;
                case FIVE:
                    ret += '5';
                    break;
                case SIX:
                    ret += '6';
                    break;
                case SEVEN:
                    ret += '7';
                    break;
                case EIGHT:
                    ret += '8';
                    break;
                case NINE:
                    ret += '9';
                    break;
                case MINUS:
                    if (v == -32) ret += '_';
                    else ret += '-';
                    break;
                case BACKSPACE:
                    ret = ret.substr(0, ret.size() - 1);
                    break;
            }
                break;
            case SDL_KEYUP:
            {
                case LSHIFT:
                    holdl = false;
                    break;
                case RSHIFT:
                    holdr = false;
                    break;
            }
            
        }
    }
    if (holdr || holdl)
        v = -32;
    else
        v = 0;
    return 0;
}


bool clicked(int x, int y, SDL_Rect subject)
{
    return (x >= subject.x && x <= subject.x + subject.w
            && y >= subject.y && y <= subject.y + subject.h);
}
