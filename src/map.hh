#ifndef __MAP_HH
#define __MAP_HH

#include "structs.hpp"
#include "consts.hpp"
#include <vector>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

char isPossible(Point p, t_field *field) {
    if(p.x < 0 || p.y < 0) return 0;
    if(p.x > W || p.y > H) return 0;
    if(field[conv(p)] != 1)return 0;
    return 1;
}
char isUsable(Point p, t_field *field) {
    if(p.x <  0 || p.y <  0) return 0;
    if(p.x >= W || p.y >= H) return 0;
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
    for(int i =0; i < W * H; i++) field[i] = 1;

    // Finish trigger
    field[W - 1 + 5 * W] = 4;
    
    std::stack<Point> history = {};
    Point pos = {2, 0};
    while(1){
        field[conv(pos)] = 0;
        if(!history.empty())
            fullFill(history.top(), pos, field);
        std::vector<Point> possible = createPossible(pos, field);
        if(possible.empty()) {
            if(history.empty()) break;
            pos = history.top();
            history.pop();
            continue;
        }
        history.push(pos);
        pos = possible[rand() % possible.size()];
        usleep(5000);
    }
}

#endif
