#include <cmath>
#include <iostream>
#include <stack>
#include <string>
#include <thread>
#include <unistd.h>
#include <raylib.h>
#include <cstdlib>
#include <vector>

#include "structs.hpp"
#include "consts.hpp"
#include "draw.hpp"
#include "move.hpp"
#include "map.hh"

t_field field[W * H];

mazeData mazeGen() {
    srand(time(NULL));
    Point p = start;
    std::stack<Point> history;
    std::thread(CreateField, field).detach();
    return mazeData {p, history, 0.0};

int main() {
    
    // Raylib init
    SetTraceLogLevel(TraceLogLevel::LOG_NONE);
    InitWindow(W * Sc, H * Sc, "");
    SetTargetFPS(30);

    // Mazegen
    mazeData maze = mazeGen();

    while(!WindowShouldClose()){
        BeginDrawing();
        ClearBackground({255, 255, 255, 255});
        
        // Draw the tiles
        drawMaze(field);
        // Grid
        drawGrid();
        
        // Timer
        maze.timer += GetFrameTime();
        std::string str = std::to_string((std::round(maze.timer * 100) / 100.0f)).substr(0, 5);
        DrawText(str.c_str(), W * Sc- 120, H * Sc -20, 20, PLAYER);
        
        EndDrawing();
        {
            // Winning
            if((maze.p.y == 5 && maze.p.x == W - 2) || (maze.p.y == 4 && maze.p.x == W - 1) || (maze.p.y == 6 && maze.p.x == W - 1)) {
                std::cout << std::to_string(maze.timer) << "\n";
                maze = mazeGen();
                continue;
            }

            // Movement
            Point op = maze.p;
            REGISTER_MOVE(NORMAL_UP, +0, -1);
            REGISTER_MOVE(NORMAL_DOWN, +0, +1);
            REGISTER_MOVE(NORMAL_LEFT, -1, +0);
            REGISTER_MOVE(NORMAL_RIGHT, +1, +0);
            REGISTER_BIGMOVE(DIRECT_UP, +0, -1);
            REGISTER_BIGMOVE(DIRECT_DOWN, +0, +1);
            REGISTER_BIGMOVE(DIRECT_LEFT, -1, +0);
            REGISTER_BIGMOVE(DIRECT_RIGHT, +1, +0);

            if (maze.p.x != op.x || maze.p.y != op.y) { // History
                maze.history.push(maze.p);
            }

            if(maze.p.x < 0 || maze.p.x >= W || maze.p.y < 0 || maze.p.y >= H) { // No out of bounds!
                maze.p = maze.history.top();
                maze.history.pop();
            }

            if(IsKeyPressed(KEY_SPACE)) { // Reset
                resetPlayer(maze, field);
            }
            if(IsKeyPressed(KEY_B)) { // Go back
                backTrack(maze, field);
            }
            field[conv(maze.p)] = 2;
        }
    }
}
