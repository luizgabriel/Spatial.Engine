include(CMakeParseArguments)

find_program(REGEN_PROGRAM resgen HINTS ${CONAN_CONFIG_DIR})

function(add_embed_resources TARGET)
    set(oneValueArgs OUTPUT PACKAGE)
    set(multiValueArgs SOURCES DEPENDEES)
    cmake_parse_arguments(ARG "" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    if(NOT IS_ABSOLUTE ${ARG_OUTPUT})
        set(ARG_OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/${ARG_OUTPUT}")
    endif()

    foreach(SOURCE ${ARG_SOURCES})
        if (IS_ABSOLUTE ${SOURCE})
            list(APPEND SOURCES_LIST ${SOURCE})
        else()
            list(APPEND SOURCES_LIST ${CMAKE_CURRENT_SOURCE_DIR}/${SOURCE})
        endif()
    endforeach()

    list(JOIN SOURCES_LIST "\" \"" SOURCE_FILES_LIST)
    add_custom_command(
        OUTPUT ${ARG_OUTPUT}/resources.h
        COMMAND ${REGEN_PROGRAM} -k -t -x ${ARG_OUTPUT} -p ${ARG_PACKAGE} \"${SOURCE_FILES_LIST}\"
        COMMENT ${REGEN_PROGRAM} -k -t -x ${ARG_OUTPUT} -p ${ARG_PACKAGE} \"${SOURCE_FILES_LIST}\"
    )

    #add_library(${TARGET} MODULE ${ARG_OUTPUT}/resources.h ${ARG_OUTPUT}/resources.S)

    #foreach(DEPENDEE ${ARG_DEPENDEES})
    #    target_link_libraries(${DEPENDEE} PRIVATE ${TARGET})
    #endforeach()
endfunction()
