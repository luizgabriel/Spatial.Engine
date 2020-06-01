include(ResourcesLibrary)

add_executable(Spatial.Sandbox
        src/main.cpp
        src/Sandbox.cpp
)

target_link_libraries(Spatial.Sandbox PUBLIC Spatial.Core)
set_property(TARGET Spatial.Sandbox PROPERTY CXX_STANDARD 20)
set_property(TARGET Spatial.Sandbox PROPERTY CXX_STANDARD_REQUIRED ON)

cmake_policy(SET CMP0091 NEW)
set_property(
        TARGET Spatial.Sandbox
        PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
)

add_material_resources(Sandbox.Assets.Materials
    SOURCES
        assets/materials/default.mat
        assets/materials/ui_blit.mat
    OUTPUT assets/materials
    DEPENDEES Spatial.Sandbox
)

add_mesh_resources(Sandbox.Assets.Models
    SOURCES
        assets/models/sphere.fbx
        assets/models/debug_cube.fbx
    OUTPUT
        assets/models
    DEPENDEES
        Spatial.Sandbox
)

add_copy_resources(Sandbox.Assets.Config
    SOURCES
        assets/config/application.toml
    OUTPUT assets/config
    DEPENDEES Spatial.Sandbox
)

add_copy_resources(Sandbox.Assets.Fonts
    SOURCES
        assets/fonts/Roboto-Regular.ttf
        assets/fonts/Roboto-Medium.ttf
    OUTPUT assets/fonts
    DEPENDEES Spatial.Sandbox
)

add_copy_resources(Sandbox.Assets.Textures
    SOURCES
        assets/textures/debug_cube.png
    OUTPUT assets/textures
    DEPENDEES Spatial.Sandbox
)

add_skybox_resources(Sandbox.Assets.Skyboxes
    SOURCES
        assets/textures/pillars_2k.hdr
        assets/textures/railway_bridge_2k.hdr
    FORMAT ktx
    RESOLUTION 4K
    OUTPUT assets/textures
    DEPENDEES Spatial.Sandbox
)

install(
    FILES
        assets/fonts/Roboto-Medium.ttf
        assets/fonts/Roboto-Regular.ttf
    DESTINATION assets/fonts
)

install(
    FILES
        assets/materials/default.mat
        assets/materials/ui_blit.mat
    DESTINATION assets/materials
)

install(
    FILES
        assets/textures/uv_test.png
        assets/textures/pillars_2k.hdr
    DESTINATION assets/textures
)