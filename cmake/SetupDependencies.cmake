include(FetchContent)

set(FETCHCONTENT_QUIET OFF)

# This will also clean up the CMake ALL_BUILD, INSTALL, RUN_TESTS and ZERO_CHECK projects.
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

include(dependencies/glfw)
include(dependencies/mathfu)
include(dependencies/filament)
include(dependencies/fmt)

set(SPATIAL_CORE_DEPENDENCIES glfw filament fmt)
