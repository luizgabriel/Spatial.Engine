find_program(FILAMESH_COMMAND filamesh)
add_custom_command(
    OUTPUT 
        meshes/plane_hd.filamesh
        meshes/space_ship.filamesh
        meshes/sphere_hd.filamesh
    DEPENDS
        meshes/plane_hd.obj
        meshes/space_ship.obj
        meshes/sphere_hd.obj
    COMMAND ${FILAMESH_COMMAND} meshes/plane_hd.obj meshes/plane_hd.filamesh
    COMMAND ${FILAMESH_COMMAND} meshes/space_ship.obj meshes/space_ship.filamesh
    COMMAND ${FILAMESH_COMMAND} meshes/sphere_hd.obj meshes/sphere_hd.filamesh
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_custom_target(Spatial.Game
    COMMAND $<TARGET_FILE:Spatial.Editor>
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    DEPENDS
        meshes/plane_hd.filamesh
        meshes/space_ship.filamesh
        meshes/sphere_hd.filamesh
    COMMENT Running $<TARGET_FILE:Spatial.Editor> on ${CMAKE_CURRENT_SOURCE_DIR} project
)