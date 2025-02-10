#include <raylib.h>

const int W = 800;
const int H = 800;

struct Player {
    Vector2 size;
    Vector2 pos;
    Vector2 vel;
};

void drawPlayer(const Player* player) {
    DrawRectangle(player->pos.x, player->pos.y, player->size.x, player->size.y, SKYBLUE);
}

void movePlayer(Player* player) {
    // Reset velocity.
    player->vel.x = 0.0f;
    player->vel.y = 0.0f;

    // Movement Controls
    if (IsKeyDown(KEY_UP)) {
        player->vel.y -= 1000.0f * GetFrameTime();
    } 
    if (IsKeyDown(KEY_DOWN)) {
        player->vel.y += 1000.0f * GetFrameTime();
    }
    if (IsKeyDown(KEY_LEFT)) {
        player->vel.x -= 1000.0f * GetFrameTime();
    } 
    if (IsKeyDown(KEY_RIGHT)) {
        player->vel.x += 1000.0f * GetFrameTime();
    }
}

void applyVelocity(Player* player) {
    player->pos.x += player->vel.x;
    player->pos.y += player->vel.y;
}

int main() {
    InitWindow(W, H, "Rectangle");
    SetTargetFPS(60); // Vsync

    Player player = {
        {32.0f, 32.0f},  // size.
        {300.0f, 300.0f}, // pos.
        {0.0f, 0.0f}  // vel.
    };

    while (!WindowShouldClose()) {
        movePlayer(&player);
        applyVelocity(&player);

        // Drawing on Screen.
        BeginDrawing();
        ClearBackground(RAYWHITE);
        drawPlayer(&player);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}