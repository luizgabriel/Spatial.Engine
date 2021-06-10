add_library(Spatial.UI
	
	## HEADERS
	include/spatial/ui/system/ImGuiHelpers.h
	include/spatial/ui/system/UserInterfaceSystem.h
	include/spatial/ui/system/UserInterfaceRenderer.h
	include/spatial/ui/system/UserInterfaceInput.h
	
	include/spatial/ui/components/ImGuiWindow.h
	include/spatial/ui/components/ImGuiPropertiesPanel.h
	include/spatial/ui/components/ImGuiCollapse.h
	include/spatial/ui/components/ImGuiSceneWindow.h
	include/spatial/ui/components/ImGuiDockSpace.h
	include/spatial/ui/components/ImGuiComponents.h
	include/spatial/ui/components/ImGuiListPanel.h
	
	## SOURCES
	src/system/ImGuiHelpers.cpp
	src/system/UserInterfaceSystem.cpp
	src/system/UserInterfaceRenderer.cpp
	src/system/UserInterfaceInput.cpp
	
	src/components/ImGuiWindow.cpp
	src/components/ImGuiPropertiesPanel.cpp
	src/components/ImGuiCollapse.cpp
	src/components/ImGuiSceneWindow.cpp
	src/components/ImGuiDockSpace.cpp
	src/components/ImGuiListPanel.cpp
	src/components/ImGuiComponents.cpp)

set_project_warnings(Spatial.UI)
enable_sanitizers(Spatial.UI)

cmake_policy(SET CMP0091 NEW)
set_property(
	TARGET Spatial.UI
	PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

target_include_directories(Spatial.UI
	PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include>
	PRIVATE src)

# LINK PLATFORM INDEPENDENT DEPENDENCIES
target_link_libraries(Spatial.UI
	PUBLIC
	Spatial.Core
	CONAN_PKG::imgui)

install(
	TARGETS Spatial.UI
	EXPORT Spatial.Export
	LIBRARY DESTINATION lib
	PUBLIC_HEADER DESTINATION include
)

install(DIRECTORY include/ DESTINATION include)