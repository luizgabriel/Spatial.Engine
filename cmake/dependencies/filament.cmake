FetchContent_Declare(
    filament
    GIT_REPOSITORY https://github.com/google/filament
    GIT_TAG v1.4.2
)

FetchContent_GetProperties(filament)
if(NOT filament_POPULATED)
    FetchContent_Populate(filament)

    set(ENABLE_JAVA OFF CACHE INTERNAL)

    add_subdirectory(${filament_SOURCE_DIR} ${filament_BINARY_DIR})
endif()