include(CMakeParseArguments)

find_program(MATC_PROGRAM matc HINTS ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
find_program(CMGEN_PROGRAM cmgen HINTS ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

function(add_resources_library TARGET_NAME)
    set(oneValueArgs OUTPUT)
    set(multiValueArgs MATERIALS MODELS TEXTURES DEPENDEES)
    cmake_parse_arguments(RESLIB "" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
    
    foreach(MATERIAL_FILE ${RESLIB_MATERIALS})
        _add_custom_material_command(${MATERIAL_FILE} ${RESLIB_OUTPUT})
        list(APPEND MATERIAL_OUTPUT_FILES ${MATERIAL_OUTPUT_FILE})
    endforeach()

    foreach(TEXTURE_FILE ${RESLIB_TEXTURES})

        if (${TEXTURE_FILE} MATCHES ".+\.hdr$")
            _add_custom_hdr_texture_command(${TEXTURE_FILE} ${RESLIB_OUTPUT})
        elseif()
            _add_custom_texture_command(${TEXTURE_FILE} ${RESLIB_OUTPUT})
        endif()

        list(APPEND TEXTURE_OUTPUT_FILES ${TEXTURE_OUTPUT_FILE})
    endforeach()
    
    add_custom_target(${TARGET_NAME}
        DEPENDS 
            ${MATERIAL_OUTPUT_FILES}
            ${TEXTURE_OUTPUT_FILES}
    )

    foreach(DEPENDEE ${RESLIB_DEPENDEES})
        add_dependencies(${DEPENDEE} ${TARGET_NAME})
    endforeach()

endfunction()


function(_add_custom_texture_command TEXTURE_FILE OUTPUT)
    get_filename_component(TEXTURE_FILE_NAME ${TEXTURE_FILE} NAME)

    set(TEXTURE_OUTPUT_FOLDER  "${OUTPUT}/textures")
    set(TEXTURE_OUTPUT_FILE "${TEXTURE_OUTPUT_FOLDER}/${TEXTURE_FILE_NAME}")
    set(TEXTURE_OUTPUT_FILE "${TEXTURE_OUTPUT_FILE}" PARENT_SCOPE)

    add_custom_command(
        OUTPUT ${TEXTURE_OUTPUT_FILE}
        COMMAND ${CMAKE_COMMAND} -E make_directory ${TEXTURE_OUTPUT_FOLDER}
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/${TEXTURE_FILE} ${TEXTURE_OUTPUT_FILE} 
        COMMENT "Copying texture: ${TEXTURE_FILE} => ${TEXTURE_OUTPUT_FILE}"
    )
endfunction()

function(_add_custom_hdr_texture_command TEXTURE_FILE OUTPUT)
    get_filename_component(TEXTURE_FILE_NAME ${TEXTURE_FILE} NAME)
    string(REGEX REPLACE "\\.[^.]*$" "" TEXTURE_FILE_NAME ${TEXTURE_FILE_NAME})

    set(TEXTURE_OUTPUT_FOLDER  "${OUTPUT}/textures")
    set(TEXTURE_OUTPUT_FILE "${TEXTURE_OUTPUT_FOLDER}/${TEXTURE_FILE_NAME}/${TEXTURE_FILE_NAME}_ibl.ktx")
    list(APPEND TEXTURE_OUTPUT_FILE "${TEXTURE_OUTPUT_FOLDER}/${TEXTURE_FILE_NAME}/${TEXTURE_FILE_NAME}_skybox.ktx")
    list(APPEND TEXTURE_OUTPUT_FILE "${TEXTURE_OUTPUT_FOLDER}/${TEXTURE_FILE_NAME}/sh.txt")

    set(TEXTURE_OUTPUT_FILE "${TEXTURE_OUTPUT_FILE}" PARENT_SCOPE)

    execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory ${TEXTURE_OUTPUT_FOLDER})

    add_custom_command(
        OUTPUT ${TEXTURE_OUTPUT_FILE}
        COMMAND ${CMGEN_PROGRAM} -q --format=ktx --size=256 --extract-blur=0.1 ${CMAKE_CURRENT_SOURCE_DIR}/${TEXTURE_FILE}
        COMMENT "Compiling texture: ${TEXTURE_FILE}"
        WORKING_DIRECTORY ${TEXTURE_OUTPUT_FOLDER}
    )
endfunction()

function(_add_custom_material_command MATERIAL_FILE OUTPUT)
    get_filename_component(MATERIAL_FILE_NAME ${MATERIAL_FILE} NAME)
    string(REGEX REPLACE "\\.[^.]*$" "" MATERIAL_FILE_NAME ${MATERIAL_FILE_NAME})

    set(MATERIAL_OUTPUT_FOLDER  "${OUTPUT}/materials")
    set(MATERIAL_OUTPUT_FILE "${MATERIAL_OUTPUT_FOLDER}/${MATERIAL_FILE_NAME}.filamat")
    set(MATERIAL_OUTPUT_FILE "${MATERIAL_OUTPUT_FILE}" PARENT_SCOPE)

    add_custom_command(
        OUTPUT ${MATERIAL_OUTPUT_FILE}
        COMMAND ${CMAKE_COMMAND} -E make_directory ${MATERIAL_OUTPUT_FOLDER}
        COMMAND ${MATC_PROGRAM} -a all -p desktop -o ${MATERIAL_OUTPUT_FILE} ${CMAKE_CURRENT_SOURCE_DIR}/${MATERIAL_FILE}
        COMMENT "Compiling material: ${MATERIAL_FILE} => ${MATERIAL_OUTPUT_FILE}"
    )
endfunction()

