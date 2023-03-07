if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
  message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
  file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/0.18.1/conan.cmake"
                "${CMAKE_BINARY_DIR}/conan.cmake"
                TLS_VERIFY ON)
endif()

include(${CMAKE_BINARY_DIR}/conan.cmake)

if (CONAN_EXPORTED)
    include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    conan_basic_setup(TARGETS NO_OUTPUT_DIRS)
else ()
    if (CONAN_PROFILE)
        message("Conan: using CONAN_PROFILE=${CONAN_PROFILE}")
        conan_cmake_run(
                CONANFILE conanfile.py
                BUILD missing
                PROFILE ${CONAN_PROFILE}
                BASIC_SETUP CMAKE_TARGETS NO_OUTPUT_DIRS KEEP_RPATHS)
    else ()
        message("Conan: detecting conan profile settings. If this is failing, you should provide a -DCONAN_PROFILE=yourprofile argument")
        conan_cmake_autodetect(CONAN_SETTINGS)
        conan_cmake_run(
                CONANFILE conanfile.py
                BUILD missing
                SETTINGS ${CONAN_SETTINGS}
                BASIC_SETUP CMAKE_TARGETS NO_OUTPUT_DIRS KEEP_RPATHS)
    endif ()
endif ()
