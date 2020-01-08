include(ResourcesLibrary)

add_executable(Spatial.Sandbox
    src/main.cpp
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
    MATERIALS
        assets/materials/plastic.mat
        assets/materials/ui_blit.mat
    DEPENDEES
        Spatial.Sandbox
    OUTPUT
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets/materials
)

add_resources_library(Sandbox.Assets.Fonts
    COPY
        assets/fonts/Roboto-Regular.ttf
        assets/fonts/Roboto-Medium.ttf
    DEPENDEES
        Spatial.Sandbox
    OUTPUT
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets/fonts
)