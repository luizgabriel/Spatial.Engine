include(CMakeParseArguments)


find_program(MATC_PROGRAM matc HINTS ${CMAKE_BINARY_DIR})

function(add_material_resources TARGET)
    set(oneValueArgs OUTPUT)
    set(multiValueArgs SOURCES DEPENDEES)
    cmake_parse_arguments(ARG "" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    if(NOT IS_ABSOLUTE ${ARG_OUTPUT})
        set(ARG_OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/${ARG_OUTPUT}")
    endif()

    foreach(SOURCE ${ARG_SOURCES})
        get_filename_component(FILE_NAME ${SOURCE} NAME)
        string(REGEX REPLACE "\\.[^.]*$" "" FILE_NAME ${FILE_NAME}) # Removes the .mat extenion
        set(OUTPUT_FILE "${ARG_OUTPUT}/${FILE_NAME}.filamat")

        add_custom_command(
            OUTPUT ${OUTPUT_FILE}
            COMMAND ${MATC_PROGRAM} -a all -p desktop -o ${OUTPUT_FILE} ${CMAKE_CURRENT_SOURCE_DIR}/${SOURCE}
            COMMENT "Compiling material: ${SOURCE} => ${OUTPUT_FILE}"
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