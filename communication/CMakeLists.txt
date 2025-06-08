# Define the library target for your features
add_library(Communication SHARED communication_client.cpp communication_client.h)

# Specify its own include directories if it has public headers
target_include_directories(Communication PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(Communication PRIVATE World)