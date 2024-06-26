#ifndef __MAP_HH
#define __MAP_HH

#include "structs.hpp"
#include "consts.hpp"
#include <vector>
#include <stdlib.h>

char isPossible(Point p, t_field *field) {
    if(p.x < 0 || p.y < 0) return 1;
    if(p.x > W || p.y > H) return 1;
    if(field[conv(p)] == 0)return 2;
    return 0;
}
char isUsable(Point p, t_field *field) {
    if(p.x < 0 || p.y < 0) return 0;
    if(p.x > W || p.y > H) return 0;
    if(field[conv(p)] != 0)return 0;
    return 1;
}
std::vector<Point> createPossible(Point p, t_field *field) {
    Point
        up    = {p.x, p.y - 2}, down  = {p.x, p.y + 2},
        left  = {p.x + 2, p.y}, right = {p.x - 2, p.y};
    std::vector<Point> result;
    if(!isPossible(up, field))   result.push_back(up);
    if(!isPossible(down, field)) result.push_back(down);
    if(!isPossible(left, field)) result.push_back(left);
    if(!isPossible(right, field))result.push_back(right);
    return result;
}
void fullFill(Point p, Point q, t_field *field) {
    if(p.x < q.x) p.x++;
    else if(p.x > q.x) p.x--;
    else if(p.y < q.y) p.y++;
    else if(p.y > q.y) p.y--;
    if(!isPossible(p, field)) field[conv(p)] = 0;
}

void CreateField(t_field *field) {
    for(int i =0; i < W * H; i++) field[i] = 1;
    std::stack<Point> history = {};
    Point position = {0, 0};
    while(1){
        field[conv(position)] = 0;
        if(!history.empty())
            fullFill(history.top(), position, field);
        std::vector<Point> possible = createPossible(position, field);
        if(possible.empty()) {
            if(history.empty()) break;
            position = history.top();
            history.pop();
            continue;
        }
        history.push(position);
        position = possible[rand() % possible.size()];
    }
    field[W - 1 + 5 * W] = 4;
    field[W - 2 + 5 * W] = 0;
}

#endif