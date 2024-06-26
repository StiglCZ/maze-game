#ifndef __CONSTS_HPP
#define __CONSTS_HPP

#include <raylib.h>
#define conv(p) (p.x + p.y * W)

const int W = 30, H = 30, Sc = 20;
typedef u_char t_field;

const Color
    WALLS      = GREEN,
    BACKGROUND = BLUE,
    PLAYER     = RED,
    FINISH     = YELLOW,
    INVALID    = BLACK,
    GRID       = WHITE;

const int
    NORMAL_UP   = KEY_W,
    NORMAL_DOWN = KEY_S,
    NORMAL_LEFT = KEY_A,
    NORMAL_RIGHT= KEY_D,

    DIRECT_UP   = KEY_UP,
    DIRECT_DOWN = KEY_DOWN,
    DIRECT_LEFT = KEY_LEFT,
    DIRECT_RIGHT= KEY_RIGHT;

#endif