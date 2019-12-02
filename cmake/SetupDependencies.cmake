include(FetchContent)

set(FETCHCONTENT_QUIET OFF)

# This will also clean up the CMake ALL_BUILD, INSTALL, RUN_TESTS and ZERO_CHECK projects.
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# TODO: Move all dependencies to ExternalProject format

include(dependencies/glfw)
include(dependencies/mathfu)
#include(dependencies/filament)
include(dependencies/fmt)
include(dependencies/entt)

# OpenGL
find_package(OpenGL REQUIRED)

set(SPATIAL_CORE_DEPENDENCIES OpenGL::GL glfw fmt EnTT)
