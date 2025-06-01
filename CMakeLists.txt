cmake_minimum_required(VERSION 3.10) # Adjust if your CMake is older or newer

project(OpenSceneGraphCapture CXX) # Set project name and specify C++

set(CMAKE_CXX_STANDARD 17) # Or 11, 14, 20 depending on your C++ standard preference
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF) # Use strict C++ standard

# Find OpenSceneGraph and its components
# CLion will use CMake's find_package mechanism.
# It will look for OpenSceneGraphConfig.cmake or FindOpenSceneGraph.cmake
# If you installed with Homebrew, CMake should find it automatically if PKG_CONFIG_PATH or CMAKE_PREFIX_PATH is set.
find_package(OpenSceneGraph REQUIRED
        osg
        osgDB
        osgGA
        osgViewer
        osgText
        osgUtil
        OpenThreads # OSG often depends on OpenThreads
)

# Check if OpenSceneGraph was found
if (NOT OPENSCENEGRAPH_FOUND)
    message(FATAL_ERROR "OpenSceneGraph not found. Please set OpenSceneGraph_DIR or CMAKE_PREFIX_PATH.")
endif()

# Define your executable
add_executable(capture_views capture_views.cpp)

# Link the OpenSceneGraph libraries to your executable
target_link_libraries(capture_views
        ${OPENSCENEGRAPH_LIBRARIES}
)

# Include directories for OSG headers
target_include_directories(capture_views PRIVATE
        ${OPENSCENEGRAPH_INCLUDE_DIRS}
)

# (Optional) Copy cessna.osg to the build directory for running
# This assumes cessna.osg is in the same directory as CMakeLists.txt
file(COPY "${CMAKE_SOURCE_DIR}/cessna.osg" DESTINATION "${CMAKE_BINARY_DIR}/")