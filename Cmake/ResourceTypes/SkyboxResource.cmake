include(CMakeParseArguments)


find_program(CMGEN_PROGRAM cmgen HINTS ${CMAKE_BINARY_DIR})

function(add_skybox_resources TARGET_PREFIX)
    set(oneValueArgs OUTPUT FORMAT)
    set(multiValueArgs SOURCES DEPENDEES)
    cmake_parse_arguments(ARG "" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    foreach(SOURCE ${ARG_SOURCES})
        get_filename_component(FILE_NAME ${SOURCE} NAME)
        string(REGEX REPLACE "\\.[^.]*$" "" FILE_NAME ${FILE_NAME}) # Removes the .mat extenion
        set(OUTPUT_FILE ${ARG_OUTPUT}/${FILE_NAME}/sh.txt)

        add_custom_command(
            OUTPUT ${OUTPUT_FILE}
            COMMAND ${CMGEN_PROGRAM} -q -f ${ARG_FORMAT} -x ${ARG_OUTPUT} ${CMAKE_CURRENT_SOURCE_DIR}/${SOURCE}
            COMMENT "Compiling skybox: ${SOURCE} => [${ARG_FORMAT}] ${ARG_OUTPUT}/${FILE_NAME}"
        )

        set(TARGET "${TARGET_PREFIX}_${FILE_NAME}")
        add_custom_target(${TARGET}
            DEPENDS
                ${OUTPUT_FILE}
        )

        foreach(DEPENDEE ${ARG_DEPENDEES})
            add_dependencies(${DEPENDEE} ${TARGET})
        endforeach()
    endforeach()
endfunction()
