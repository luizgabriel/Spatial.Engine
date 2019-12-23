cmake_minimum_required(VERSION 3.15)
project(SpatialEngine
    LANGUAGES CXX
    VERSION 1.0.0
)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)
include(SetupDependencies)

option(BUILD_SPATIAL_SANDBOX "Build Spatial Sandbox target" ON)

add_subdirectory(Spatial.Core)

if (BUILD_SPATIAL_SANDBOX)
add_subdirectory(Spatial.Sandbox)
endif()

