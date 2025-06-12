add_library(
        Scenarios
        random_nodes.h
        random_nodes.cpp
        scenario.h
        scenario.cpp
)

find_package(raylib CONFIG REQUIRED)

# Check if OpenSceneGraph was found
if (NOT raylib_FOUND)
    message(FATAL_ERROR "Raylib not found. Please ensure it's installed and discoverable by CMake.")
endif()

# Define public include directories for this library
# This makes its headers available to projects that link against it (like MyOsgProject)
target_include_directories(Scenarios PUBLIC
        $<INSTALL_INTERFACE:include> # For installation
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}> # For local build within the project
        ${raylib_INCLUDE_DIRS}
        ${raylib_INCLUDE_DIRS}/raylib
)

target_link_libraries(Scenarios PRIVATE
        ${raylib_LIBRARIES}
        Telemetry
        World
        Objects
        Sensing
        Communication
)