cmake_minimum_required(VERSION 3.15)
project(SpatialEngine
    LANGUAGES CXX
    VERSION 1.0.0
)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/CMake)
include(SetupDependencies)
include(CMakePackageConfigHelpers)

option(SPATIAL_SANDBOX "Build Spatial Sandbox target" ON)
option(SPATIAL_DEPENDENCY_GRAPH "Generates a depedency graph file. ('.dot' graphviz)" OFF)

add_subdirectory(Spatial.Core)

if (SPATIAL_SANDBOX)
    add_subdirectory(Spatial.Sandbox)
endif()

if (SPATIAL_DEPENDENCY_GRAPH)
    find_program(DOT_PROGRAM dot)

    if (DOT_PROGRAM)
    add_custom_target(Spatial.GraphViz
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/graphviz
        COMMAND ${CMAKE_COMMAND} "--graphviz=graphviz/spatial.dot" .
        COMMAND ${DOT_PROGRAM} -Tpng spatial.dot -o graphviz/dependency_graph.png
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Generating graphviz file: ${CMAKE_BINARY_DIR}/spatial.dot"
    )
    else()
    message(WARNING "Could not find graphviz dot executable")
    endif()
endif()

install(DIRECTORY ${CMAKE_BINARY_DIR}/tools DESTINATION /)

install(FILES CMake/ResourcesLibrary.cmake DESTINATION cmake)

install(
	FILES
		CMake/ResourceTypes/CopyResource.cmake
		CMake/ResourceTypes/MaterialResource.cmake
		CMake/ResourceTypes/MeshResource.cmake
		CMake/ResourceTypes/SkyboxResource.cmake
	DESTINATION
		cmake/ResourceTypes
)

install(EXPORT
    SpatialEngine
    NAMESPACE Spatial::
    DESTINATION /
)

write_basic_package_version_file(
    SpatialEngineConfigVersion.cmake
    VERSION ${CMAKE_PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(
    FILES
        ${CMAKE_CURRENT_BINARY_DIR}/SpatialEngineConfig.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/SpatialEngineConfigVersion.cmake
    DESTINATION
        ${CMAKE_INSTALL_PREFIX}/cmake
)