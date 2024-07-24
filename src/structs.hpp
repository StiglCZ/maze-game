#ifndef __STRUCTS_HPP
#define __STRUCTS_HPP

#include <stack>
#include <math.h>

struct Point {
    public:
    int x, y;
    friend bool operator==(Point a, Point b){
        return a.x == b.x && a.y == b.y;
    }
    friend bool operator!=(Point a, Point b){ return !(a==b); }
    int Distance(Point b){
        return std::abs(x - b.x) + std::abs(y - b.y);
    }
};

struct mazeData {
    Point p;
    std::stack<Point> history;
    float timer;
};

#endif
