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
        {   // Drawing
            BeginDrawing();
            ClearBackground({255, 255, 255, 255});

            // Draw the tiles
            drawMaze(field);

            // Timer
            if(maze.p != start)
                maze.timer += GetFrameTime();
            else maze.timer = 0.0f;
            
            std::string str = std::to_string(maze.timer).substr(0, 5);
            DrawText(str.c_str(), W * Sc - 50, 0, 20, WHITE);
        
            EndDrawing();
        }
        {
            // Movement
            Point previous = maze.p;
            REGISTER_MOVE(NORMAL_UP, +0, -1);
            REGISTER_MOVE(NORMAL_DOWN, +0, +1);
            REGISTER_MOVE(NORMAL_LEFT, -1, +0);
            REGISTER_MOVE(NORMAL_RIGHT, +1, +0);

            // History
            if(maze.p != previous)
                maze.history.push(previous);
            
            // No out of bounds!
            if(!maze.p.Inside({W, H}))
                backTrack(maze, field);

            REGISTER_BIGMOVE(DIRECT_UP, +0, -1);
            REGISTER_BIGMOVE(DIRECT_DOWN, +0, +1);
            REGISTER_BIGMOVE(DIRECT_LEFT, -1, +0);
            REGISTER_BIGMOVE(DIRECT_RIGHT, +1, +0);
        }
        // Reset player
        if(IsKeyPressed(RESET))
            resetPlayer(maze, field);

        // Go back
        if(IsKeyPressed(BACK))
            backTrack(maze, field);

        // Change map
        if(IsKeyPressed(CHANGE_MAP) && field[conv(finish)] == 4) {
            std::cout << "Skipped\n";
            maze = mazeGen();
            continue;
        }

        // Winning
        if(maze.p.Distance(finish) < 2) {
            std::cout << std::to_string(maze.timer) << std::endl;
            maze = mazeGen();
            continue;
        }

        field[conv(maze.p)] = 2;
    }
}
