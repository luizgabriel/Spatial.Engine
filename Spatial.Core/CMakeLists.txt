add_library(Spatial.Core
	
	## HEADERS
	include/spatial/common/EventQueue.h
	include/spatial/common/Key.h
	include/spatial/common/Signal.h
	include/spatial/common/StringHelpers.h
	
	include/spatial/core/Application.h
	include/spatial/core/ApplicationUtils.h
	include/spatial/core/Clock.h
	include/spatial/core/Logger.h
	
	include/spatial/desktop/PlatformEvent.h
	include/spatial/desktop/PlatformEventUtils.h
	include/spatial/desktop/Window.h
	include/spatial/desktop/InputSystem.h
	include/spatial/desktop/InputState.h
	
	include/spatial/render/Engine.h
	include/spatial/render/EngineResource.h
	include/spatial/render/Entity.h
	include/spatial/render/RenderingSystem.h
	include/spatial/render/ResourceLoaders.h
	include/spatial/render/Resources.h
	include/spatial/render/SkyboxResources.h
	include/spatial/render/FilameshFile.h
	include/spatial/render/Light.h
	include/spatial/render/Stage.h
	include/spatial/render/InstanceHandle.h
	include/spatial/render/InstanceBuilder.h
	include/spatial/render/Transform.h
	include/spatial/render/SceneNodeName.h
	include/spatial/render/Renderable.h
	
	include/spatial/ui/ImGuiHelpers.h
	include/spatial/ui/UserInterfaceInput.h
	include/spatial/ui/UserInterfaceRenderer.h
	include/spatial/ui/UserInterfaceSystem.h
	include/spatial/ui/UserInterfaceUtils.h
	
	## SOURCES
	src/common/Key.cpp
	src/common/StringHelpers.cpp
	
	src/core/Application.cpp
	src/core/Logger.cpp
	
	src/desktop/Window.cpp
	src/desktop/WindowContext.cpp
	src/desktop/InputSystem.cpp
	src/desktop/InputState.cpp
	
	src/render/Stage.cpp
	src/render/Resources.cpp
	src/render/Engine.cpp
	src/render/Entity.cpp
	src/render/RenderingSystem.cpp
	src/render/ResourceLoaders.cpp
	src/render/SkyboxResources.cpp
	src/render/Camera.cpp
	src/render/InstanceBuilder.cpp
	src/render/Transform.cpp
	src/render/Light.cpp
	src/render/Renderable.cpp
	
	src/ui/ImGuiHelpers.cpp
	src/ui/UserInterfaceSystem.cpp
	src/ui/UserInterfaceRenderer.cpp
	src/ui/UserInterfaceInput.cpp)

set_project_warnings(Spatial.Core)
enable_sanitizers(Spatial.Core)

cmake_policy(SET CMP0091 NEW)
set_property(
	TARGET Spatial.Core
	PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

target_precompile_headers(Spatial.Core
	PRIVATE <memory> <vector> <unordered_map> <unordered_set> <future> <optional> <filesystem> <chrono> <utility>)

target_include_directories(Spatial.Core
	PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include>
	PRIVATE src)

# LINK PLATFORM INDEPENDENT DEPENDENCIES
target_link_libraries(Spatial.Core
	PUBLIC
	CONAN_PKG::entt
	CONAN_PKG::filament
	CONAN_PKG::spdlog
	CONAN_PKG::imgui
	CONAN_PKG::boost
	CONAN_PKG::stb
	CONAN_PKG::glfw)

find_package(OpenGL REQUIRED)
target_link_libraries(Spatial.Core PRIVATE OpenGL::GL)

# ADD PLATFORM FLAGS AND PLATFORM SPECIFIC DEPENDENCIES
if (UNIX AND NOT APPLE)
	message(STATUS "Detected Linux Device")
	target_compile_definitions(Spatial.Core PRIVATE SPATIAL_PLATFORM_UNIX)
	
	find_package(Threads REQUIRED)
	target_link_libraries(Spatial.Core PRIVATE Threads::Threads)
elseif (WIN32)
	message(STATUS "Detected Windows Device")
	
	target_compile_definitions(Spatial.Core PUBLIC SPATIAL_PLATFORM_WINDOWS)
	
	target_link_libraries(Spatial.Core PRIVATE Shlwapi)
elseif (APPLE)
	message(STATUS "Detected Apple Device")
	target_compile_definitions(Spatial.Core PUBLIC SPATIAL_PLATFORM_OSX)
	
	target_link_libraries(Spatial.Core PRIVATE
		"-framework Cocoa"
		"-framework Metal"
		"-framework CoreVideo"
		"-framework Carbon"
		"-framework IOKit"
		"-framework CoreAudio"
		"-framework AudioToolbox"
		"-framework ForceFeedback")
	target_sources(Spatial.Core PRIVATE src/desktop/native/WindowHelperCocoa.mm)
	set_source_files_properties(src/desktop/native/WindowHelperCocoa.mm PROPERTIES SKIP_PRECOMPILE_HEADERS ON)
else ()
	message(FATAL_ERROR "Unsupported platform")
endif ()

# ADD DEBUG/RELEASE FLAGS
target_compile_definitions(Spatial.Core PUBLIC "$<$<CONFIG:DEBUG>:SPATIAL_DEBUG>")

install(
	TARGETS Spatial.Core
	EXPORT Spatial.Export
	LIBRARY DESTINATION lib
	PUBLIC_HEADER DESTINATION include
)

install(DIRECTORY include/ DESTINATION include)

if (SPATIAL_BUILD_TESTS)
	add_subdirectory(tests)
endif ()