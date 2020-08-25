add_subdirectory(assets)

add_executable(Spatial.Editor
	src/main.cpp
	src/EditorSystem.cpp
	src/Resources.cpp
	src/Components.h
)
set_project_warnings(Spatial.Editor)
enable_sanitizers(Spatial.Editor)
target_precompile_headers(Spatial.Editor REUSE_FROM Spatial.Core)
target_link_libraries(Spatial.Editor PUBLIC
	Spatial.Core
	CONAN_PKG::fmt
	CONAN_PKG::argh)

target_link_libraries(Spatial.Editor PRIVATE Spatial.Editor.Resources)

cmake_policy(SET CMP0091 NEW)
set_property(
	TARGET Spatial.Editor
	PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

install(TARGETS Spatial.Editor RUNTIME DESTINATION bin)

