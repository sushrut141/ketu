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
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>
)

target_include_directories(Communication PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(
        Communication PRIVATE
        CommunicationInterface
        World
)
