#ifndef __DRAW_HPP
#define __DRAW_HPP

#include <cmath>
#include "consts.hpp"
#include "structs.hpp"

#define rect(x, y, c) DrawRectangle(x * Sc, y * Sc, Sc, Sc, c);

void drawGrid() {
    for(int i =0; i < W; i++) DrawLine(i * Sc, 0 * Sc, i * Sc, H * Sc, GRID);
    for(int i =0; i < H; i++) DrawLine(0 * Sc, i * Sc, W * Sc, i * Sc, GRID);
}

void drawMaze(t_field *field) {
    for(int i =0; i < W * H; i++){
        Color c;
        switch(field[i]){
            case 0: c = BACKGROUND; break;
            case 1: c = WALLS;  break;
            case 2: c = PLAYER; break;
            case 3: c = FINISH; break;
            default: c = INVALID;
        }

        rect(i % W, std::floor(i / (float)W), c);
    }
}

#endif