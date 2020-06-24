from conans import ConanFile, CMake, tools


class SpatialEngineConan(ConanFile):
    name = "spatial"
    version = "1.0.0"
    license = "Apache 2.0"
    url = "https://github.com/luizgabriel/Spatial.Engine"
    description = "Spatial is a cross-platform c++ game engine created on top off google's [filament](" \
                  "https://github.com/google/filament) rendering engine. This projects uses C++17 and modern cmake " \
                  "features. "
    settings = "os", "compiler", "build_type", "arch"
    options = {"editor": [True, False]}
    default_options = {"editor": True}
    generators = "cmake"
    build_requires = "cmake_installer/3.16.3@conan/stable"
    exports_sources = "*"
    requires = [
        "entt/3.4.0@skypjack/stable",
        "fmt/6.0.0@bincrafters/stable",
        "spdlog/1.4.2@bincrafters/stable",
        "stb/20190512@conan/stable",
        "boost_tti/1.67.0@bincrafters/stable",
        "sdl2/2.0.12@bincrafters/stable",
        "filament/1.7.0@google/stable",
        "imgui/docking@ocornut/testing",
        "toml++/1.2.0@marzer/stable",
    ]

    def imports(self):
        self.copy("*", "bin", "bin", "filament")

    def _cmake(self):
        cmake = CMake(self)
        cmake.definitions["SPATIAL_EDITOR"] = "ON" if self.options["editor"] else "OFF"
        cmake.configure()

        return cmake

    def build(self):
        cmake = self._cmake()
        cmake.build()

    def package(self):
        cmake = self._cmake()
        cmake.install()

    def package_info(self):
        self.cpp_info.builddirs = ["lib/cmake"]
        self.cpp_info.libs = tools.collect_libs(self)
