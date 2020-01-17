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
        assets/materials/plastic.mat
        assets/materials/ui_blit.mat
    OUTPUT
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets/materials
    DEPENDEES
        Spatial.Sandbox
)

add_resources_library(Sandbox.Assets.Models
    MODEL
        assets/models/sphere.fbx
    OUTPUT
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets/models
    DEPENDEES
        Spatial.Sandbox
)

add_resources_library(Sandbox.Assets.Fonts
    COPY
        assets/fonts/Roboto-Regular.ttf
        assets/fonts/Roboto-Medium.ttf
    OUTPUT
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets/fonts
    DEPENDEES
        Spatial.Sandbox
)