# Define the library target for your features
add_library(
        Telemetry SHARED
        position.cpp
        position.h
        position_util.cpp
        position_utils.h
)

# Specify its own include directories if it has public headers
target_include_directories(Telemetry PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include/telemetry>
)

target_link_libraries(Telemetry)