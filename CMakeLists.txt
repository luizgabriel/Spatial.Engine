cmake_minimum_required(VERSION 3.15)
project(SpatialEngine VERSION 1.0.0)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
include(SetupDependencies)

add_subdirectory(Spatial.Core)
add_subdirectory(Spatial.Sandbox)

