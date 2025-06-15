#include "scenario.h"

#include <raylib.h>
#include <raymath.h>

namespace ketu::scenarios
{

    Scenario::Scenario(std::unique_ptr<ketu::world::World> world)
    {
        world_ = std::move(world);
        frameNumber_ = 0;
    }

    void Scenario::run()
    {
        InitWindow(GetScreenWidth(), GetScreenHeight(), "Ketu");

        // Define the camera parameters (position, target, up, field of view)
        Camera camera = {0}; // Initialize camera struct to zero
        camera.position = (Vector3){0.0f, 10.0f, 10.0f}; // Camera position
        camera.target = (Vector3){0.0f, 0.0f, 0.0f}; // Camera looks at this point
        camera.up = (Vector3){0.0f, 1.0f, 0.0f}; // Camera up vector
        camera.fovy = 45.0f; // Camera field-of-view in Y-direction
        camera.projection = CAMERA_PERSPECTIVE; // Use perspective projection

        Color sphereColor = BLUE;
        while (!WindowShouldClose())
        {
            onTick(frameNumber_);

            BeginDrawing(); // Start drawing operations

            ClearBackground(RAYWHITE); // Clear the background with a white color

            BeginMode3D(camera); // Start 3D drawing with the defined camera

            for (const auto& node_id_pos_pair : world_->getNodePositions())
            {
                const auto& node_position = node_id_pos_pair.second;
                Vector3 spherePosition;
                spherePosition.x = node_position.getX();
                spherePosition.y = node_position.getY();
                spherePosition.z = node_position.getZ();

                // Draw the sphere
                DrawSphere(spherePosition, 0.2f, sphereColor);

                // Optionally, draw a grid to help visualize the 3D space
                DrawGrid(20, 1.0f); // Larger grid for larger movement range
            }

            EndMode3D(); // End 3D drawing

            DrawFPS(10, 10); // Display FPS at top-left corner

            EndDrawing(); // End drawing and swap buffers

            frameNumber_ += 1;
        }
    }

} // namespace ketu::scenarios
