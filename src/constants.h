#ifndef CONSTANTS_H
#define CONSTANTS_H


#include <SDL.h>

//SDL key shortcuts
const Uint32 LEFT = SDLK_LEFT;
const Uint32 RIGHT = SDLK_RIGHT;
const Uint32 UP = SDLK_UP;
const Uint32 DOWN = SDLK_DOWN;
const Uint32 ENTER = SDLK_RETURN;
const Uint32 SPACE = SDLK_SPACE;


// important numbers for game mechanics
const int FRAMES_PER_SEC = 30;
const int TIME_FOR_BOMB_EX[] = {2000}; //time kept in miliseconds
const int EXPLOSION_TIME = 2000;
const int MAX_BOMBS_PER_PLAYER = 10;

#endif

