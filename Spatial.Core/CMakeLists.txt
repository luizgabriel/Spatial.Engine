add_library(Spatial.Core

    ## HEADERS
    include/spatial/common/Configuration.h
    include/spatial/common/EventQueue.h
    include/spatial/common/EventQueueUtils.h
    include/spatial/common/Exceptions.h
    include/spatial/common/Key.h
    include/spatial/common/Signal.h
    include/spatial/common/StringHelpers.h

    include/spatial/core/Application.h
    include/spatial/core/ApplicationUtils.h
    include/spatial/core/Asset.h
    include/spatial/core/Clock.h
    include/spatial/core/ConfigurationLoader.h
    include/spatial/core/Logger.h
    include/spatial/core/System.h

    include/spatial/desktop/MessageBox.h
    include/spatial/desktop/PlatformEvent.h
    include/spatial/desktop/Window.h

    include/spatial/facilities/Initializer.h

    include/spatial/input/InputSystem.h
    include/spatial/input/Keyboard.h
    include/spatial/input/KeyboardState.h
    include/spatial/input/Mouse.h
    include/spatial/input/MouseState.h

    include/spatial/render/CameraControllers.h
    include/spatial/render/Engine.h
    include/spatial/render/EngineResource.h
    include/spatial/render/Entity.h
    include/spatial/render/RenderingSystem.h
    include/spatial/render/ResourceLoaders.h
    include/spatial/render/Resources.h
    include/spatial/render/SkyboxResources.h
    include/spatial/render/RegistryUtils.h
    include/spatial/render/Mesh.h

    include/spatial/ui/ImGuiHelpers.h
    include/spatial/ui/UserInterfaceInput.h
    include/spatial/ui/UserInterfaceRenderer.h
    include/spatial/ui/UserInterfaceSystem.h
    include/spatial/ui/ImGuiExtensions.h

    include/spatial/ecs/Components.h
    include/spatial/ecs/RenderableSystem.h
    include/spatial/ecs/TransformSystem.h

    include/spatial/init.h

    ## SOURCES
    src/common/Key.cpp
    src/common/StringHelpers.cpp
    src/common/Configuration.cpp
    src/common/Exceptions.cpp

    src/core/Application.cpp
    src/core/Asset.cpp
    src/core/ConfigurationLoader.cpp
    src/core/Logger.cpp

    src/desktop/Window.cpp
    src/desktop/WindowContext.cpp
    src/desktop/MessageBox.cpp

    src/facilities/Initializer.cpp

    src/input/InputSystem.cpp
    src/input/KeyboardState.cpp
    src/input/Keyboard.cpp
    src/input/Mouse.cpp
    src/input/MouseState.cpp

    src/render/Resources.cpp
    src/render/Engine.cpp
    src/render/RenderingSystem.cpp
    src/render/ResourceLoaders.cpp
    src/render/CameraControllers.cpp
    src/render/SkyboxResources.cpp
    src/render/Mesh.cpp

    src/ui/ImGuiHelpers.cpp
    src/ui/UserInterfaceSystem.cpp
    src/ui/UserInterfaceRenderer.cpp
    src/ui/UserInterfaceInput.cpp

    src/ecs/RenderableSystem.cpp
    src/ecs/TransformSystem.cpp
)
set_property(TARGET Spatial.Core PROPERTY CXX_STANDARD 20)
set_property(TARGET Spatial.Core PROPERTY CXX_STANDARD_REQUIRED ON)

cmake_policy(SET CMP0091 NEW)
set_property(
    TARGET Spatial.Core
    PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
)

target_include_directories(Spatial.Core
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include>
    PRIVATE
        src
)

# LINK PLATFORM INDEPENDENT DEPENDENCIES
target_link_libraries(Spatial.Core
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
    message(STATUS "Detected Linux Device")
    target_compile_definitions(Spatial.Core PRIVATE SPATIAL_PLATFORM_UNIX)

    find_package(Threads REQUIRED)
    target_link_libraries(Spatial.Core PRIVATE Threads::Threads)
    target_sources(Spatial.Core PRIVATE src/desktop/native/WindowHelperLinux.cpp)
elseif (WIN32)
    message(STATUS "Detected Windows Device")

    target_compile_definitions(Spatial.Core PUBLIC SPATIAL_PLATFORM_WINDOWS)

    target_link_libraries(Spatial.Core PRIVATE Shlwapi)
    target_sources(Spatial.Core PRIVATE src/desktop/native/WindowHelperWindows.cpp)
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
            "-framework ForceFeedback"
    )
    target_sources(Spatial.Core PRIVATE src/desktop/native/WindowHelperCocoa.mm)
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