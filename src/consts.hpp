#ifndef __CONSTS_HPP
#define __CONSTS_HPP

#include <raylib.h>
#include "structs.hpp"

#define conv(p) (p.x + p.y * W)
typedef unsigned char t_field;

const int
    W     = 35,
    H     = 35,
    Sc    = 20,
    step  = 2,
    delay = 5000;

const Point
    start = {2, 0},
    finish = {W - 1, H - 1};

const int
    NORMAL_UP   = KEY_W,
    NORMAL_DOWN = KEY_S,
    NORMAL_LEFT = KEY_A,
    NORMAL_RIGHT= KEY_D,

    DIRECT_UP   = KEY_UP,
    DIRECT_DOWN = KEY_DOWN,
    DIRECT_LEFT = KEY_LEFT,
    DIRECT_RIGHT= KEY_RIGHT,

    CHANGE_MAP  = KEY_Q,

    RESET = KEY_SPACE,
    BACK  = KEY_B;

const Color
    WALLS      = GREEN,
    BACKGROUND = BLUE,
    PLAYER     = RED,
    FINISH     = YELLOW,
    INVALID    = BLACK,
    TIME       = WHITE;

#endif
