add_subdirectory(assets)

add_executable(Spatial.Editor
	src/main.cpp
	src/SceneEditorSystem.cpp
	src/EditorCameraScript.h
	src/EditorCameraScript.cpp
	src/EditorCamera.h
	src/DefaultMaterial.h
	src/DefaultMaterial.cpp
	src/ImGuiComponents.h
	src/ImGuiComponents.cpp
	src/ImGuiSceneWindow.h
	src/ImGuiSceneWindow.cpp
	src/ImGuiDockSpace.h
	src/ImGuiDockSpace.cpp
	src/ImGuiWindow.h
	src/ImGuiWindow.cpp
	src/Serialization.h
)
set_project_warnings(Spatial.Editor)
enable_sanitizers(Spatial.Editor)
target_precompile_headers(Spatial.Editor REUSE_FROM Spatial.Core)
target_link_libraries(Spatial.Editor PUBLIC
	Spatial.Core
	CONAN_PKG::fmt
	CONAN_PKG::argh
	CONAN_PKG::ghc-filesystem)

target_link_libraries(Spatial.Editor PRIVATE Spatial.Editor.Resources)

cmake_policy(SET CMP0091 NEW)
set_property(
	TARGET Spatial.Editor
	PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

install(TARGETS Spatial.Editor RUNTIME DESTINATION bin)

