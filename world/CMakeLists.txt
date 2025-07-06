# Define the target for your utility library
# Use add_library() to create a static or shared library
add_library(World world.cpp world.h) # Add all source and header files for this package


# Define public include directories for this library
# This makes its headers available to projects that link against it (like MyOsgProject)
target_include_directories(World PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include/world>
)

target_link_libraries(World PRIVATE Telemetry)