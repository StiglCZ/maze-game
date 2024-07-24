#ifndef __STRUCTS_HPP
#define __STRUCTS_HPP

#include <stack>

struct Point {
    int x;
    int y;
    friend bool operator==(Point a, Point b){
        return a.x == b.x && a.y == b.y;
    }
};

struct mazeData {
    Point p;
    std::stack<Point> history;
    float timer;
};

#endif
