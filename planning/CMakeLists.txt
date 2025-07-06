# Define the library target for your features
add_library(
        Planning SHARED
        movement.cpp
        movement.h
)

# Specify its own include directories if it has public headers
target_include_directories(Planning PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include/planning>
)

target_link_libraries(
        Planning PRIVATE
        CommunicationInterface
        Telemetry
)