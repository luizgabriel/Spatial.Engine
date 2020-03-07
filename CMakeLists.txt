cmake_minimum_required(VERSION 3.15)
project(SpatialEngine
    LANGUAGES CXX
    VERSION 1.0.0
)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/CMake)
include(SetupDependencies)

option(BUILD_SPATIAL_SANDBOX "Build Spatial Sandbox target" ON)
option(GENERATE_DEPENDENCY_GRAPH "Generates a depedency graph file. ('.dot' graphviz)" OFF)


add_subdirectory(Spatial.Core)

if (BUILD_SPATIAL_SANDBOX)
    add_subdirectory(Spatial.Sandbox)
endif()

if (GENERATE_DEPENDENCY_GRAPH)
    find_program(DOT_PROGRAM dot)

    if (DOT_PROGRAM)
    add_custom_target(Spatial.GraphViz ALL
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