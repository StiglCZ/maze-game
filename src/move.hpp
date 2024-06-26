#ifndef MOVE_HPP
#define MOVE_HPP

#include "structs.hpp"
#include "consts.hpp"

#define REGISTER_MOVE(KEY, PX, PY) if(IsKeyPressed(KEY) && isUsable({maze.p.x PX, maze.p.y PY}, field)) { \
    maze.p.y = maze.p.y PY; \
    maze.p.x = maze.p.x PX;}
#define REGISTER_BIGMOVE(KEY, PX, PY) \
    if (IsKeyPressed(KEY)){ \
        while(isUsable({maze.p.x PX, maze.p.y PY}, field)) { \
            maze.history.push(maze.p); \
            maze.p.y = maze.p.y PY; \
            maze.p.x = maze.p.x PX; \
            field[conv(maze.p)] = 2;}}

void backTrack(mazeData &maze, t_field *field) {
    if(maze.history.empty()) return;
    field[conv(maze.p)] = 0;
    maze.p = maze.history.top();
    maze.history.pop();
}

void resetPlayer(mazeData &maze, t_field *field) {
    field[conv(maze.p)] = 0;
    while(!maze.history.empty()){
        field[conv(maze.history.top())] = 0;
        maze.history.pop();
    }
    maze.p = start;
    maze.timer = 0;
}

#endif