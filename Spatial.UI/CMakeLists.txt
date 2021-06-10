add_library(Spatial.UI
	
	## HEADERS
	include/spatial/ui/system/ImGuiHelpers.h
	include/spatial/ui/system/UserInterfaceSystem.h
	include/spatial/ui/system/UserInterfaceRenderer.h
	include/spatial/ui/system/UserInterfaceInput.h
	
	include/spatial/ui/components/Window.h
	include/spatial/ui/components/PropertiesPanel.h
	include/spatial/ui/components/Collapse.h
	include/spatial/ui/components/DockSpace.h
	include/spatial/ui/components/Components.h
	include/spatial/ui/components/ListPanel.h
	
	include/spatial/ui/components/styles/BorderlessWindowStyle.h
	include/spatial/ui/components/styles/NoPaddingWindow.h
	
	## SOURCES
	src/system/ImGuiHelpers.cpp
	src/system/UserInterfaceSystem.cpp
	src/system/UserInterfaceRenderer.cpp
	src/system/UserInterfaceInput.cpp
	
	src/components/Window.cpp
	src/components/PropertiesPanel.cpp
	src/components/Collapse.cpp
	src/components/DockSpace.cpp
	src/components/ListPanel.cpp
	src/components/Components.cpp
	
	src/components/styles/BorderlessWindowStyle.cpp
	src/components/styles/NoPaddingWindow.cpp)

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