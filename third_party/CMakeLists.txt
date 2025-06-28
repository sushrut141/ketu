# Define the library target for your features
add_library(
        TinyObj SHARED
        tinyobj/tinyobjloader.h
        tinyobj/tinyobjloader.cc
)

# Specify its own include directories if it has public headers
target_include_directories(TinyObj PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(TinyObj)