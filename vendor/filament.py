from conans import ConanFile, CMake, tools


class FilamentConan(ConanFile):
    name = "filament"
    version = "1.22.2"
    license = "Apache License 2.0"
    homepage = "https://github.com/google/filament"
    url = "https://github.com/luizgabriel/conan-filament"
    description = "Filament is a real-time physically based rendering engine for Android, iOS, Windows, Linux, " \
                  "macOS and WASM/WebGL "
    topics = ("graphics", "3d", "filament", "google")
    settings = ("os", "compiler", "build_type", "arch")
    revision_mode = "hash"
    generators = "cmake"
    build_requires = [
        "cmake/3.19.8",
        "ninja/1.10.0"
    ]

    options = {
        "enable_lto": [True, False],
        "skip_samples": [True, False],
        "skip_sdl2": [True, False],
        "build_filamat": [True, False],
        "use_swiftshader": [True, False],
        "supports_opengl": [True, False],
        "supports_metal": [True, False],
        "supports_vulkan": [True, False],
    }

    default_options = {
        "enable_lto": True,
        "skip_samples": True,
        "skip_sdl2": True,
        "build_filamat": True,
        "use_swiftshader": False,
        "supports_opengl": True,
        "supports_metal": False,
        "supports_vulkan": False,
    }

    def source(self):
        git = tools.Git(folder="filament")
        git.clone("https://github.com/google/filament.git", "v" + self.version, shallow=True)

        tools.replace_in_file("filament/CMakeLists.txt", "project(TNT)",
                              '''project(TNT)
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()
''')

    def _configure_cmake(self):
        cmake = CMake(self)
        cmake.definitions["FILAMENT_ENABLE_LTO"] = self.options["enable_lto"]
        cmake.definitions["FILAMENT_SKIP_SAMPLES"] = self.options["skip_samples"]
        cmake.definitions["FILAMENT_SKIP_SDL2"] = self.options["skip_sdl2"]
        cmake.definitions["FILAMENT_SUPPORTS_OPENGL"] = self.options["supports_opengl"]
        cmake.definitions["FILAMENT_SUPPORTS_METAL"] = self.options["supports_metal"]
        cmake.definitions["FILAMENT_SUPPORTS_VULKAN"] = self.options["supports_vulkan"]
        cmake.definitions["FILAMENT_BUILD_FILAMAT"] = self.options["build_filamat"]

        cmake.configure(source_dir="filament")

        return cmake

    def build(self):
        cmake = self._configure_cmake()
        cmake.build()

    def package(self):
        cmake = self._configure_cmake()
        cmake.install()

    def package_info(self):
        self.cpp_info.libdirs = ["lib/x86_64", "lib/arm64"]
        self.cpp_info.libs = tools.collect_libs(self)