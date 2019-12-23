if(CONAN_EXPORTED)
    include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    conan_basic_setup(TARGETS)
else()
    include(Conan)

    conan_cmake_run(
        CONANFILE conanfile.txt
        PROFILE clang
        BUILD missing
        BASIC_SETUP CMAKE_TARGETS
    )    
endif()

find_package(SDL2 CONFIG REQUIRED)
