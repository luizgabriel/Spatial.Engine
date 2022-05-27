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
        "skip_samples": [True, False],
        "skip_sdl2": [True, False],
        "enable_lto": [True, False],
        "use_swiftshader": [True, False],
        "build_filamat": [True, False],
        "supports_opengl": [True, False],
        "supports_metal": [True, False],
        "supports_vulkan": [True, False],
    }

    default_options = {
        "skip_samples": True,
        "skip_sdl2": True,
        "enable_lto": True,
        "use_swiftshader": False,
        "build_filamat": True,
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
        cmake.definitions["FILAMENT_SKIP_SAMPLES"] = self.options.get_safe("skip_samples", False)
        cmake.definitions["FILAMENT_ENABLE_LTO"] = self.options.get_safe("enable_lto", False)
        cmake.definitions["FILAMENT_SKIP_SDL2"] = self.options.get_safe("skip_sdl2", False)
        cmake.definitions["FILAMENT_SUPPORTS_OPENGL"] = self.options.get_safe("supports_opengl", False)
        cmake.definitions["FILAMENT_SUPPORTS_METAL"] = self.options.get_safe("supports_metal", False)
        cmake.definitions["FILAMENT_SUPPORTS_VULKAN"] = self.options.get_safe("supports_vulkan", False)
        cmake.definitions["FILAMENT_BUILD_FILAMAT"] = self.options.get_safe("build_filamat", False)

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

        libs = tools.collect_libs(self)
        if not self.options.get_safe("supports_vulkan", False):
            vulkan_libs = ["MoltenVK", "vulkan.1"]
            for vulkan_lib in vulkan_libs:
                libs = [lib for lib in libs if vulkan_lib not in lib]

        self.cpp_info.libs = libs

