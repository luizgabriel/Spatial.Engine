add_executable(Spatial.Core.Tests
        Application.test.cpp
        RegistrySerialization.test.cpp
        RegistryRelation.test.cpp
        RegistryResource.test.cpp
        FileSystem.test.cpp
        main.cpp)

set_property(
        TARGET Spatial.Core.Tests
        PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

target_link_libraries(Spatial.Core.Tests
        PUBLIC
            Spatial.Core
            gtest::gtest)

gtest_discover_tests(Spatial.Core.Tests)

file(COPY fs DESTINATION ${CMAKE_CURRENT_BINARY_DIR})