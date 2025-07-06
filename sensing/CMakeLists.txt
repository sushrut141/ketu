# Define the library target for your features
add_library(Sensing SHARED sensing_client.cpp sensing_client.h)

# Specify its own include directories if it has public headers
target_include_directories(Sensing PUBLIC # Changed from PRIVATE
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include/sensing>
)

target_link_libraries(
        Sensing PRIVATE
        World
        Telemetry
)