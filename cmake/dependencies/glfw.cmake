FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw
    GIT_TAG 3.3
)

FetchContent_GetProperties(glfw)
if(NOT glfw_POPULATED)
    FetchContent_Populate(glfw)

    set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL)
    set(GLFW_BUILD_TESTS OFF CACHE INTERNAL)
    set(GLFW_BUILD_DOCS OFF CACHE INTERNAL)
    set(GLFW_INSTALL OFF CACHE INTERNAL)

    add_subdirectory(${glfw_SOURCE_DIR} ${glfw_BINARY_DIR})
endif()