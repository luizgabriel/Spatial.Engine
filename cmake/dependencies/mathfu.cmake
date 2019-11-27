FetchContent_Declare(
    mathfu
    GIT_REPOSITORY https://github.com/google/mathfu
    GIT_TAG v1.1.0
)

FetchContent_GetProperties(mathfu)
if(NOT mathfu_POPULATED)
    FetchContent_Populate(mathfu)

    set(mathfu_enable_simd ON CACHE INTERNAL)
    set(mathfu_build_benchmarks OFF CACHE INTERNAL)
    set(mathfu_build_tests OFF CACHE INTERNAL)

    add_subdirectory(${mathfu_SOURCE_DIR} ${mathfu_BINARY_DIR})
endif()