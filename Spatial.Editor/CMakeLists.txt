add_executable(Spatial.Editor
	src/main.cpp
	src/EditorSystem.cpp)
set_project_warnings(Spatial.Editor)
enable_sanitizers(Spatial.Editor)
target_precompile_headers(Spatial.Editor REUSE_FROM Spatial.Core)
target_link_libraries(Spatial.Editor PUBLIC Spatial.Core)

cmake_policy(SET CMP0091 NEW)
set_property(
	TARGET Spatial.Editor
	PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

add_custom_target(Spatial.Editor.Resources
	COMMAND "${SPATIAL_RESOURCE_COMPILER} --format=embed ${CMAKE_CURRENT_SOURCE_DIR}/assets.json ${CMAKE_CURRENT_BINARY_DIR}/generated-resources/"
	DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/assets.json
	MESSAGE "Compiling Spatial.Editor resource pack")

install(TARGETS Spatial.Editor RUNTIME DESTINATION bin)