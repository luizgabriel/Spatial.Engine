include(ResourcesLibrary)

add_executable(Spatial.Editor
    src/main.cpp
    src/EditorSystem.cpp
    src/Components.h
    src/DebugMeshSystem.h
    src/DebugMeshSystem.cpp
)
target_link_libraries(Spatial.Editor PUBLIC Spatial.Core)

cmake_policy(SET CMP0091 NEW)
set_property(
    TARGET Spatial.Editor
    PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
)

add_subdirectory(assets)