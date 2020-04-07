if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/.conan/conanbuildinfo.cmake)
    include(${CMAKE_CURRENT_SOURCE_DIR}/.conan/conanbuildinfo.cmake)
    conan_basic_setup(TARGETS NO_OUTPUT_DIRS)
endif ()

if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/.conan/bin)
    file(GLOB_RECURSE PROGRAMS_TOOLS_FILES "${CMAKE_CURRENT_SOURCE_DIR}/.conan/bin/*")
    install(
        PROGRAMS
            ${PROGRAMS_TOOLS_FILES}
        DESTINATION
            bin
    )
endif ()
