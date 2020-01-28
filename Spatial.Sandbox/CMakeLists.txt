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

add_resources_library(Sandbox.Assets.Materials
    MATERIAL
        assets/materials/default.mat
        assets/materials/plastic.mat
        assets/materials/ui_blit.mat
    OUTPUT
        ${CMAKE_CURRENT_BINARY_DIR}/assets/materials
    DEPENDEES
        Spatial.Sandbox
)

add_resources_library(Sandbox.Assets.Models
    MODEL
        assets/models/sphere.fbx
        assets/models/debug_cube.fbx
    OUTPUT
        ${CMAKE_CURRENT_BINARY_DIR}/assets/models
    DEPENDEES
        Spatial.Sandbox
)

add_resources_library(Sandbox.Assets.Fonts
    COPY
        assets/fonts/Roboto-Regular.ttf
        assets/fonts/Roboto-Medium.ttf
    OUTPUT
        ${CMAKE_CURRENT_BINARY_DIR}/assets/fonts
    DEPENDEES
        Spatial.Sandbox
)

add_resources_library(Sandbox.Assets.Textures
    COPY
        assets/textures/debug_cube.png
    OUTPUT
        ${CMAKE_CURRENT_BINARY_DIR}/assets/textures
    DEPENDEES
        Spatial.Sandbox
)

add_resources_library(Sandbox.Assets.Skyboxes
    SKYBOX
        assets/textures/pillars_2k.hdr
    OUTPUT
        ${CMAKE_CURRENT_BINARY_DIR}/assets/textures
    DEPENDEES
        Spatial.Sandbox
)