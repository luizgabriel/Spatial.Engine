find_program(FILAMESH_COMMAND filamesh)
add_custom_command(
    OUTPUT 
        Meshes/plane_hd.filamesh
        Meshes/space_ship.filamesh
        Meshes/sphere_hd.filamesh
    DEPENDS
        Meshes/plane_hd.obj
        Meshes/space_ship.obj
        Meshes/sphere_hd.obj
    COMMAND ${FILAMESH_COMMAND} Meshes/plane_hd.obj Meshes/plane_hd.filamesh
    COMMAND ${FILAMESH_COMMAND} Meshes/space_ship.obj Meshes/space_ship.filamesh
    COMMAND ${FILAMESH_COMMAND} Meshes/sphere_hd.obj Meshes/sphere_hd.filamesh
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_custom_target(Spatial.Game
    COMMAND $<TARGET_FILE:Spatial.Editor>
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    DEPENDS
        Meshes/plane_hd.filamesh
        Meshes/space_ship.filamesh
        Meshes/sphere_hd.filamesh
    COMMENT Running $<TARGET_FILE:Spatial.Editor> on ${CMAKE_CURRENT_SOURCE_DIR} project
)