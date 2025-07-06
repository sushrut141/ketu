# Define the library target for your features
add_library(Objects SHARED node.cpp node.h)

# Specify its own include directories if it has public headers
target_include_directories(Objects PUBLIC # Changed from PRIVATE
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include/objects> # Adjusted
)

target_link_libraries(Objects PRIVATE
        Sensing
        Communication
        CommunicationInterface
        Telemetry
        Formation
)