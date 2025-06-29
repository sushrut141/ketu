# Define the library target for your features
add_library(
        TinyObj SHARED
        tinyobj/tinyobjloader.h
        tinyobj/loader.h
        tinyobj/loader.cc
)

# Specify its own include directories if it has public headers
target_include_directories(TinyObj PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>
        Telemetry
)

target_link_libraries(
        TinyObj PUBLIC
        Telemetry
)