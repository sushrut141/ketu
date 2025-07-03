#include "scenario.h"

#include <cfloat>
#include <iostream>
#include <unordered_set>

#include <raylib.h>
#include <raymath.h>
#include "raylib/rlights.h"

namespace ketu::scenarios
{
    constexpr int LIGHT_DISTANCE = 10.0;
    constexpr double SPHERE_RADIUS = 0.2f;

    Scenario::Scenario(std::unique_ptr<ketu::world::World> world)
    {
        world_ = std::move(world);
        frameNumber_ = 0;
    }

    void Scenario::run()
    {
        SetConfigFlags(FLAG_MSAA_4X_HINT);
        SetConfigFlags(FLAG_FULLSCREEN_MODE);
        InitWindow(GetScreenWidth(), GetScreenHeight(), "Ketu");

        // Define the camera parameters (position, target, up, field of view)
        Camera camera = {0}; // Initialize camera struct to zero
        camera.position = (Vector3){-5.0f, 5.0f, 10.0f}; // Camera position
        camera.target = (Vector3){0.0f, 0.0f, 0.0f}; // Camera looks at this point
        camera.up = (Vector3){0.0f, 1.0f, 0.0f}; // Camera up vector
        camera.fovy = 45.0f; // Camera field-of-view in Y-direction
        camera.projection = CAMERA_PERSPECTIVE; // Use perspective projection

        UpdateCamera(&camera, CAMERA_ORBITAL);

        Shader shader = LoadShader(TextFormat("shaders/lighting.vs", GLSL_VERSION),
                                   TextFormat("shaders/lighting.fs", GLSL_VERSION));
        // Get some required shader locations
        shader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(shader, "viewPos");

        int ambientLoc = GetShaderLocation(shader, "ambient");
        SetShaderValue(shader, ambientLoc, (float[4]){0.1f, 0.1f, 0.1f, 1.0f}, SHADER_UNIFORM_VEC4);

        Light lights[MAX_LIGHTS] = {0};
        lights[0] = CreateLight(LIGHT_POINT, (Vector3){LIGHT_DISTANCE, LIGHT_DISTANCE, LIGHT_DISTANCE}, Vector3Zero(),
                                WHITE, shader);
        lights[1] = CreateLight(LIGHT_POINT, (Vector3){LIGHT_DISTANCE, LIGHT_DISTANCE, -LIGHT_DISTANCE}, Vector3Zero(),
                                WHITE, shader);
        lights[2] = CreateLight(LIGHT_POINT, (Vector3){-LIGHT_DISTANCE, LIGHT_DISTANCE, LIGHT_DISTANCE}, Vector3Zero(),
                                WHITE, shader);
        lights[3] = CreateLight(LIGHT_POINT, (Vector3){-LIGHT_DISTANCE, LIGHT_DISTANCE, -LIGHT_DISTANCE}, Vector3Zero(),
                                WHITE, shader);

        std::unordered_set<std::string> selectedIds;

        while (!WindowShouldClose())
        {

            float cameraPos[3] = {camera.position.x, camera.position.y, camera.position.z};
            SetShaderValue(shader, shader.locs[SHADER_LOC_VECTOR_VIEW], cameraPos, SHADER_UNIFORM_VEC3);

            for (int i = 0; i < MAX_LIGHTS; i++)
            {
                UpdateLightValues(shader, lights[i]);
            }

            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                Ray mouseRay = GetMouseRay(GetMousePosition(), camera);
                for (const auto& node_id_pos_pair : world_->getNodePositions())
                {
                    const auto& node_position = node_id_pos_pair.second;
                    Vector3 spherePosition = {static_cast<float>(node_position.getX()),
                                              static_cast<float>(node_position.getY()),
                                              static_cast<float>(node_position.getZ())};
                    RayCollision collision = GetRayCollisionSphere(mouseRay, spherePosition, SPHERE_RADIUS);
                    if (collision.hit)
                    {
                        if (selectedIds.empty())
                        {
                            selectedIds.insert(node_id_pos_pair.first);
                        }
                        else
                        {
                            selectedIds.clear();
                            selectedIds.insert(node_id_pos_pair.first);
                        }
                        break;
                    }
                }
            }
            else if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
            {
                UpdateCamera(&camera, CAMERA_CUSTOM);
                if (!selectedIds.empty())
                {
                    Vector2 mouseDelta = GetMouseDelta();
                    std::cout << "Moving entity " << *selectedIds.begin() << " to X: " << mouseDelta.x
                              << " Y: " << mouseDelta.y << std::endl;
                    onSelectedEntityMoved(*selectedIds.begin(), mouseDelta.x, mouseDelta.y);
                }
            }
            else if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
            {
                selectedIds.clear();
            } else
            {
                UpdateCamera(&camera, CAMERA_ORBITAL);
            }

            onTick(frameNumber_);

            BeginDrawing(); // Start drawing operations

            ClearBackground(RAYWHITE); // Clear the background with a white color

            BeginMode3D(camera); // Start 3D drawing with the defined camera

            BeginShaderMode(shader);

            for (const auto& node_id_pos_pair : world_->getNodePositions())
            {
                const auto& node_position = node_id_pos_pair.second;
                Vector3 spherePosition;
                spherePosition.x = node_position.getX();
                spherePosition.y = node_position.getY();
                spherePosition.z = node_position.getZ();

                // Draw the sphere
                Color sphereColor = BLUE;
                if (node_id_pos_pair.first.find("leader") != std::string::npos)
                {
                    sphereColor = RED;
                }
                if (selectedIds.find(node_id_pos_pair.first) != selectedIds.end())
                {
                    sphereColor = GREEN;
                }
                DrawSphere(spherePosition, 0.1f, sphereColor);
            }

            DrawPlane((Vector3){0.0f, -2.0f, 0.0f}, (Vector2){20.0, 20.0}, GRAY);
            EndShaderMode();

            DrawGrid(20, 1.0f); // Larger grid for larger movement range

            EndMode3D(); // End 3D drawing

            DrawFPS(10, 10); // Display FPS at top-left corner

            EndDrawing(); // End drawing and swap buffers

            frameNumber_ += 1;
        }
    }

} // namespace ketu::scenarios
