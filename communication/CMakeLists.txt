add_library(
        CommunicationInterface SHARED
        interfaces.h
        interfaces.cpp
)

add_library(
        Communication SHARED
        communication_client.cpp
        communication_client.h
)

target_include_directories(CommunicationInterface PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include/communication>  # Adjusted for potential install structure
)

target_include_directories(Communication PUBLIC  # Changed from PRIVATE
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include/communication>  # Adjusted for potential install structure
)

target_link_libraries(
        Communication PRIVATE
        CommunicationInterface
        World
)
