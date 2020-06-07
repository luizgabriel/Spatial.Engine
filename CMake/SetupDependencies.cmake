set(CONAN_CONFIG_DIR ${CMAKE_CURRENT_SOURCE_DIR}/.conan)

if (EXISTS ${CONAN_CONFIG_DIR}/conanbuildinfo.cmake)
    set(CONAN_DISABLE_CHECK_COMPILER ON)
    include(${CONAN_CONFIG_DIR}/conanbuildinfo.cmake)
    conan_basic_setup(TARGETS NO_OUTPUT_DIRS)
endif ()

if (EXISTS ${CONAN_CONFIG_DIR}/bin)
    file(GLOB_RECURSE PROGRAMS_TOOLS_FILES "${CONAN_CONFIG_DIR}/bin/*")
    install(
        PROGRAMS
            ${PROGRAMS_TOOLS_FILES}
        DESTINATION
            bin
    )
endif ()