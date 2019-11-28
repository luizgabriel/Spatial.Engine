FetchContent_Declare(filament
    GIT_REPOSITORY https://github.com/google/filament
    GIT_TAG v1.4.2
)

FetchContent_GetProperties(filament)
if(NOT filament_POPULATED)
    FetchContent_Populate(filament)

    set(ENABLE_JAVA OFF CACHE INTERNAL "")
	set(INSTALL_BACKEND_TEST OFF CACHE INTERNAL "")
	if (MSVC)
		set(CMAKE_MSVC_RUNTIME_LIBRARY "" CACHE INTERNAL "")
	endif()

    add_subdirectory(${filament_SOURCE_DIR} ${filament_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()