#include <cmath>
#include <iostream>
#include <stack>
#include <string>
#include <thread>
#include <unistd.h>
#include <raylib.h>
#include <cstdlib>

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
}

int main() {
    
    // Raylib init
    SetTraceLogLevel(TraceLogLevel::LOG_NONE);
    InitWindow(W * Sc, H * Sc, "Maze");
    SetTargetFPS(60);

    // Mazegen
    mazeData maze = mazeGen();

    while(!WindowShouldClose()){
        BeginDrawing();
        ClearBackground({255, 255, 255, 255});
        
        // Draw the tiles
        drawMaze(field);
        
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
            
            // History
            if (maze.p.x != op.x || maze.p.y != op.y)
                maze.history.push(maze.p);
            
            // No out of bounds!
            if(maze.p.x < 0 || maze.p.x >= W || maze.p.y < 0 || maze.p.y >= H) {
                maze.p = maze.history.top();
                maze.history.pop();
            }

            // Reset player
            if(IsKeyPressed(RESET))
                resetPlayer(maze, field);

            // Go back
            if(IsKeyPressed(BACK))
                backTrack(maze, field);

            // Change map
            if(IsKeyPressed(CHANGE_MAP) && maze.timer > 1.7f) {
                std::cout << "Skipped\n";
                maze = mazeGen();
                continue;
            }

            field[conv(maze.p)] = 2;
        }
    }
}
