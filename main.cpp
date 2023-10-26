#include <iostream>
#include <stdio.h>
#include <raylib.h>
#include <raymath.h>
#include <stdlib.h>
#include <time.h>
#include <chrono>
#include <list>
 
using namespace std;
 
class Ball {
public:
    Vector2 position;
    Vector2 velocity;
    float radius;
    Color color;
};
 
void updateBall(Ball &ball, float gravity, float frictionCoefficient, float deltaTime, float screenWidth, float screenHeight, float bounciness, std::list<Ball> &balls) {
      
     for (auto &otherBall : balls) {
        // Avoid checking the ball against itself
        if (&otherBall == &ball) {
            continue;
        }
 
        // Calculate the distance between the two balls
        float distance = Vector2Distance(ball.position, otherBall.position);
 
        // Check if a collision has occurred
        if (distance < (ball.radius + otherBall.radius)) {
            ball.velocity.x = -(ball.velocity.x);
            otherBall.velocity.x = -(ball.velocity.x);
            ball.velocity.y = -(ball.velocity.y);
            otherBall.velocity.y = -(otherBall.velocity.y);
        }
    }
 
    ball.velocity.y -= gravity;
 
    // Apply friction if the ball is at the top or bottom of the screen
    ball.velocity.x *= (ball.position.y >= screenHeight - ball.radius || ball.position.y <= ball.radius) ? frictionCoefficient : 1;
 
    // Update the ball's position based on its velocity
    ball.position.x += ball.velocity.x * deltaTime;
    ball.position.y += -ball.velocity.y * deltaTime;
 
    // Clamp the ball's position within the screen bounds
    ball.position.x = Clamp(ball.position.x, ball.radius, screenWidth - ball.radius);
    ball.position.y = Clamp(ball.position.y, ball.radius, screenHeight - ball.radius);
 
    // Check for collisions with screen edges and apply bounciness
    ball.velocity.x *= (ball.position.x == ball.radius || ball.position.x == screenWidth - ball.radius) ? -bounciness : 1;
    ball.velocity.y *= (ball.position.y == ball.radius || ball.position.y == screenHeight - ball.radius) ? -bounciness : 1;
 
}
 
int randRange(int lo, int hi) {
    auto now = chrono::system_clock::now();
    int milliseconds = chrono::duration_cast<chrono::milliseconds>(now.time_since_epoch()).count();
    srand(milliseconds);
    if (lo == 0) {
        return rand() % hi;
    } else {
        return rand() % lo + hi;
    }
}
 
Color randColor() {
    auto now = chrono::system_clock::now();
    int milliseconds = chrono::duration_cast<chrono::milliseconds>(now.time_since_epoch()).count();
    srand(milliseconds);
    int r = rand() % 255;
    int g = rand() % 255;
    int b = rand() % 255;
    int a = rand() % 255;
    unsigned char rc = (unsigned char) r;
    unsigned char gc = (unsigned char) g;
    unsigned char bc = (unsigned char) b;
    unsigned char ac = (unsigned char) a;
    return Color{rc, gc, bc, ac};
}
 
Color randColorOpaque() {
    auto now = chrono::system_clock::now();
    int milliseconds = chrono::duration_cast<chrono::milliseconds>(now.time_since_epoch()).count();
    srand(milliseconds);
    int r = rand() % 255;
    int g = rand() % 255;
    int b = rand() % 255;
    unsigned char rc = (unsigned char) r;
    unsigned char gc = (unsigned char) g;
    unsigned char bc = (unsigned char) b;
    unsigned char ac = (unsigned char) 255;
    return Color{rc, gc, bc, ac};
}
 
Ball newBall(Vector2 position, Vector2 velocity, float radius, Color color) {
    Ball tempBall;
    tempBall.position = position;
    tempBall.velocity = velocity;
    tempBall.radius = radius;
    tempBall.color = color;
    return tempBall;
}
 
int main() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
 
    int screenWidth = 900;
    int screenHeight = 900;
 
    InitWindow(screenWidth, screenHeight, "cppPhysics");
    SetTargetFPS(120);
 
    const float gravity = 9.81f;
    const float frictionCoefficient = 0.981f;
    const float bounciness = 0.7f;
 
    Ball ballOne = newBall({100, 100}, {400, -450}, randRange(5, 40), randColorOpaque());
    Ball ballTwo = newBall({50, 200}, {400, 450}, randRange(5, 50), randColor());
    Ball ballThree = newBall({200, 50}, {300, -450}, randRange(10, 30), randColor());
 
    std::list<Ball> balls;
    balls.push_back(ballOne);
    balls.push_back(ballTwo);
    balls.push_back(ballThree);
 
    while (!WindowShouldClose()) {
        if (IsWindowResized()) {
            screenWidth = GetScreenWidth();
            screenHeight = GetScreenHeight();
        }
        float deltaTime = GetFrameTime();
 
        for (auto &ball : balls) {
            updateBall(ball, gravity, frictionCoefficient, deltaTime, screenWidth, screenHeight, bounciness, balls);
        }
 
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            for (auto &ball : balls) {
                ball.velocity.y += randRange(-400, 1000);
                ball.velocity.x += randRange(-400, 1000);
            }
        }
 
        BeginDrawing();
        ClearBackground(BLACK);
 
        for (auto &ball : balls) {
            DrawCircleV(ball.position, ball.radius, ball.color);
        }
 
        EndDrawing();
    }
 
    CloseWindow();
    return 0;
}
 
