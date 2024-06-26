#ifndef __STRUCTS_HPP
#define __STRUCTS_HPP

#include <stack>

struct Point {
  int x;
  int y;
};

struct mazeData {
    Point p;
    std::stack<Point> history;
    float timer;
};

#endif