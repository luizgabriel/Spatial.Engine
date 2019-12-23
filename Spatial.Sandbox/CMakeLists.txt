add_executable(Spatial.Sandbox
    src/main.cpp
)

target_link_libraries(Spatial.Sandbox PUBLIC Spatial.Core)

cmake_policy(SET CMP0091 NEW)
set_property(
	TARGET Spatial.Sandbox
	PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
)