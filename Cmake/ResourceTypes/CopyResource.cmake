include(CMakeParseArguments)


function(add_copy_resources TARGET)
    set(oneValueArgs OUTPUT)
    set(multiValueArgs SOURCES DEPENDEES)
    cmake_parse_arguments(ARG "" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    foreach(SOURCE ${ARG_SOURCES})
        get_filename_component(FILE_NAME ${SOURCE} NAME)
        set(OUTPUT_FILE "${ARG_OUTPUT}/${FILE_NAME}")

        add_custom_command(
            OUTPUT ${OUTPUT_FILE}
            COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/${SOURCE} ${OUTPUT_FILE} 
            COMMENT "Copying: ${SOURCE} => ${OUTPUT_FILE}"
        )

        list(APPEND OUTPUT_FILES ${OUTPUT_FILE})
    endforeach()
    
    add_custom_target(${TARGET}
        DEPENDS 
            ${OUTPUT_FILES}
    )

    foreach(DEPENDEE ${ARG_DEPENDEES})
        add_dependencies(${DEPENDEE} ${TARGET})
    endforeach()
endfunction()
