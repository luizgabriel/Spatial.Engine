include(CMakeParseArguments)


find_program(FILAMESH_PROGRAM filamesh HINTS ${CONAN_CONFIG_DIR})

function(add_mesh_resources TARGET)
    set(oneValueArgs OUTPUT)
    set(multiValueArgs SOURCES DEPENDEES)
    cmake_parse_arguments(ARG "" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    if(NOT IS_ABSOLUTE ${ARG_OUTPUT})
        set(ARG_OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/${ARG_OUTPUT}")
    endif()

    file(MAKE_DIRECTORY ${ARG_OUTPUT})

    foreach(SOURCE ${ARG_SOURCES})
        get_filename_component(FILE_NAME ${SOURCE} NAME)
        string(REGEX REPLACE "\\.[^.]*$" "" FILE_NAME ${FILE_NAME}) # Removes the .mat extenion
        set(OUTPUT_FILE "${ARG_OUTPUT}/${FILE_NAME}.filamesh")

        add_custom_command(
            OUTPUT ${OUTPUT_FILE}
            COMMAND ${FILAMESH_PROGRAM} ${CMAKE_CURRENT_SOURCE_DIR}/${SOURCE} ${OUTPUT_FILE}
            COMMENT "Compiling mesh: ${SOURCE} => ${OUTPUT_FILE}"
        )

        list(APPEND OUTPUT_FILES "${OUTPUT_FILE}")
    endforeach()

    target_sources(${TARGET} PUBLIC ${OUTPUT_FILES})
    install(FILES ${OUTPUT_FILES} DESTINATION ${ARG_OUTPUT})
endfunction()
