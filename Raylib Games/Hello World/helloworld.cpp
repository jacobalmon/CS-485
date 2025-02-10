#include <raylib.h>

int main() {
    InitWindow(1280, 720, "Hello World");

    while (!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawText("Hello World!", 500, 360, 42, RED);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}