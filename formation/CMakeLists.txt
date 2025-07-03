add_library(
        Formation SHARED
        interface.h
        interface.cpp
)


add_library(
        FormationCoordinator SHARED
        mesh_based_formation_coordinator.cpp
        mesh_based_formation_coordinator.h
        proximity_based_formation_coordinator.cpp
        proximity_based_formation_coordinator.h
        formation_coordinator.h
)

target_include_directories(Formation PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>
        CommunicationInterface
        Telemetry
)

target_include_directories(FormationCoordinator PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>
        World
        CommunicationInterface
        Telemetry
        Planning
        TinyObj
)

target_link_libraries(
        Formation PRIVATE
        CommunicationInterface
        Telemetry
)

target_link_libraries(
        FormationCoordinator PRIVATE
        World
        CommunicationInterface
        Telemetry
        Planning
        TinyObj
)
