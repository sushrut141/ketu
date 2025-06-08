# Define the library target for your features
add_library(Telemetry SHARED position.cpp position.h)

# Specify its own include directories if it has public headers
target_include_directories(Telemetry PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(Telemetry)