#ifndef __CONSTS_HPP
#define __CONSTS_HPP

#include <raylib.h>
#include "structs.hpp"

#define conv(p) (p.x + p.y * W)

const int W = 30, H = 30, Sc = 20, step = 2;
const Point start = {2, 0};
typedef unsigned char t_field;

const int
    NORMAL_UP   = KEY_W,
    NORMAL_DOWN = KEY_S,
    NORMAL_LEFT = KEY_A,
    NORMAL_RIGHT= KEY_D,

    DIRECT_UP   = KEY_UP,
    DIRECT_DOWN = KEY_DOWN,
    DIRECT_LEFT = KEY_LEFT,
    DIRECT_RIGHT= KEY_RIGHT,

    RESET = KEY_SPACE,
    BACK = KEY_B;

const Color
    WALLS      = GREEN,
    BACKGROUND = BLUE,
    PLAYER     = RED,
    FINISH     = YELLOW,
    INVALID    = BLACK,
    GRID       = WHITE;

#endif
