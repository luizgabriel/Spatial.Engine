cmake_minimum_required(VERSION 3.15)
project(SpatialEngine
    LANGUAGES CXX
    VERSION 1.0.0
)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/CMake)
include(SetupDependencies)

option(BUILD_SPATIAL_SANDBOX "Build Spatial Sandbox target" ON)
option(GENERATE_DEPENDENCY_GRAPH "Generates a depedency graph file. ('.dot' graphviz)" ON)


add_subdirectory(Spatial.Core)

if (BUILD_SPATIAL_SANDBOX)
    add_subdirectory(Spatial.Sandbox)
endif()

if (GENERATE_DEPENDENCY_GRAPH)
    add_custom_target(Spatial.GraphViz ALL
        COMMAND ${CMAKE_COMMAND} "--graphviz=spatial.dot" .
        COMMAND dot -Tpng spatial.dot -o foo.png
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Generating graphviz file: ${CMAKE_BINARY_DIR}/spatial.dot"
    )
endif()