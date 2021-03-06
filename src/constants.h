#ifndef CONSTANTS_H
#define CONSTANTS_H


#include <SDL.h>

const int WIDTH = 900;
const int HEIGHT = 636;
//COLORS
const SDL_Color WHITE = {255, 255, 255};
const SDL_Color BLACK = {0, 0, 0};
const SDL_Color GREEN = {0, 255, 0};
const SDL_Color RED = {255, 0, 0};
const SDL_Color BLUE = {0, 0, 255};
const SDL_Color YELLOW = {255, 255, 0};
const SDL_Color ORANGE = {255, 128, 0};
const SDL_Color PURPLE = {128, 0, 128};


const SDL_Rect END_GAME_RECT = {(WIDTH - 180) / 2,(HEIGHT - 30) / 2, 180, 30};
const SDL_Rect KILLS_RECT = {(WIDTH - 100) / 2, END_GAME_RECT.y + 40, 100, 20};
const SDL_Rect DEATHS_RECT = {(WIDTH - 100) / 2, KILLS_RECT.y + 30, 100, 20};
const SDL_Rect DROPS_RECT = {(WIDTH - 100) / 2, DEATHS_RECT.y + 30, 100, 20};
const SDL_Rect KDA_RECT = {(WIDTH - 120) / 2, DROPS_RECT.y + 30, 120, 25};
const SDL_Rect PROMPT_RECT = {(WIDTH - 120) / 2, KDA_RECT.y + 30, 120, 30};

//SDL key shortcuts
const Uint32 LEFT = SDLK_LEFT;
const Uint32 RIGHT = SDLK_RIGHT;
const Uint32 UP = SDLK_UP;
const Uint32 DOWN = SDLK_DOWN;
const Uint32 ENTER = SDLK_RETURN;
const Uint32 SPACE = SDLK_SPACE;
const Uint32 A = SDLK_a;
const Uint32 B = SDLK_b;
const Uint32 C = SDLK_c;
const Uint32 D = SDLK_d;
const Uint32 E = SDLK_e;
const Uint32 F = SDLK_f;
const Uint32 G = SDLK_g;
const Uint32 H = SDLK_h;
const Uint32 I = SDLK_i;
const Uint32 J = SDLK_j;
const Uint32 K = SDLK_k;
const Uint32 L = SDLK_l;
const Uint32 M = SDLK_m;
const Uint32 N = SDLK_n;
const Uint32 O = SDLK_o;
const Uint32 P = SDLK_p;
const Uint32 Q = SDLK_q;
const Uint32 R = SDLK_r;
const Uint32 S = SDLK_s;
const Uint32 T = SDLK_t;
const Uint32 U = SDLK_u;
const Uint32 V = SDLK_v;
const Uint32 W = SDLK_w;
const Uint32 X = SDLK_x;
const Uint32 Y = SDLK_y;
const Uint32 Z = SDLK_z;
const Uint32 ZERO = SDLK_0;
const Uint32 ONE = SDLK_1;
const Uint32 TWO = SDLK_2;
const Uint32 THREE = SDLK_3;
const Uint32 FOUR = SDLK_4;
const Uint32 FIVE = SDLK_5;
const Uint32 SIX = SDLK_6;
const Uint32 SEVEN = SDLK_7;
const Uint32 EIGHT = SDLK_8;
const Uint32 NINE = SDLK_9;
const Uint32 RSHIFT = SDLK_RSHIFT;
const Uint32 LSHIFT = SDLK_LSHIFT;
const Uint32 BACKSPACE = SDLK_BACKSPACE;
const Uint32 MINUS = SDLK_MINUS;

// important numbers for game mechanics
const int FRAMES_PER_SEC = 60;
const int MAX_HEALTH = 2;
const int MAX_LIVES = 3;
const int RESPAWN_TIME = 10000;
const int TIME_FOR_BOMB_EX[] = {2000}; //time kept in miliseconds
const int EXPLOSION_TIME = 2000;
const int MAX_BOMBS_PER_PLAYER = 10;


#endif

