add_library(Spatial.Core
	src/common/KeyAction.cpp
	src/desktop/Window.cpp
	src/desktop/WindowContext.cpp
	src/render/RenderingSubsystem.cpp
	src/render/RenderPipeline.cpp
	src/input/InputSubsystem.cpp
	src/Application.cpp
)
set_property(TARGET Spatial.Core PROPERTY CXX_STANDARD 17)
set_property(TARGET Spatial.Core PROPERTY CXX_STANDARD_REQUIRED ON)

cmake_policy(SET CMP0091 NEW)
set_property(
	TARGET Spatial.Core
	PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
)

target_include_directories(
	Spatial.Core
	PUBLIC
		include
	PRIVATE
		src		
)

target_link_libraries(
	Spatial.Core
	PUBLIC
		fmt::fmt
		entt::entt
		sdl2::sdl2
		filament::filament
)

find_package(OpenGL REQUIRED)
target_link_libraries(Spatial.Core PRIVATE OpenGL::GL)

if (UNIX AND NOT APPLE)
	target_compile_definitions(Spatial.Core PRIVATE SPATIAL_PLATFORM_UNIX)

	find_package(Threads REQUIRED)
	target_link_libraries(Spatial.Core PRIVATE Threads::Threads)
elseif(WIN32)
	target_compile_definitions(Spatial.Core PRIVATE SPATIAL_PLATFORM_WINDOWS)

	
elseif(APPLE)
	target_compile_definitions(Spatial.Core PRIVATE SPATIAL_PLATFORM_OSX)
	
	target_link_libraries(Spatial.Core PRIVATE "-framework Cocoa" "-framework Metal" "-framework CoreVideo")
endif()

