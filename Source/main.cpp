#include "raylib.h"
#include "level.h"


int main(void)
{    
    const int screenWidth = 800;
    const int screenHeight = 800;

    SetConfigFlags(ConfigFlags::FLAG_WINDOW_RESIZABLE | ConfigFlags::FLAG_VSYNC_HINT | ConfigFlags::FLAG_MSAA_4X_HINT);
    InitWindow(screenWidth, screenHeight, "A* pathfinding");

    // SetTargetFPS(60); // Uncomment this if you want to use fixed framerate (I suggest using vsync)
    // SetExitKey(0);    // Uncomment this to disable ESC as exit key

    Level level;

    level.reset();

    while (!WindowShouldClose())
    {
        // Update
        level.update();

        // Draw
        BeginDrawing();
        ClearBackground(RAYWHITE);
        level.draw();
        EndDrawing();
    }

    CloseWindow();

    return 0;
}