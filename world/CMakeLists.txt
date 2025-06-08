# Define the target for your utility library
# Use add_library() to create a static or shared library
add_library(World world.cpp world.h) # Add all source and header files for this package

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

# Define public include directories for this library
# This makes its headers available to projects that link against it (like MyOsgProject)
target_include_directories(World PRIVATE
        $<INSTALL_INTERFACE:include/world> # For installation
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}> # For local build within the project
        ${OPENSCENEGRAPH_INCLUDE_DIRS}
)

target_link_libraries(World PRIVATE ${OPENSCENEGRAPH_LIBRARIES})