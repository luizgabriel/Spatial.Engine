add_executable(Spatial.Graphics.Tests
        RenderingSystem.test.cpp
        main.cpp)

set_property(
        TARGET Spatial.Graphics.Tests
        PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

target_link_libraries(Spatial.Graphics.Tests
        PUBLIC
            Spatial.Graphics
            gtest::gtest)

gtest_discover_tests(Spatial.Graphics.Tests)
