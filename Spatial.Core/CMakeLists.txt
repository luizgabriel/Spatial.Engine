add_library(Spatial.Core

        ## HEADERS
        include/spatial/common/EventQueue.h
        include/spatial/common/Key.h
        include/spatial/common/Signal.h
        include/spatial/common/AxisAlignedBoundingBox.h
        include/spatial/common/AspectRatio.h

        include/spatial/core/Application.h
        include/spatial/core/ApplicationUtils.h
        include/spatial/core/Clock.h
        include/spatial/core/Logger.h

        include/spatial/desktop/PlatformEvent.h
        include/spatial/desktop/PlatformEventUtils.h
        include/spatial/desktop/Window.h

        include/spatial/input/InputSystem.h
        include/spatial/input/InputSystemUtils.h
        include/spatial/input/InputState.h

        include/spatial/ecs/Transform.h
        include/spatial/ecs/Camera.h
        include/spatial/ecs/Light.h
        include/spatial/ecs/Registry.h
        include/spatial/ecs/Name.h
        include/spatial/ecs/Mesh.h
        include/spatial/ecs/Builder.h
        include/spatial/ecs/Relation.h
        include/spatial/ecs/View.h
        include/spatial/ecs/Script.h
        include/spatial/ecs/Material.h
        include/spatial/ecs/Resource.h
        include/spatial/ecs/Texture.h

        include/spatial/serialization/Archives.h
        include/spatial/serialization/Math.h
        include/spatial/serialization/Camera.h
        include/spatial/serialization/Light.h
        include/spatial/serialization/Transform.h
        include/spatial/serialization/Mesh.h
        include/spatial/serialization/Name.h
        include/spatial/serialization/BoundingBox.h
        include/spatial/serialization/Registry.h
        include/spatial/serialization/Relation.h
        include/spatial/serialization/Resource.h
        include/spatial/serialization/Snapshot.h
        include/spatial/serialization/Texture.h
        include/spatial/serialization/Material.h

        include/spatial/resources/Math.h
        include/spatial/resources/BoundingBox.h
        include/spatial/resources/FilameshFile.h
        include/spatial/resources/ResourceError.h
        include/spatial/resources/FileSystem.h
        include/spatial/resources/PhysicalFileSystem.h
        include/spatial/resources/ResourceController.h
        include/spatial/resources/MeshComponents.h

        # include/spatial/script/Isolate.h
        # include/spatial/script/PlatformContext.h
        # include/spatial/script/ScriptController.h
        # include/spatial/script/ScriptSourceStream.h
        # include/spatial/script/Utils.h
        # include/spatial/script/Parsing.h

        ## SOURCES
        src/core/Application.cpp
        src/core/Logger.cpp

        src/desktop/Window.cpp
        src/desktop/WindowContext.cpp

        src/input/InputSystem.cpp
        src/input/InputState.cpp

        src/ecs/Transform.cpp
        src/ecs/Registry.cpp
        src/ecs/EntityBuilder.cpp
        src/ecs/Relation.cpp
        src/ecs/Mesh.cpp
        src/ecs/Script.cpp
        src/ecs/Resource.cpp
        src/ecs/Material.cpp

        # src/script/PlatformContext.cpp
        # src/script/Isolate.cpp
        # src/script/ScriptController.cpp
        # src/script/ScriptSourceStream.cpp
        # src/script/Utils.cpp
        # src/script/Parsing.cpp

        src/resources/FilameshFile.cpp
        src/resources/FileSystem.cpp
        src/resources/PhysicalFileSystem.cpp
        src/resources/MemoryFileSystem.cpp
        src/resources/ResourceController.cpp
        src/resources/MeshComponents.cpp)

set_project_warnings(Spatial.Core)
enable_sanitizers(Spatial.Core)

target_precompile_headers(Spatial.Core
        PRIVATE <memory> <vector> <unordered_map> <unordered_set> <future> <optional> <filesystem> <chrono> <utility> <type_traits> <set> <random> <array> <functional>)

target_include_directories(Spatial.Core
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include>
        PRIVATE src)

find_package(boost REQUIRED)
find_package(EnTT REQUIRED)
find_package(spdlog REQUIRED)
find_package(stb REQUIRED)
find_package(glfw3 REQUIRED)
find_package(cereal REQUIRED)
find_package(glm REQUIRED)
find_package(magic_enum REQUIRED)

# LINK PLATFORM INDEPENDENT DEPENDENCIES
target_link_libraries(Spatial.Core
        PUBLIC
        boost::boost
        EnTT::EnTT
        spdlog::spdlog
        stb::stb
        glfw
        cereal::cereal
        glm::glm
        magic_enum::magic_enum)

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
else ()
    message(FATAL_ERROR "Unsupported platform")
endif ()

# ADD DEBUG/RELEASE FLAGS
target_compile_definitions(Spatial.Core PUBLIC "$<$<CONFIG:DEBUG>:SPATIAL_DEBUG>")

install(
        TARGETS Spatial.Core
        EXPORT Spatial.Export
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        COMPONENT libraries
)

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} COMPONENT headers)

add_subdirectory(tests)
