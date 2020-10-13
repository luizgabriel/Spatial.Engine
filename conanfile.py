from conans import ConanFile, CMake, tools


class SpatialEngineConan(ConanFile):
    name = "spatial"
    version = "1.2.0"
    license = "Apache 2.0"
    url = "https://github.com/luizgabriel/Spatial.Engine"
    description = "Spatial is a cross-platform c++ game engine created on top off google's [filament](" \
                  "https://github.com/google/filament) rendering engine. This projects uses C++17 and modern cmake " \
                  "features. "
    settings = "os", "compiler", "build_type", "arch"
    options = {"editor": [True, False]}
    default_options = {"editor": True}
    generators = "cmake"
    build_requires = "cmake/3.16.3"
    exports_sources = "*"
    requires = [
        "boost_tti/1.67.0@bincrafters/stable",
        "sdl2/2.0.12@bincrafters/stable",
        "argh/1.2.1@adishavit/stable",
        "gtest/1.10.0",
        "fmt/7.0.3",
        "spdlog/1.8.0",
        "stb/20200203",
        "entt/3.5.2",

        "imgui/docking@ocornut/stable",  # Installed with vendor/imgui.py
        "filament/1.8.1@google/stable",  # Installed with vendor/filament.py
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
        self.cpp_info.resdirs = ["assets"]
        self.cpp_info.libs = tools.collect_libs(self)
