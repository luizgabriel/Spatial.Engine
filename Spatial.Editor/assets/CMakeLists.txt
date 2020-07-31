find_program(MATC_COMMAND matc)
add_custom_command(
	OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/materials/default.filamat ${CMAKE_CURRENT_BINARY_DIR}/materials/ui_blit.filamat
	COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/materials/
	COMMAND ${MATC_COMMAND} -o ${CMAKE_CURRENT_BINARY_DIR}/materials/default.filamat materials/default.mat
	COMMAND ${MATC_COMMAND} -o ${CMAKE_CURRENT_BINARY_DIR}/materials/ui_blit.filamat materials/ui_blit.mat
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_custom_target(Spatial.Editor.Materials SOURCES
	${CMAKE_CURRENT_BINARY_DIR}/materials/default.filamat
	${CMAKE_CURRENT_BINARY_DIR}/materials/ui_blit.filamat)

set(SPATIAL_EDITOR_RESOURCE_FILES
	fonts/Roboto_Medium.ttf
	${CMAKE_CURRENT_BINARY_DIR}/materials/default.filamat
	${CMAKE_CURRENT_BINARY_DIR}/materials/ui_blit.filamat
	meshes/cube.obj
	meshes/plane.obj
	meshes/sphere.obj)

add_custom_command(
	OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/generated.cpp
	DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/materials/default.filamat ${CMAKE_CURRENT_BINARY_DIR}/materials/ui_blit.filamat
	COMMAND $<TARGET_FILE:Spatial.Res> -p ASSETS_ -h ${CMAKE_CURRENT_BINARY_DIR}/generated.h -s ${CMAKE_CURRENT_BINARY_DIR}/generated.cpp ${SPATIAL_EDITOR_RESOURCE_FILES}
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_library(Spatial.Editor.Resources STATIC ${CMAKE_CURRENT_BINARY_DIR}/generated.cpp)
target_include_directories(Spatial.Editor.Resources PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/../)
