add_library(Spatial.Graphics

        ## HEADERS
        include/spatial/graphics/Engine.h
        include/spatial/graphics/EngineResource.h
        include/spatial/graphics/Entity.h
        include/spatial/graphics/RenderingSystem.h
        include/spatial/graphics/Resources.h
        include/spatial/graphics/SkyboxResources.h
        include/spatial/graphics/Light.h
        include/spatial/graphics/Transform.h
        include/spatial/graphics/Renderable.h
        include/spatial/graphics/SceneController.h
        include/spatial/graphics/TransformController.h
        include/spatial/graphics/LightController.h
        include/spatial/graphics/CameraController.h
        include/spatial/graphics/MeshController.h
        include/spatial/graphics/RegistryRenderingSystem.h
        include/spatial/graphics/MaterialInstance.h
        include/spatial/graphics/TextureController.h
        include/spatial/graphics/TextureUtils.h
        include/spatial/graphics/MathConversionUtils.h
        include/spatial/graphics/TextureResources.h
        include/spatial/graphics/MeshResources.h
        include/spatial/graphics/native/CocoaHelper.h

        ## SOURCES
        src/Resources.cpp
        src/Engine.cpp
        src/Entity.cpp
        src/RenderingSystem.cpp
        src/SkyboxResources.cpp
        src/Camera.cpp
        src/Transform.cpp
        src/Light.cpp
        src/Renderable.cpp
        src/TransformController.cpp
        src/LightController.cpp
        src/CameraController.cpp
        src/MeshController.cpp
        src/SceneController.cpp
        src/MaterialController.cpp
        src/SceneRenderingSystem.cpp
        src/MaterialInstance.cpp
        src/TextureController.cpp
        src/TextureUtils.cpp
        src/TextureResources.cpp
        src/MeshResources.cpp)

set_project_warnings(Spatial.Graphics)
enable_sanitizers(Spatial.Graphics)

target_precompile_headers(Spatial.Graphics
        PRIVATE <memory> <sstream>)

target_include_directories(Spatial.Graphics
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include>
        PRIVATE src)

find_package(filament REQUIRED)

target_link_libraries(Spatial.Graphics
        PUBLIC
        Spatial.Core
        filament::filament)

find_package(OpenGL)
if (OpenGL_FOUND)
    target_link_libraries(Spatial.Graphics PRIVATE OpenGL::GL)
endif ()

option(SPATIAL_GRAPHICS_BACKEND "The graphics backend to use (opengl, metal or vulkan)")

if (NOT SPATIAL_GRAPHICS_BACKEND)
    if (APPLE)
        set(DEFAULT_BACKEND "metal")
    else ()
        set(DEFAULT_BACKEND "opengl")
    endif ()

    message(STATUS "No graphics backend specified, using default: ${DEFAULT_BACKEND}")
    set(SPATIAL_GRAPHICS_BACKEND ${DEFAULT_BACKEND})

else ()
    message(STATUS "Selected graphics backend: ${SPATIAL_GRAPHICS_BACKEND}")
endif ()

string(TOUPPER ${SPATIAL_GRAPHICS_BACKEND} SPATIAL_GRAPHICS_BACKEND_UPPER)
target_compile_definitions(Spatial.Graphics PUBLIC SPATIAL_GRAPHICS_BACKEND_${SPATIAL_GRAPHICS_BACKEND_UPPER})

# ADD PLATFORM FLAGS AND PLATFORM SPECIFIC DEPENDENCIES
if (UNIX AND NOT APPLE)
    message(STATUS "Detected Linux Device")
    target_compile_definitions(Spatial.Graphics PRIVATE SPATIAL_PLATFORM_UNIX)

    find_package(Threads REQUIRED)
    target_link_libraries(Spatial.Graphics PRIVATE Threads::Threads)
elseif (WIN32)
    message(STATUS "Detected Windows Device")

    target_compile_definitions(Spatial.Graphics PUBLIC SPATIAL_PLATFORM_WINDOWS)

    target_link_libraries(Spatial.Graphics PRIVATE Shlwapi)
elseif (APPLE)
    message(STATUS "Detected Apple Device")
    target_compile_definitions(Spatial.Graphics PUBLIC SPATIAL_PLATFORM_OSX)

    target_sources(Spatial.Graphics PRIVATE src/CocoaHelper.mm)

    target_link_libraries(Spatial.Graphics PRIVATE
            "-framework Cocoa"
            "-framework Metal"
            "-framework QuartzCore"
            "-framework CoreVideo"
            "-framework Carbon"
            "-framework IOKit"
            "-framework CoreAudio"
            "-framework AudioToolbox"
            "-framework ForceFeedback")
else ()
    message(FATAL_ERROR "Unsupported platform")
endif ()

install(
        TARGETS Spatial.Graphics
        EXPORT Spatial.Export
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        COMPONENT libraries
)

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} COMPONENT headers)
