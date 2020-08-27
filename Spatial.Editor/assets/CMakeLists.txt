find_program(CMGEN_COMMAND cmgen)
add_custom_command(
	OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/textures/default_skybox/sh.txt ${CMAKE_CURRENT_BINARY_DIR}/textures/default_skybox/default_skybox_ibl.ktx ${CMAKE_CURRENT_BINARY_DIR}/textures/default_skybox/default_skybox_skybox.ktx
	COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/textures/
	COMMAND ${CMGEN_COMMAND} -q -f ktx -s 512 -x ${CMAKE_CURRENT_BINARY_DIR}/textures/default_skybox textures/default_skybox.exr
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

find_program(MATC_COMMAND matc)
add_custom_command(
	OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/materials/default.filamat ${CMAKE_CURRENT_BINARY_DIR}/materials/ui_blit.filamat
	DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/materials/default.mat ${CMAKE_CURRENT_SOURCE_DIR}/materials/ui_blit.mat
	COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/materials/
	COMMAND ${MATC_COMMAND} -o ${CMAKE_CURRENT_BINARY_DIR}/materials/default.filamat materials/default.mat
	COMMAND ${MATC_COMMAND} -o ${CMAKE_CURRENT_BINARY_DIR}/materials/ui_blit.filamat materials/ui_blit.mat
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

find_program(FILAMESH_COMMAND filamesh)
add_custom_command(
	OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/meshes/plane.filamesh ${CMAKE_CURRENT_BINARY_DIR}/meshes/cube.filamesh ${CMAKE_CURRENT_BINARY_DIR}/meshes/sphere.filamesh ${CMAKE_CURRENT_BINARY_DIR}/meshes/cylinder.filamesh
	DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/meshes/plane.obj ${CMAKE_CURRENT_SOURCE_DIR}/meshes/cube.obj ${CMAKE_CURRENT_SOURCE_DIR}/meshes/sphere.obj ${CMAKE_CURRENT_SOURCE_DIR}/meshes/cylinder.obj
	COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/meshes/
	COMMAND ${FILAMESH_COMMAND} meshes/plane.obj ${CMAKE_CURRENT_BINARY_DIR}/meshes/plane.filamesh
	COMMAND ${FILAMESH_COMMAND} meshes/cube.obj ${CMAKE_CURRENT_BINARY_DIR}/meshes/cube.filamesh
	COMMAND ${FILAMESH_COMMAND} meshes/sphere.obj ${CMAKE_CURRENT_BINARY_DIR}/meshes/sphere.filamesh
	COMMAND ${FILAMESH_COMMAND} meshes/cylinder.obj ${CMAKE_CURRENT_BINARY_DIR}/meshes/cylinder.filamesh
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_custom_target(Spatial.Editor.Materials SOURCES
	${CMAKE_CURRENT_BINARY_DIR}/materials/default.filamat
	${CMAKE_CURRENT_BINARY_DIR}/materials/ui_blit.filamat)

set(SPATIAL_EDITOR_RESOURCE_FILES
	fonts/Roboto_Medium.ttf
	${CMAKE_CURRENT_BINARY_DIR}/materials/default.filamat
	${CMAKE_CURRENT_BINARY_DIR}/materials/ui_blit.filamat
	${CMAKE_CURRENT_BINARY_DIR}/textures/default_skybox/sh.txt
	${CMAKE_CURRENT_BINARY_DIR}/textures/default_skybox/default_skybox_ibl.ktx
	${CMAKE_CURRENT_BINARY_DIR}/textures/default_skybox/default_skybox_skybox.ktx
	textures/uv.png
	${CMAKE_CURRENT_BINARY_DIR}/meshes/plane.filamesh
	${CMAKE_CURRENT_BINARY_DIR}/meshes/cube.filamesh
	${CMAKE_CURRENT_BINARY_DIR}/meshes/sphere.filamesh
	${CMAKE_CURRENT_BINARY_DIR}/meshes/cylinder.filamesh)

add_custom_command(
	OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/generated.cpp
	DEPENDS ${SPATIAL_EDITOR_RESOURCE_FILES}
	COMMAND $<TARGET_FILE:Spatial.Res> -p ASSETS_ -h ${CMAKE_CURRENT_BINARY_DIR}/generated.h -s ${CMAKE_CURRENT_BINARY_DIR}/generated.cpp ${SPATIAL_EDITOR_RESOURCE_FILES}
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_library(Spatial.Editor.Resources STATIC ${CMAKE_CURRENT_BINARY_DIR}/generated.cpp)
target_include_directories(Spatial.Editor.Resources PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/../)
