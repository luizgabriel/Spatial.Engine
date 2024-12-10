cmake_minimum_required(VERSION 3.25)
project(SpatialEngine LANGUAGES CXX OBJCXX VERSION 1.3.21)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/CMake)

include(GNUInstallDirs)
include(CompilerWarnings)
include(Doxygen)
include(Sanitizers)
include(StandardSettings)
include(OptimizeSettings)
include(CMakePackageConfigHelpers)
include(PackageSettings)
include(GoogleTest)

set_property(GLOBAL PROPERTY CTEST_TARGETS_ADDED 1)
include(CTest)
find_package(gtest REQUIRED)
enable_testing()

add_subdirectory(Spatial.Core)
add_subdirectory(Spatial.Graphics)
add_subdirectory(Spatial.UI)
add_subdirectory(Spatial.Res)
add_subdirectory(Spatial.Editor)
add_subdirectory(Spatial.Game)

install(EXPORT Spatial.Export
        NAMESPACE Spatial::
        FILE SpatialEngineConfig.cmake
        DESTINATION .
        COMPONENT configs)
write_basic_package_version_file(SpatialEngineConfigVersion.cmake VERSION ${PACKAGE_VERSION} COMPATIBILITY AnyNewerVersion)
install(FILES
            LICENSE.txt
            ${CMAKE_CURRENT_BINARY_DIR}/SpatialEngineConfigVersion.cmake
        DESTINATION .
        COMPONENT configs)