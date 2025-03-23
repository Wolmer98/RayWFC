#include <iostream>
#include <memory>

#include "raylib.h"
#include "World.h"

int main()
{
    const std::size_t screenWidth = 1200;
    const std::size_t screenHeight = 800;

    InitWindow(screenWidth, screenHeight, "RayWFC");
    SetTargetFPS(0);

    std::unique_ptr<World> world = std::make_unique<World>();
    world->GenerateWorld();

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        if (IsKeyPressed(KEY_G))
            world->GenerateWorld();

        // -- Render -- //
        BeginDrawing();
        
        ClearBackground(BLACK);
        world->RenderWorld();
        
        EndDrawing();
    }

    CloseWindow();
}
