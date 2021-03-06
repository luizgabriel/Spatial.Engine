find_program(FILAMESH_COMMAND filamesh)
add_custom_command(
	OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/assets/plane.filamesh
	DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/assets/plane.obj
	COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/assets/
	COMMAND ${FILAMESH_COMMAND} assets/plane.obj ${CMAKE_CURRENT_BINARY_DIR}/assets/plane.filamesh
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(Spatial.Core.Tests
	${CMAKE_CURRENT_BINARY_DIR}/assets/plane.filamesh
	Application.test.cpp
	RegistrySerialization.test.cpp
	DesktopWindowContext.test.cpp
	RenderingSystem.test.cpp)

target_link_libraries(Spatial.Core.Tests PUBLIC Spatial.Core CONAN_PKG::gtest)

include(GoogleTest)
gtest_discover_tests(Spatial.Core.Tests)