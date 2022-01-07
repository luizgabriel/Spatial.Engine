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
        include/spatial/desktop/InputSystemUtils.h
        include/spatial/desktop/InputState.h

        include/spatial/ecs/Transform.h
        include/spatial/ecs/Camera.h
        include/spatial/ecs/Light.h
        include/spatial/ecs/Registry.h
        include/spatial/ecs/RegistryUtils.h
        include/spatial/ecs/EntityHandle.h
        include/spatial/ecs/Name.h
        include/spatial/ecs/Mesh.h
        include/spatial/ecs/Tags.h
        include/spatial/ecs/EntityBuilder.h
        include/spatial/ecs/Relation.h
        include/spatial/ecs/UpdateController.h
        include/spatial/ecs/SceneView.h
        include/spatial/ecs/Script.h

        include/spatial/render/Engine.h
        include/spatial/render/EngineResource.h
        include/spatial/render/Entity.h
        include/spatial/render/RenderingSystem.h
        include/spatial/render/Resources.h
        include/spatial/render/SkyboxResources.h
        include/spatial/resources/FilameshFile.h
        include/spatial/render/Light.h
        include/spatial/render/Transform.h
        include/spatial/render/Renderable.h
        include/spatial/render/SceneController.h
        include/spatial/render/TransformController.h
        include/spatial/render/LightController.h
        include/spatial/render/CameraController.h
        include/spatial/render/MeshController.h
        include/spatial/render/TextureView.h
        include/spatial/render/IndirectLightController.h
        include/spatial/render/ResourceFinders.h

        include/spatial/serialization/Archives.h
        include/spatial/serialization/Math.h
        include/spatial/serialization/Camera.h
        include/spatial/serialization/Light.h
        include/spatial/serialization/Transform.h
        include/spatial/serialization/Mesh.h
        include/spatial/serialization/Name.h
        include/spatial/serialization/BoundingBox.h
        include/spatial/serialization/Registry.h
        include/spatial/serialization/FileSystem.h
        include/spatial/serialization/Relation.h
        include/spatial/serialization/SkyBox.h

        include/spatial/resources/Math.h
        include/spatial/resources/BoundingBox.h
        include/spatial/resources/FilameshFile.h
        include/spatial/resources/ResourceLoader.h
        include/spatial/resources/ResourceFinder.h
        include/spatial/resources/ResourceError.h
        include/spatial/resources/Resource.h

        #include/spatial/script/Isolate.h
        #include/spatial/script/PlatformContext.h

        ## SOURCES
        src/common/StringHelpers.cpp

        src/core/Application.cpp
        src/core/Logger.cpp

        src/desktop/Window.cpp
        src/desktop/WindowContext.cpp
        src/desktop/InputSystem.cpp
        src/desktop/InputState.cpp

        src/ecs/EditorCamera.cpp
        src/ecs/Registry.cpp
        src/ecs/RegistryUtils.cpp
        src/ecs/EntityBuilder.cpp
        src/ecs/Relation.cpp

        src/render/Resources.cpp
        src/render/Engine.cpp
        src/render/Entity.cpp
        src/render/RenderingSystem.cpp
        src/render/SkyboxResources.cpp
        src/render/Camera.cpp
        src/render/Transform.cpp
        src/render/Light.cpp
        src/render/Renderable.cpp
        src/render/TransformController.cpp
        src/render/LightController.cpp
        src/render/CameraController.cpp
        src/render/MeshController.cpp
        src/render/SceneController.cpp
        src/render/TextureView.cpp
        src/render/IndirectLightController.cpp

        #src/script/PlatformContext.cpp
        #src/script/Isolate.cpp

        src/resources/BoundingBox.cpp
        src/resources/FilameshFile.cpp
        src/resources/ResourceLoader.cpp src/ecs/Mesh.cpp src/render/MaterialController.cpp)

set_project_warnings(Spatial.Core)
enable_sanitizers(Spatial.Core)

cmake_policy(SET CMP0091 NEW)
set_property(
        TARGET Spatial.Core
        PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

target_precompile_headers(Spatial.Core
        PRIVATE <memory> <vector> <unordered_map> <unordered_set> <future> <optional> <filesystem> <chrono> <utility> <type_traits> <set> <random> <array> <functional>)

target_include_directories(Spatial.Core
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include>
        PRIVATE src)

# LINK PLATFORM INDEPENDENT DEPENDENCIES
target_link_libraries(Spatial.Core
        PUBLIC
        CONAN_PKG::boost
        CONAN_PKG::entt
        CONAN_PKG::filament
        CONAN_PKG::spdlog
        CONAN_PKG::stb
        CONAN_PKG::glfw
        CONAN_PKG::cereal
        CONAN_PKG::magic_enum
        #CONAN_PKG::v8
        CONAN_PKG::tl-expected)

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