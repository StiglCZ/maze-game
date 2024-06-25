#include <cmath>
#include <iostream>
#include <stack>
#include <string>
#include <thread>
#include <unistd.h>
#include <raylib.h>
#include <cstdlib>
#include <vector>

const int W = 30, H = 30, Sc = 20;
#define rect(x, y, c) DrawRectangle(x * Sc, y * Sc, Sc, Sc, c);
#define conv(p) (p.x + p.y * W)
struct Point {
  int x;
  int y;
};
// Settings
 
// Colors
const Color
    WALLS      = GREEN,
    BACKGROUND = BLUE,
    PLAYER     = RED,
    FINISH     = YELLOW,
    INVALID    = BLACK,
    GRID       = WHITE
    ;

const int
    NORMAL_UP   = KEY_W,
    NORMAL_DOWN = KEY_S,
    NORMAL_LEFT = KEY_A,
    NORMAL_RIGHT= KEY_D,

    DIRECT_UP   = KEY_UP,
    DIRECT_DOWN = KEY_DOWN,
    DIRECT_LEFT = KEY_LEFT,
    DIRECT_RIGHT= KEY_RIGHT;

u_char field[W * H];
char isPossible(Point p) {
    if(p.x < 0 || p.y < 0) return 1;
    if(p.x > W || p.y > H) return 1;
    if(field[conv(p)] == 0)return 2;
    return 0;
}
char isUsable(Point p) {
    if(p.x < 0 || p.y < 0) return 0;
    if(p.x > W || p.y > H) return 0;
    if(field[conv(p)] != 0)return 0;
    return 1;
}
std::vector<Point> createPossible(Point p) {
    Point
        up    = {p.x, p.y - 2}, down  = {p.x, p.y + 2},
        left  = {p.x + 2, p.y}, right = {p.x - 2, p.y};
    std::vector<Point> result;
    if(!isPossible(up))   result.push_back(up);
    if(!isPossible(down)) result.push_back(down);
    if(!isPossible(left)) result.push_back(left);
    if(!isPossible(right))result.push_back(right);
    return result;
}
void fullFill(Point p, Point q) {
    if(p.x < q.x) p.x++;
    else if(p.x > q.x) p.x--;
    else if(p.y < q.y) p.y++;
    else if(p.y > q.y) p.y--;
    if(!isPossible(p)) field[conv(p)] = 0;
}

void CreateField() {
    for(int i =0; i < W * H; i++) field[i] = 1;
    std::stack<Point> history = {};
    Point position = {0, 0};
    while(1){
        field[conv(position)] = 0;
        if(!history.empty())
            fullFill(history.top(), position);
        std::vector<Point> possible = createPossible(position);
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

struct mazeData {
    Point p;
    std::stack<Point> history;
};

mazeData mazeGen(float timer) {
    if(timer != 0.0f) std::cout << std::to_string(timer) << "\n";
    timer = 0;
    srand(time(NULL));
    Point p = {2, 0};
    std::stack<Point> history;
    std::thread(CreateField).detach();
    return mazeData {p, history};
}

int main() {
    
    // Raylib init
    SetTraceLogLevel(TraceLogLevel::LOG_NONE);
    InitWindow(W * Sc, H * Sc, "");
    SetTargetFPS(30);
    float timer = 0;
    
    // Mazegen
    mazeData maze = mazeGen(timer);

    
    while(!WindowShouldClose()){
        BeginDrawing();
        ClearBackground({255, 255, 255, 255});
        
        // Draw the tiles
        for(int i =0; i < W * H; i++){
            Color c;
            switch(field[i]){
                case 0: c = BACKGROUND; break;
                case 1: c = WALLS;  break;
                case 2: c = PLAYER; break;
                case 3: c = FINISH; break;
                default: c = INVALID;
            }
            
            rect(i % W, std::floor(i / (float)W), c);
        }
        // Grid
        for(int i =0; i < W; i++) DrawLine(i * Sc, 0 * Sc, i * Sc, H * Sc, GRID);
        for(int i =0; i < H; i++) DrawLine(0 * Sc, i * Sc, W * Sc, i * Sc, GRID);

        // Winning
        if((maze.p.y == 5 && maze.p.x == W - 2) || (maze.p.y == 4 && maze.p.x == W - 1) || (maze.p.y == 6 && maze.p.x == W - 1)) {
            maze = mazeGen(timer); 
            continue;
        }
        
        // Timer
        timer += 0.033;
        std::string str = std::to_string((std::round(timer * 100) / 100.0f)).substr(0, 5);
        DrawText(str.c_str(), W * Sc- 120, H * Sc -20, 20, PLAYER);
        
        EndDrawing();
        {
            // Movement
            maze.history.push(maze.p);
            if(IsKeyPressed(NORMAL_UP) && isUsable({maze.p.x, maze.p.y -1})) maze.p.y--;
            else if(IsKeyPressed(NORMAL_DOWN)  && isUsable({maze.p.x, maze.p.y +1})) maze.p.y++;
            else if(IsKeyPressed(NORMAL_LEFT)  && isUsable({maze.p.x -1, maze.p.y})) maze.p.x--;
            else if(IsKeyPressed(NORMAL_RIGHT) && isUsable({maze.p.x +1, maze.p.y})) maze.p.x++;
            
            // Big movement
            else if(IsKeyPressed(DIRECT_UP))    while(isUsable({maze.p.x, maze.p.y -1})){ maze.history.push(maze.p); maze.p.y--; field[conv(maze.p)] = 2; }
            else if(IsKeyPressed(DIRECT_DOWN))  while(isUsable({maze.p.x, maze.p.y +1})){ maze.history.push(maze.p); maze.p.y++; field[conv(maze.p)] = 2; }
            else if(IsKeyPressed(DIRECT_LEFT))  while(isUsable({maze.p.x -1, maze.p.y})){ maze.history.push(maze.p); maze.p.x--; field[conv(maze.p)] = 2; }
            else if(IsKeyPressed(DIRECT_RIGHT)) while(isUsable({maze.p.x +1, maze.p.y})){ maze.history.push(maze.p); maze.p.x++; field[conv(maze.p)] = 2; }
            else maze.history.pop();

            // OOB prevention
            if(maze.p.x < 0 || maze.p.x >= W || maze.p.y < 0 || maze.p.y >= H){
                maze.p = maze.history.top();
                maze.history.pop();
                
            // Reset
            } else if(IsKeyPressed(KEY_SPACE)){
                field[conv(maze.p)] = 0;
                while(!maze.history.empty()){
                    field[conv(maze.history.top())] = 0;
                    maze.history.pop();
                }
                maze.p = {2, 0};
                timer = 0;
            }
            
            // Go back
            if(IsKeyPressed(KEY_B)){
                if(maze.history.empty())continue;
                field[conv(maze.p)] = 0;
                maze.p = maze.history.top();
                maze.history.pop();
            }
            field[conv(maze.p)] = 2;
        }
    }
}
