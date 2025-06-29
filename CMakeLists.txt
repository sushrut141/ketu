cmake_minimum_required(VERSION 3.10) # Adjust if your CMake is older or newer

project(Ketu CXX) # Set project name and specify C++

set(CMAKE_CXX_STANDARD 17) # Or 11, 14, 20 depending on your C++ standard preference
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF) # Use strict C++ standard


add_subdirectory(world)
add_subdirectory(objects)
add_subdirectory(sensing)
add_subdirectory(communication)
add_subdirectory(telemetry)
add_subdirectory(scenarios)
add_subdirectory(formation)
add_subdirectory(planning)
add_subdirectory(third_party)


add_executable(main main.cpp)

target_link_libraries(main
        Scenarios
        TinyObj
)

file(COPY "${CMAKE_SOURCE_DIR}/scenarios/raylib/shaders" DESTINATION "${CMAKE_BINARY_DIR}/")
file(COPY "${CMAKE_SOURCE_DIR}/mesh.obj" DESTINATION "${CMAKE_BINARY_DIR}/")