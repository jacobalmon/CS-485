#include <raylib.h>
#include <cmath>

const int H = 800;
const int W = 800;
const float FRICTION_DRAG = 0.02f;

struct Ball {
  Vector2 pos;
  Vector2 vel;
  float radius;
  float ang;
  Color color;
};

void drawBall(const Ball *ball) {
  DrawCircle(ball->pos.x,
             ball->pos.y,
             ball->radius,
             ball->color);
  DrawLine(ball->pos.x,
           ball->pos.y,
           ball->pos.x + cos(ball->ang) * ball->radius,
           ball->pos.y + sin(ball->ang) * ball->radius,
           BLACK);
}

void moveBall(Ball *ball) {
  // Hold SPACE for "spacy" movement
  if (IsKeyUp(KEY_SPACE)) {
    ball->vel.x = 0.0f;
    ball->vel.y = 0.0f;
  }
  if (IsKeyDown(KEY_W)) {
    ball->vel.x = cos(ball->ang) * 100.0f;
    ball->vel.y = sin(ball->ang) * 100.0f;
  } else if (IsKeyDown(KEY_S)) {
    ball->vel.x = cos(ball->ang) * -100.0f;
    ball->vel.y = sin(ball->ang) * -100.0f;
  }
  if (IsKeyDown(KEY_A)) {
    ball->ang = std::fmod(ball->ang - 0.002f, 6.28318);
  } else if (IsKeyDown(KEY_D)) {
    ball->ang = std::fmod(ball->ang + 0.002f, 6.28318);
  }
}

void applyVelocity(Ball *ball) {
  ball->pos.x += ball->vel.x * GetFrameTime();
  ball->pos.y += ball->vel.y * GetFrameTime();
}

int main () {

  InitWindow(W, H, "Ball");

  // Ball ball = {.x = 400.0f, .y = 400.0f},
  //           {.x = 0.0f, .y = 0.0f},
  //                  .radius = 32.0f,
  //                  .ang = 0.0f,
  //                  .color = SKYBLUE
  // };

  Ball ball = {
    {400.0f, 400.0f},
    {0.0f, 0.0f},
    {32.0f},
    {0.0f},
    {SKYBLUE}
  };

  // Main Game Loop
  while (!WindowShouldClose()) {

    // Begin Updates
    moveBall(&ball);
    applyVelocity(&ball);

    // Begin Draw Loop
    BeginDrawing();
    ClearBackground(RAYWHITE);
    drawBall(&ball);
    EndDrawing();
  }

  CloseWindow();

  return 0;
}