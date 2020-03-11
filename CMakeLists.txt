cmake_minimum_required(VERSION 3.15)
project(SpatialEngine
    LANGUAGES CXX
    VERSION 1.0.0
)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/CMake)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/CMake/Modules)
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

install(DIRECTORY ${CMAKE_BINARY_DIR}/tools DESTINATION bin)

install(
	DIRECTORY
		CMake/Modules/
	DESTINATION
		cmake/modules
)

install(
    EXPORT Spatial.Export
    NAMESPACE Spatial::
    FILE SpatialEngineConfig.cmake
    DESTINATION cmake
)

write_basic_package_version_file(
    SpatialEngineConfigVersion.cmake
    VERSION ${PACKAGE_VERSION}
    COMPATIBILITY AnyNewerVersion
)

install(
    FILES
        ${CMAKE_CURRENT_BINARY_DIR}/SpatialEngineConfigVersion.cmake
    DESTINATION
        cmake
)