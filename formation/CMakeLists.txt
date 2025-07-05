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

# Testing
# Assuming GTest has been found (e.g., via find_package(GTest REQUIRED) in a higher level CMakeLists.txt)
# If not, these linkings might not resolve, or GTest needs to be added as a subdirectory.
if(TARGET GTest::gtest AND TARGET GTest::gmock)
    add_executable(
            FormationCoordinatorTests
            composite_formation_coordinator_test.cpp
    )

    target_link_libraries(
            FormationCoordinatorTests PRIVATE
            FormationCoordinator # Link against the library being tested
            GTest::gtest
            GTest::gmock
            GTest::gtest_main # or GTest::gmock_main depending on preference/setup
    )

    # Add include directories for the test target if necessary
    # target_include_directories(FormationCoordinatorTests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    # This line might be needed if composite_formation_coordinator.h is not found otherwise by tests
    # However, FormationCoordinator library already exports its include directories.

    # Enable C++17 for the test target, if needed (assuming it's used by tests)
    # target_compile_features(FormationCoordinatorTests PRIVATE cxx_std_17)

    # Discover and add tests to CTest (optional, but good practice)
    include(GoogleTest)
    gtest_discover_tests(FormationCoordinatorTests)
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
