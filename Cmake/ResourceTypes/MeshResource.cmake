include(CMakeParseArguments)


find_program(FILAMESH_PROGRAM filamesh HINTS ${CMAKE_BINARY_DIR})

function(add_mesh_resources TARGET)
    set(oneValueArgs OUTPUT)
    set(multiValueArgs SOURCES DEPENDEES)
    cmake_parse_arguments(ARG "" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    foreach(SOURCE ${ARG_SOURCES})
        get_filename_component(FILE_NAME ${SOURCE} NAME)
        string(REGEX REPLACE "\\.[^.]*$" "" FILE_NAME ${FILE_NAME}) # Removes the .mat extenion
        set(OUTPUT_FILE "${ARG_OUTPUT}/${FILE_NAME}.filamesh")

        add_custom_command(
            OUTPUT ${OUTPUT_FILE}
            COMMAND ${FILAMESH_PROGRAM} ${CMAKE_CURRENT_SOURCE_DIR}/${SOURCE} ${OUTPUT_FILE}
            COMMENT "Compiling mesh: ${SOURCE} => ${OUTPUT_FILE}"
        )

        list(APPEND OUTPUT_FILES "${OUTPUT_FILE}" PARENT_SCOPE)
    endforeach()

    add_custom_target(${TARGET}
        DEPENDS 
            ${OUTPUT_FILES}
    )

    foreach(DEPENDEE ${ARG_DEPENDEES})
        add_dependencies(${DEPENDEE} ${TARGET})
    endforeach()
endfunction()
