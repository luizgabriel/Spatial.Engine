include(CMakeParseArguments)

find_program(MATC_PROGRAM matc HINTS ${CMAKE_BINARY_DIR})
find_program(CMGEN_PROGRAM cmgen HINTS ${CMAKE_BINARY_DIR})
find_program(FILAMESH_PROGRAM filamesh HINTS ${CMAKE_BINARY_DIR})

function(add_resources_library TARGET_NAME)
    set(oneValueArgs OUTPUT)
    set(multiValueArgs SKYBOX MODEL MATERIAL COPY DEPENDEES)
    cmake_parse_arguments(RESLIB "" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    message("${CMAKE_COMMAND} -E make_directory ${RESLIB_OUTPUT}")
    execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory ${RESLIB_OUTPUT})

    foreach(SKYBOX_FILE ${RESLIB_SKYBOX})
        _add_custom_skybox_command(${SKYBOX_FILE} ${RESLIB_OUTPUT})
        list(APPEND SKYBOX_OUTPUT_FILES ${SKYBOX_OUTPUT_FILE})
    endforeach()

    foreach(MODEL_FILE ${RESLIB_MODEL})
        _add_custom_model_command(${MODEL_FILE} ${RESLIB_OUTPUT})
        list(APPEND MODEL_OUTPUT_FILES ${MODEL_OUTPUT_FILE})
    endforeach()
    
    foreach(MATERIAL_FILE ${RESLIB_MATERIAL})
        _add_custom_material_command(${MATERIAL_FILE} ${RESLIB_OUTPUT})
        list(APPEND MATERIAL_OUTPUT_FILES ${MATERIAL_OUTPUT_FILE})
    endforeach()

    foreach(COPY_FILE ${RESLIB_COPY})
        _add_custom_copy_command(${COPY_FILE} ${RESLIB_OUTPUT})
        list(APPEND COPY_OUTPUT_FILES ${COPY_OUTPUT_FILE})
    endforeach()
    
    add_custom_target(${TARGET_NAME}
        DEPENDS 
            ${SKYBOX_OUTPUT_FILES}
            ${MODEL_OUTPUT_FILES}
            ${MATERIAL_OUTPUT_FILES}
            ${COPY_OUTPUT_FILES}
    )

    foreach(DEPENDEE ${RESLIB_DEPENDEES})
        add_dependencies(${DEPENDEE} ${TARGET_NAME})
    endforeach()

endfunction()


function(_add_custom_copy_command COPY_FILE OUTPUT)
    get_filename_component(COPY_FILE_NAME ${COPY_FILE} NAME)

    set(COPY_OUTPUT_FILE "${OUTPUT}/${COPY_FILE_NAME}")
    add_custom_command(
        OUTPUT ${COPY_OUTPUT_FILE}
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/${COPY_FILE} ${COPY_OUTPUT_FILE} 
        COMMENT "Copying: ${COPY_FILE} => ${COPY_OUTPUT_FILE}"
    )

    set(COPY_OUTPUT_FILE "${COPY_OUTPUT_FILE}" PARENT_SCOPE)
endfunction()


function(_add_custom_material_command MATERIAL_FILE OUTPUT)
    get_filename_component(MATERIAL_FILE_NAME ${MATERIAL_FILE} NAME)
    string(REGEX REPLACE "\\.[^.]*$" "" MATERIAL_FILE_NAME ${MATERIAL_FILE_NAME})

    set(MATERIAL_OUTPUT_FILE "${OUTPUT}/${MATERIAL_FILE_NAME}.filamat")
    add_custom_command(
        OUTPUT ${MATERIAL_OUTPUT_FILE}
        COMMAND ${MATC_PROGRAM} -a all -p desktop -o ${MATERIAL_OUTPUT_FILE} ${CMAKE_CURRENT_SOURCE_DIR}/${MATERIAL_FILE}
        COMMENT "Compiling material: ${MATERIAL_FILE} => ${MATERIAL_OUTPUT_FILE}"
    )

    set(MATERIAL_OUTPUT_FILE "${MATERIAL_OUTPUT_FILE}" PARENT_SCOPE)
endfunction()


function(_add_custom_model_command MODEL_FILE OUTPUT)
    get_filename_component(MODEL_FILE_NAME ${MODEL_FILE} NAME)
    string(REGEX REPLACE "\\.[^.]*$" "" MODEL_FILE_NAME ${MODEL_FILE_NAME})

    set(MODEL_OUTPUT_FILE "${OUTPUT}/${MODEL_FILE_NAME}.filamesh")
    add_custom_command(
        OUTPUT ${MODEL_OUTPUT_FILE}
        COMMAND ${FILAMESH_PROGRAM} ${CMAKE_CURRENT_SOURCE_DIR}/${MODEL_FILE} ${MODEL_OUTPUT_FILE} 
        COMMENT "Compiling model: ${MODEL_FILE} => ${MODEL_OUTPUT_FILE}"
    )

    set(MODEL_OUTPUT_FILE "${MODEL_OUTPUT_FILE}" PARENT_SCOPE)
endfunction()


function(_add_custom_skybox_command SKYBOX_FILE OUTPUT)
    get_filename_component(SKYBOX_FILE_NAME ${SKYBOX_FILE} NAME)
    string(REGEX REPLACE "\\.[^.]*$" "" SKYBOX_FILE_NAME ${SKYBOX_FILE_NAME})

    set(SKYBOX_OUTPUT_FILE ${OUTPUT}/${SKYBOX_FILE_NAME})
    add_custom_command(
        OUTPUT ${SKYBOX_OUTPUT_FILE}
        COMMAND ${CMGEN_PROGRAM} -q ${CMAKE_CURRENT_SOURCE_DIR}/${SKYBOX_FILE}
        WORKING_DIRECTORY ${OUTPUT}
        COMMENT "Compiling skybox: ${SKYBOX_FILE} => ${SKYBOX_OUTPUT_FILE}"
    )
    set_property(SOURCE ${SKYBOX_OUTPUT_FILE} PROPERTY SYMBOLIC ON)

    set(SKYBOX_OUTPUT_FILE ${SKYBOX_OUTPUT_FILE} PARENT_SCOPE)
endfunction()
