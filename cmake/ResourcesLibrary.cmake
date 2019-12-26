include(CMakeParseArguments)

find_program(MATC_PROGRAM matc HINTS ${CMAKE_BINARY_DIR})

function(add_resources_library TARGET_NAME)
    set(oneValueArgs FOLDER)
    set(multiValueArgs MATERIALS MODELS TEXTURES DEPENDEES)
    cmake_parse_arguments(RESOURCE_LIBRARY "" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
    
    foreach(MATERIAL_FILE ${RESOURCE_LIBRARY_MATERIALS})
        get_filename_component(MATERIAL_FILE_NAME ${MATERIAL_FILE} NAME)
        string(REGEX REPLACE "\\.[^.]*$" "" MATERIAL_FILE_NAME ${MATERIAL_FILE_NAME})

        if (RESOURCE_LIBRARY_FOLDER)
            set(MATERIAL_OUTPUT_FOLDER  "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets/${RESOURCE_LIBRARY_FOLDER}/materials")
        else()
            set(MATERIAL_OUTPUT_FOLDER  "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets/materials")
        endif()

        set(MATERIAL_OUTPUT_FILE "${MATERIAL_OUTPUT_FOLDER}/${MATERIAL_FILE_NAME}.bin")

        add_custom_command(
            OUTPUT ${MATERIAL_OUTPUT_FILE}
            COMMAND ${CMAKE_COMMAND} -E make_directory ${MATERIAL_OUTPUT_FOLDER}
            COMMAND ${MATC_PROGRAM} -a opengl -o ${MATERIAL_OUTPUT_FILE} ${CMAKE_CURRENT_SOURCE_DIR}/${MATERIAL_FILE}
            DEPENDS ${SHADER_FILE}
            COMMENT "Compiling material: ${MATERIAL_FILE} => ${MATERIAL_OUTPUT_FILE}"
        )

        list(APPEND MATERIAL_OUTPUT_FILES ${MATERIAL_OUTPUT_FILE})
    endforeach()
    
    add_custom_target(${TARGET_NAME}
        DEPENDS 
            ${MATERIAL_OUTPUT_FILES}
    )

    foreach(DEPENDEE ${RESOURCE_LIBRARY_DEPENDEES})
        add_dependencies(${DEPENDEE} ${TARGET_NAME})
    endforeach()

endfunction()