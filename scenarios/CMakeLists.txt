add_library(Scenarios random_nodes.cpp random_nodes.h simulation_viewer.h scenario.h)

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
target_include_directories(Scenarios PUBLIC
        $<INSTALL_INTERFACE:include> # For installation
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}> # For local build within the project
        ${OPENSCENEGRAPH_INCLUDE_DIRS}
)

target_link_libraries(Scenarios PRIVATE
        ${OPENSCENEGRAPH_LIBRARIES}
        Telemetry
        World
        Objects
        Sensing
        Communication
)