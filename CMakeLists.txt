cmake_minimum_required(VERSION 3.22)
project(Spatial LANGUAGES CXX VERSION 1.3.7)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/CMake)
include(SetupConan)
include(CMakePackageConfigHelpers)
include(CompilerWarnings)
include(Doxygen)
include(Sanitizers)
include(StandardSettings)
include(StaticAnalyzers)

option(SPATIAL_BUILD_TESTS "Build Spatial Resource Compiler" OFF)

add_subdirectory(Spatial.Core)
add_subdirectory(Spatial.Res)
add_subdirectory(Spatial.UI)
add_subdirectory(Spatial.Editor)
add_subdirectory(Spatial.Game)

install(EXPORT Spatial.Export NAMESPACE Spatial:: FILE SpatialEngineConfig.cmake DESTINATION .)
write_basic_package_version_file(SpatialEngineConfigVersion.cmake VERSION ${PACKAGE_VERSION} COMPATIBILITY AnyNewerVersion)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/SpatialEngineConfigVersion.cmake DESTINATION .)