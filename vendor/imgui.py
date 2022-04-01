import os

from conans import ConanFile, CMake, tools

CMAKE_FILE_DATA = """
cmake_minimum_required(VERSION 3.19)
project(imgui CXX)

# Setup C++17 Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(conanbuildinfo.cmake)
conan_basic_setup()

set(CMAKE_VERBOSE_MAKEFILE TRUE)
if (WIN32 AND MSVC AND BUILD_SHARED_LIBS)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif(WIN32 AND MSVC AND BUILD_SHARED_LIBS)

set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/source_subfolder)
set(MISC_DIR ${SOURCE_DIR}/misc)
set(EXTRA_FONTS_DIR ${MISC_DIR}/fonts)

file(GLOB SOURCE_FILES ${SOURCE_DIR}/*.cpp)
file(GLOB HEADER_FILES ${SOURCE_DIR}/*.h)
file(GLOB EXTRA_FONTS_FILES ${EXTRA_FONTS_DIR}/*.ttf)
if (MSVC)
        file(GLOB EXTRA_NATVIS_FILES ${MISC_DIR}/natvis/*.natvis)
endif()

set(BINARY_TO_COMPRESSED_BIN binary_to_compressed_c)

add_executable(${BINARY_TO_COMPRESSED_BIN} ${EXTRA_FONTS_DIR}/binary_to_compressed_c.cpp)

add_library(${PROJECT_NAME} ${SOURCE_FILES})
target_include_directories(${PROJECT_NAME} PRIVATE ${SOURCE_DIR})
target_link_libraries(${PROJECT_NAME} PUBLIC ${CONAN_LIBS})

install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
        LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
        ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
install(TARGETS ${BINARY_TO_COMPRESSED_BIN}
        RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
install(FILES ${HEADER_FILES}
        DESTINATION ${CMAKE_INSTALL_PREFIX}/include
        PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ WORLD_READ)
install(FILES ${EXTRA_FONTS_FILES}
        DESTINATION ${CMAKE_INSTALL_PREFIX}/misc/fonts
        PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ WORLD_READ)
if (MSVC)
        install(FILES ${EXTRA_NATVIS_FILES}
                DESTINATION ${CMAKE_INSTALL_PREFIX}/misc/natvis
                PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ WORLD_READ)
endif()
"""


class IMGUIConan(ConanFile):
    name = "imgui"
    version = "docking"
    url = "https://github.com/bincrafters/conan-imgui"
    homepage = "https://github.com/ocornut/imgui"
    description = "Bloat-free Immediate Mode Graphical User interface for C++ with minimal dependencies"
    topics = ("conan", "imgui", "gui", "graphical")
    license = "MIT"
    exports = ["LICENSE.md"]
    exports_sources = ["CMakeLists.txt", ]
    generators = "cmake"
    revision_mode = "hash"
    settings = "os", "arch", "compiler", "build_type"
    options = {"shared": [True, False], "fPIC": [True, False]}
    default_options = {"shared": False, "fPIC": True}
    _source_subfolder = "source_subfolder"

    def config_options(self):
        if self.settings.os == 'Windows':
            del self.options.fPIC

    def source(self):
        tools.get("{0}/archive/{1}.tar.gz".format(self.homepage, self.version))
        extracted_dir = self.name + "-" + self.version
        os.rename(extracted_dir, self._source_subfolder)

        with open("CMakeLists.txt", "w+") as f:
            f.write(CMAKE_FILE_DATA)

    def _configure_cmake(self):
        cmake = CMake(self)
        cmake.configure()
        return cmake

    def build(self):
        cmake = self._configure_cmake()
        cmake.build()

    def package(self):
        self.copy(pattern="LICENSE.txt", dst="licenses", src=self._source_subfolder)
        self.copy(pattern="examples/imgui_impl_*", dst="misc/bindings", src=self._source_subfolder, keep_path=False)
        cmake = self._configure_cmake()
        cmake.install()

    def package_info(self):
        self.cpp_info.srcdirs = ["misc", ]
        self.cpp_info.libs = tools.collect_libs(self)
