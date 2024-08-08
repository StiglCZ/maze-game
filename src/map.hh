#ifndef __MAP_HH
#define __MAP_HH

#include "structs.hpp"
#include "consts.hpp"
#include <cstdlib>
#include <vector>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <iostream>
bool inBounds(Point p, Point max) {
    return (p.x < max.x && p.y < max.y && p.x > -1 && p.y > -1);
}
char isPossible(Point p, t_field *field) {
    if(!inBounds(p, {W, H}))  return 0;
    if(field[conv(p)] != 1)   return 0;
    return 1;
}

char isUsable(Point p, t_field *field) {
    if(!inBounds(p, {W, H}))  return 0;
    if(field[conv(p)] != 0)return 0;
    return 1;
}
std::vector<Point> createPossible(Point p, t_field *field) {
    Point
        up    = {p.x, p.y - step}, down  = {p.x, p.y + step},
        left  = {p.x + step, p.y}, right = {p.x - step, p.y};
    std::vector<Point> result;
    if(isPossible(up, field))   result.push_back(up);
    if(isPossible(down, field)) result.push_back(down);
    if(isPossible(left, field)) result.push_back(left);
    if(isPossible(right, field))result.push_back(right);
    return result;
}
void fullFill(Point p, Point q, t_field *field) {
    if(p.x < q.x) p.x++;
    else if(p.x > q.x) p.x--;
    else if(p.y < q.y) p.y++;
    else if(p.y > q.y) p.y--;
    if(isPossible(p, field)) field[conv(p)] = 0;
}

void CreateField(t_field *field) {
    srand(time(NULL));
    std::cout << time(NULL) << " - ";
    for(int i =0; i < W * H; i++) field[i] = 1;
    std::stack<Point> history = {};
    Point pos = start;

    while(1){
        std::vector<Point> possible = createPossible(pos, field);
        if(possible.empty()) {
            if(history.empty()) break;
            pos = history.top();
            history.pop();
            continue;
        }
        usleep(delay);
        history.push(pos);
        pos = possible[rand() % possible.size()];
        fullFill(history.top(), pos, field);
        if(field[conv(pos)] != 2) field[conv(pos)] = 0;
    }
    // Enhance the maze (for perfect maze delete this)
    for(int i =0; i < 20; i++){
        Point p;
        do{ p = {(rand() % (W / 2)) * 2 + 1, (rand() % (H / 2)) * 2};
        }while(field[conv(p)] == 0);
        field[conv(p)] = 0;
    }
    // Finish trigger
    field[conv(finish)] = 4;
}

#endif
