#include <cmath>
#include <iostream>
#include <stack>
#include <string>
#include <thread>
#include <unistd.h>
#include <raylib.h>
#include <cstdlib>
#include <vector>

#define rect(x, y, c) DrawRectangle(x *Sc, y *Sc, Sc, Sc, c);
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
    GRID       = WHITE;

// Keys
const int
    NORMAL_UP   = KEY_W,
    NORMAL_DOWN = KEY_S,
    NORMAL_LEFT = KEY_A,
    NORMAL_RIGHT= KEY_D,

    DIRECT_UP   = KEY_UP,
    DIRECT_DOWN = KEY_DOWN,
    DIRECT_LEFT = KEY_LEFT,
    DIRECT_RIGHT= KEY_RIGHT,

    RESET = KEY_SPACE,
    BACK = KEY_B
    ;
const int
    W = 30,
    H = 30,
    Sc = 20,
    step = 2
    ;
const Point start = {2, 0};
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
        up    = {p.x, p.y - step}, down  = {p.x, p.y + step},
        left  = {p.x + step, p.y}, right = {p.x - step, p.y};
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
    srand(time(NULL));
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

    // Finish trigger
    field[W - 1 + 5 * W] = 4;
    field[W - 2 + 5 * W] = 0;
}

int main() {
    
    // Raylib init
    SetTraceLogLevel(TraceLogLevel::LOG_NONE);
    InitWindow(W * Sc, H * Sc, "");
    SetTargetFPS(30);
    float timer = 0;
    
    // Mazegen
    start:
    if(timer != 0.0f) std::cout << std::to_string(timer) << "\n"; timer = 0;
    Point p = start;
    std::stack<Point> history;
    std::thread(CreateField).detach();
    
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
        if(p.y == 5 && p.x == W - 2)goto start;
        else if(p.y == 4 && p.x == W - 1)goto start;
        else if(p.y == 6 && p.x == W - 1)goto start;
        
        // Timer
        timer += 0.033;
        std::string str = std::to_string((std::round(timer * 100) / 100.0f)).substr(0, 5);
        DrawText(str.c_str(), W * Sc- 120, H * Sc -20, 20, PLAYER);
        
        EndDrawing();
        {
            
            // Movement
            history.push(p);
            if(IsKeyPressed(NORMAL_UP) && isUsable({p.x, p.y -1})) p.y--;
            else if(IsKeyPressed(NORMAL_DOWN)  && isUsable({p.x, p.y +1})) p.y++;
            else if(IsKeyPressed(NORMAL_LEFT)  && isUsable({p.x -1, p.y})) p.x--;
            else if(IsKeyPressed(NORMAL_RIGHT) && isUsable({p.x +1, p.y})) p.x++;
            
            // Big movement
            else if(IsKeyPressed(DIRECT_UP))    while(isUsable({p.x, p.y -1})){ history.push(p); p.y--; field[conv(p)] = 2; }
            else if(IsKeyPressed(DIRECT_DOWN))  while(isUsable({p.x, p.y +1})){ history.push(p); p.y++; field[conv(p)] = 2; }
            else if(IsKeyPressed(DIRECT_LEFT))  while(isUsable({p.x -1, p.y})){ history.push(p); p.x--; field[conv(p)] = 2; }
            else if(IsKeyPressed(DIRECT_RIGHT)) while(isUsable({p.x +1, p.y})){ history.push(p); p.x++; field[conv(p)] = 2; }
            else history.pop();
                
            // OOB prevention
            if(p.x < 0 || p.x >= W || p.y < 0 || p.y >= H){
                p = history.top();
                history.pop();
                
            // Reset
            } else if(IsKeyPressed(KEY_SPACE)){
                field[conv(p)] = 0;
                while(!history.empty()){
                    field[conv(history.top())] = 0;
                    history.pop();
                }
                p = start;
                timer = 0;
            }
            
            // Go back
            if(IsKeyPressed(KEY_B)){
                if(history.empty())continue;
                field[conv(p)] = 0;
                p = history.top();
                history.pop();
            }
            field[conv(p)] = 2;
        }
    }
}
