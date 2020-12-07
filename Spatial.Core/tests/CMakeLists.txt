add_custom_command(
	OUTPUT  ${CMAKE_CURRENT_BINARY_DIR}/assets/test.txt
	COMMAND ${CMAKE_COMMAND} -E copy assets/test.txt ${CMAKE_CURRENT_BINARY_DIR}/assets/test.txt
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(Spatial.Core.Tests
	${CMAKE_CURRENT_BINARY_DIR}/assets/test.txt
	Application.test.cpp
	Stage.test.cpp
	DesktopWindowContext.test.cpp
	PhysicalDirLoader.test.cpp
	RenderingSystem.test.cpp)

target_link_libraries(Spatial.Core.Tests PUBLIC Spatial.Core CONAN_PKG::gtest)

include(GoogleTest)
gtest_discover_tests(Spatial.Core.Tests)
