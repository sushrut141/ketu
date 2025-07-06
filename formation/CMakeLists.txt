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
        composite_formation_coordinator.cpp # Added source
        composite_formation_coordinator.h # Added header
)

target_include_directories(Formation PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include/formation>
)

target_include_directories(FormationCoordinator PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include/formation>
)

# Testing
if(TARGET GTest::gtest AND TARGET GTest::gmock)
    # No tests defined in this file currently.
endif()

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
