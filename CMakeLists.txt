cmake_minimum_required(VERSION 3.10) # Adjust if your CMake is older or newer

project(Ketu CXX) # Set project name and specify C++

set(CMAKE_CXX_STANDARD 17) # Or 11, 14, 20 depending on your C++ standard preference
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF) # Use strict C++ standard


add_subdirectory(world)
add_subdirectory(objects)
add_subdirectory(sensing)
add_subdirectory(communication)

# Define your executable
add_executable(main main.cpp)

# Link the OpenSceneGraph libraries to your executable
target_link_libraries(main
        World
)

# (Optional) Copy cessna.osg to the build directory for running
# This assumes cessna.osg is in the same directory as CMakeLists.txt
file(COPY "${CMAKE_SOURCE_DIR}/cessna.osg" DESTINATION "${CMAKE_BINARY_DIR}/")