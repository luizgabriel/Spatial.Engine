from conans import ConanFile, CMake, tools


class SpatialEngineConan(ConanFile):
    name = "spatial"
    version = "1.3.0"
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
        "boost_locale/1.69.0@bincrafters/stable",
        "boost_tti/1.69.0@bincrafters/stable",
        "argh/1.2.1@adishavit/stable",
        "glfw/3.3.4",
        "ghc-filesystem/1.4.0",
        "gtest/1.10.0",
        "spdlog/1.8.5",
        "stb/20200203",
        "entt/3.5.2",
        # "cereal/1.3.0",

        "imgui/docking@vendor/stable",  # Installed with vendor/imgui.py
        "filament/1.9.21@vendor/stable",  # Installed with vendor/filament.py
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
