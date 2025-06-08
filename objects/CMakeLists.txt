
# Define the library target for your features
add_library(Objects SHARED node.cpp node.h)

# Specify its own include directories if it has public headers
target_include_directories(Objects PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(Objects PUBLIC World)