if(CONAN_EXPORTED)
    include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    conan_basic_setup(TARGETS)
else()
    list(APPEND CMAKE_PREFIX_PATH ${CMAKE_CURRENT_BINARY_DIR})

    include(Conan)
    
    option(CONAN_COMPILER_TOOLSET "Defaults the toolset to LLVM" "LLVM")

    conan_cmake_run(
        CONANFILE conanfile.txt
        PROFILE clang
        BUILD missing
    )

    # SDL2 can't build with clang
    conan_cmake_run(
        REQUIRES sdl2/2.0.9@bincrafters/stable
        PROFILE gcc
        BUILD missing
        GENERATORS cmake_find_package
    )

    find_package(fmt CONFIG REQUIRED)
    find_package(entt CONFIG REQUIRED)
    find_package(filament CONFIG REQUIRED)
    find_package(sdl2 CONFIG REQUIRED)
endif()
