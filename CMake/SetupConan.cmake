if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
	message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
	file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/v0.16.1/conan.cmake"
		"${CMAKE_BINARY_DIR}/conan.cmake"
		EXPECTED_HASH SHA256=396e16d0f5eabdc6a14afddbcfff62a54a7ee75c6da23f32f7a31bc85db23484
		TLS_VERIFY ON)
endif()

include(${CMAKE_BINARY_DIR}/conan.cmake)

if(CONAN_EXPORTED)
	include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
	conan_basic_setup(TARGETS NO_OUTPUT_DIRS)
else()
	if(CONAN_PROFILE)
		conan_cmake_run(
				CONANFILE conanfile.py
				BUILD missing
				PROFILE ${CONAN_PROFILE}
				BASIC_SETUP CMAKE_TARGETS NO_OUTPUT_DIRS)
	else()
		conan_cmake_autodetect(CONAN_SETTINGS)
		conan_cmake_run(
				CONANFILE conanfile.py
				BUILD missing
				SETTINGS ${CONAN_SETTINGS}
				BASIC_SETUP CMAKE_TARGETS NO_OUTPUT_DIRS)
	endif()
endif()
