include(ResourcesLibrary)

add_executable(Spatial.Sandbox
    src/main.cpp
    src/engine.cpp
)

target_link_libraries(Spatial.Sandbox PUBLIC Spatial.Core)
set_property(TARGET Spatial.Sandbox PROPERTY CXX_STANDARD 17)
set_property(TARGET Spatial.Sandbox PROPERTY CXX_STANDARD_REQUIRED ON)

cmake_policy(SET CMP0091 NEW)
set_property(
	TARGET Spatial.Sandbox
	PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
)

add_resources_library(Sandbox.Assets
    FOLDER sandbox
    MATERIALS
        assets/materials/default.mat
    DEPENDEES
        Spatial.Sandbox
)