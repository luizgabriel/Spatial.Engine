from conans import ConanFile, CMake, tools


class FilamentConan(ConanFile):
    name = "filament"
    version = "1.17.0"
    license = "Apache License 2.0"
    homepage = "https://github.com/google/filament"
    url = "https://github.com/luizgabriel/conan-filament"
    description = "Filament is a real-time physically based rendering engine for Android, iOS, Windows, Linux, " \
                  "macOS and WASM/WebGL "
    topics = ("graphics", "3d", "filament", "google")
    settings = ("os", "compiler", "build_type", "arch")
    generators = "cmake"
    build_requires = "cmake/3.19.8"
    options = {
        "enable_java": [True, False],
        "skip_samples": [True, False],
        "enable_lto": [True, False],
        "build_filamat": [True, False],
        "use_swiftshader": [True, False],
        "supports_opengl": [True, False],
        "supports_metal": [True, False],
        "supports_vulkan": [True, False],
        "generate_js_docs": [True, False],
    }
    default_options = {
        "enable_java": False,
        "skip_samples": True,
        "enable_lto": False,
        "build_filamat": True,
        "use_swiftshader": False,
        "supports_opengl": True,
        "supports_metal": False,
        "supports_vulkan": False,
        "generate_js_docs": False,
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

        cmake.definitions["FILAMENT_ENABLE_JAVA"] = self.options.get_safe("enable_java", False)
        cmake.definitions["FILAMENT_SKIP_SAMPLES"] = self.options.get_safe("skip_samples", False)
        cmake.definitions["FILAMENT_ENABLE_LTO"] = self.options.get_safe("enable_lto", False)
        cmake.definitions["FILAMENT_BUILD_FILAMAT"] = self.options.get_safe("build_filamat", False)
        cmake.definitions["FILAMENT_USE_SWIFTSHADER"] = self.options.get_safe("use_swiftshader", False)
        cmake.definitions["FILAMENT_SUPPORTS_OPENGL"] = self.options.get_safe("supports_opengl", False)
        cmake.definitions["FILAMENT_SUPPORTS_METAL"] = self.options.get_safe("supports_metal", False)
        cmake.definitions["FILAMENT_SUPPORTS_VULKAN"] = self.options.get_safe("supports_vulkan", False)
        cmake.definitions["FILAMENT_GENERATE_JS_DOCS"] = self.options.get_safe("generate_js_docs", False)

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
        self.cpp_info.libs = [
            "backend", "filaflat", "geometry", "matdbg",
            "bluegl", "filamat", "gltfio", "meshoptimizer",
            "camutils", "filamat_lite", "gltfio_core", "shaders",
            "dracodec", "filament", "ibl", "smol-v",
            "filabridge", "filameshio", "image", "utils"
        ]
