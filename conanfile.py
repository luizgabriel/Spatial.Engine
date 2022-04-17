from conans import ConanFile, CMake, tools


class SpatialEngineConan(ConanFile):
    name = "spatial"
    version = "1.3.7"
    license = "Apache 2.0"
    url = "https://github.com/luizgabriel/Spatial.Engine"
    description = "Spatial is a cross-platform c++ game engine created on top off google's [filament](" \
                  "https://github.com/google/filament) rendering engine. This projects uses C++17 and modern cmake " \
                  "features. "
    settings = "os", "compiler", "build_type", "arch"
    options = {"editor": [True, False]}
    default_options = {"editor": True}
    generators = "cmake"
    build_requires = "cmake/3.19.8"
    exports_sources = "*"
    requires = [
        "boost/1.77.0",
        "argh/1.3.1",
        "glfw/3.3.4",
        "gtest/1.10.0",
        "spdlog/1.8.5",
        "stb/20200203",
        "entt/3.7.0",
        "cereal/1.3.0",
        "magic_enum/0.7.3",
        "tl-expected/20190710",
        "assimp/5.1.0",

        # Remember to run 'setup.py' to make these dependencies available
        "imgui/docking@spatial/stable",    # Installed with vendor/imgui.py
        "filament/1.18.0@spatial/stable",  # Installed with vendor/filament.py
        "v8/10.1.69@spatial/stable",        # Installed with vendor/v8.py
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
        self.cpp_info.libs = tools.collect_libs(self)
