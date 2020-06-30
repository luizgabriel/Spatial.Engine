cmake_minimum_required(VERSION 3.16)
project(Spatial
	LANGUAGES CXX
	VERSION 1.0.0)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/CMake)
include(SetupDependencies)
include(CMakePackageConfigHelpers)
include(CompilerWarnings)
include(Doxygen)
include(Sanitizers)
include(GraphViz)
include(StandardSettings)
include(StaticAnalyzers)

option(SPATIAL_EDITOR "Build Spatial Editor" ON)
option(SPATIAL_RESOURCE_COMPILER "Build Spatial Resource Compiler" ON)
option(SPATIAL_BUILD_TESTS "Build Spatial Resource Compiler" OFF)

add_subdirectory(Spatial.Core)

if (SPATIAL_EDITOR)
	add_subdirectory(Spatial.Editor)
endif ()

if (SPATIAL_RESOURCE_COMPILER)
	add_subdirectory(Spatial.ResourceCompiler)
endif()

install(EXPORT Spatial.Export NAMESPACE Spatial:: FILE SpatialEngineConfig.cmake DESTINATION .)
write_basic_package_version_file(SpatialEngineConfigVersion.cmake VERSION ${PACKAGE_VERSION} COMPATIBILITY AnyNewerVersion)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/SpatialEngineConfigVersion.cmake DESTINATION .)