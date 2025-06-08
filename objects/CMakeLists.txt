# Define the library target for your features
add_library(Objects SHARED node.cpp node.h)

# Specify its own include directories if it has public headers
target_include_directories(Objects PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>
        Sensing
        Communication
)

target_link_libraries(Objects PRIVATE
        Sensing
        Communication
)