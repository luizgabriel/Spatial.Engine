from conans import ConanFile, CMake, tools


class SpatialEngineConan(ConanFile):
    name = "spatial"
    version = "1.3.10"
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
        "boost/1.78.0",
        "argh/1.3.1",
        "glfw/3.3.7",
        "gtest/1.11.0",
        "spdlog/1.10.0",
        "stb/20200203",
        "entt/3.10.1",
        "cereal/1.3.1",
        "magic_enum/0.8.0",
        "glm/0.9.9.8",
        "imgui/cci.20220207+1.87.docking",

        # "assimp/5.1.0",

        # Remember to run 'setup.py' to make these dependencies available
        "filament/1.23.2@spatial/stable",  # Installed with vendor/filament.py
        "v8/10.4.66@spatial/stable",       # Installed with vendor/v8.py
    ]

    def imports(self):
        self.copy("*", dst="bin", src="bin")
        self.copy("*.dll", dst="lib", src="lib", keep_path=False)
        self.copy("*.dylib*", dst="lib", src="lib", keep_path=False)

    def _cmake(self):
        cmake = CMake(self)
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
