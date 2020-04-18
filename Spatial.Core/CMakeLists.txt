add_library(Spatial.Core
	src/common/Key.cpp

	src/core/Application.cpp
	src/core/Asset.cpp
	src/core/Logger.cpp
	src/core/Clock.cpp

	src/desktop/Window.cpp
	src/desktop/WindowContext.cpp

	src/input/InputSystem.cpp
	src/input/InputState.cpp
	src/input/Input.cpp

	src/render/CommonResources.cpp
	src/render/Engine.cpp
	src/render/RenderingSystem.cpp
	src/render/ResourceLoaders.cpp

	src/ui/ImGuiHelpers.cpp
	src/ui/UserInterfaceSystem.cpp
	src/ui/UserInterfaceRenderer.cpp
	src/ui/UserInterfaceInput.cpp
)
set_property(TARGET Spatial.Core PROPERTY CXX_STANDARD 17)
set_property(TARGET Spatial.Core PROPERTY CXX_STANDARD_REQUIRED ON)

cmake_policy(SET CMP0091 NEW)
set_property(
	TARGET Spatial.Core
	PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
)


# LINK PLATFORM INDEPENDENT DEPENDENCIES
target_include_directories(
	Spatial.Core
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include>
	PRIVATE
		src		
)

target_link_libraries(
	Spatial.Core
	PUBLIC
		CONAN_PKG::fmt
		CONAN_PKG::entt
		CONAN_PKG::filament
		CONAN_PKG::spdlog
		CONAN_PKG::imgui
		CONAN_PKG::boost_tti
		CONAN_PKG::stb
		CONAN_PKG::sdl2
		CONAN_PKG::toml++
)

find_package(OpenGL REQUIRED)
target_link_libraries(Spatial.Core PRIVATE OpenGL::GL)


# ADD PLATFORM FLAGS AND PLATFORM SPECIFIC DEPENDENCIES
if (UNIX AND NOT APPLE)
	target_compile_definitions(Spatial.Core PRIVATE SPATIAL_PLATFORM_UNIX)

	find_package(Threads REQUIRED)
	target_link_libraries(Spatial.Core PRIVATE Threads::Threads)
elseif(WIN32)
	target_compile_definitions(Spatial.Core PRIVATE SPATIAL_PLATFORM_WINDOWS)

	target_link_libraries(Spatial.Core PRIVATE Shlwapi)
elseif(APPLE)
	target_compile_definitions(Spatial.Core PRIVATE SPATIAL_PLATFORM_OSX)
	
	target_link_libraries(Spatial.Core PRIVATE "-framework Cocoa" "-framework Metal" "-framework CoreVideo")
endif()


# ADD DEBUG/RELEASE FLAGS
target_compile_definitions(Spatial.Core PUBLIC "$<$<CONFIG:DEBUG>:SPATIAL_DEBUG>")


install(
	TARGETS Spatial.Core
	EXPORT Spatial.Export
	LIBRARY DESTINATION lib
)

install(DIRECTORY include/ DESTINATION include)