include(ResourcesLibrary)

add_executable(Spatial.Sandbox
    src/main.cpp
    src/Sandbox.cpp
    src/SandboxInterface.cpp
)

target_link_libraries(Spatial.Sandbox PUBLIC Spatial.Core)
set_property(TARGET Spatial.Sandbox PROPERTY CXX_STANDARD 17)
set_property(TARGET Spatial.Sandbox PROPERTY CXX_STANDARD_REQUIRED ON)

cmake_policy(SET CMP0091 NEW)
set_property(
	TARGET Spatial.Sandbox
	PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
)

add_material_resources(Sandbox.Assets.Materials
    SOURCES
        assets/materials/default.mat
        assets/materials/plastic.mat
        assets/materials/ui_blit.mat
    OUTPUT
        ${CMAKE_CURRENT_BINARY_DIR}/assets/materials
    DEPENDEES
        Spatial.Sandbox
)

add_mesh_resources(Sandbox.Assets.Models
    SOURCES
        assets/models/sphere.fbx
        assets/models/debug_cube.fbx
    OUTPUT
        ${CMAKE_CURRENT_BINARY_DIR}/assets/models
    DEPENDEES
        Spatial.Sandbox
)

add_copy_resources(Sandbox.Assets.Fonts
    SOURCES
        assets/fonts/Roboto-Regular.ttf
        assets/fonts/Roboto-Medium.ttf
    OUTPUT
        ${CMAKE_CURRENT_BINARY_DIR}/assets/fonts
    DEPENDEES
        Spatial.Sandbox
)

add_copy_resources(Sandbox.Assets.Textures
    SOURCES
        assets/textures/debug_cube.png
    OUTPUT
        ${CMAKE_CURRENT_BINARY_DIR}/assets/textures
    DEPENDEES
        Spatial.Sandbox
)

add_skybox_resources(Sandbox.Assets.Skyboxes
    SOURCES
        assets/textures/pillars_2k.hdr
    FORMAT
        ktx
    OUTPUT
        ${CMAKE_CURRENT_BINARY_DIR}/assets/textures
    DEPENDEES
        Spatial.Sandbox
)