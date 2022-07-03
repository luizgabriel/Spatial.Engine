find_program(FILAMESH_COMMAND filamesh)
add_custom_command(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/assets/plane.filamesh
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/assets/plane.obj
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/assets/
        COMMAND ${FILAMESH_COMMAND} assets/plane.obj ${CMAKE_CURRENT_BINARY_DIR}/assets/plane.filamesh
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(Spatial.Core.Tests
        Application.test.cpp
        RegistrySerialization.test.cpp
        RegistryRelation.test.cpp
        RegistryResource.test.cpp
        DesktopWindowContext.test.cpp
        FileSystem.test.cpp
        main.cpp)

set_property(
        TARGET Spatial.Core.Tests
        PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

target_link_libraries(Spatial.Core.Tests
        PUBLIC
            Spatial.Core
            CONAN_PKG::gtest)

gtest_discover_tests(Spatial.Core.Tests)

file(COPY fs DESTINATION ${CMAKE_CURRENT_BINARY_DIR})