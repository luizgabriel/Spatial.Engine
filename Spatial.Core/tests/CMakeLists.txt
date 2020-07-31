add_executable(Spatial.Core.Tests
	test.cpp)

target_link_libraries(Spatial.Core.Tests PUBLIC Spatial.Core CONAN_PKG::gtest)

include(GoogleTest)
gtest_discover_tests(Spatial.Core.Tests)
