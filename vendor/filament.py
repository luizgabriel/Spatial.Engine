from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMake, cmake_layout, CMakeDeps
from conan.tools.scm import Git
from conan.tools.files import replace_in_file
from conan.tools.system.package_manager import Apt
from conan.tools.build import check_min_cppstd

class FilamentConan(ConanFile):
    name = "filament"
    version = "1.40.4"
    license = "Apache License 2.0"
    homepage = "https://github.com/google/filament"
    url = "https://github.com/luizgabriel/conan-filament"
    description = "Filament is a real-time physically based rendering engine for Android, iOS, Windows, Linux, " \
                  "macOS and WASM/WebGL "
    topics = ("graphics", "3d", "filament", "google")
    settings = "os", "compiler", "build_type", "arch"

    cmake_variables = [
        "FILAMENT_USE_EXTERNAL_GLES3",  # Experimental: Compile Filament against OpenGL ES 3
        "FILAMENT_USE_SWIFTSHADER",  # Compile Filament against SwiftShader
        "FILAMENT_ENABLE_LTO",  # Enable link-time optimizations if supported by the compiler
        "FILAMENT_SKIP_SAMPLES",  # Don't build samples
        "FILAMENT_SUPPORTS_XCB",  # Include XCB support in Linux builds
        "FILAMENT_SUPPORTS_XLIB",  # Include XLIB support in Linux builds
        "FILAMENT_SUPPORTS_EGL_ON_LINUX",  # Use EGL for OpenGL in Linux builds
        "FILAMENT_SUPPORTS_WAYLAND",  # Include Wayland support in Linux builds
        "FILAMENT_SKIP_SDL2",  # Skip dependencies of SDL2, and SDL2
        "FILAMENT_LINUX_IS_MOBILE",  # Treat Linux as Mobile
        "FILAMENT_ENABLE_ASAN_UBSAN",  # Enable Address and Undefined Behavior Sanitizers
        "FILAMENT_SUPPORTS_OPENGL",  # By default, build with support for OpenGL on all platforms.
        "FILAMENT_SUPPORTS_METAL",  # Build with Metal support on non-WebGL Apple platforms.
        "FILAMENT_SUPPORTS_VULKAN",
        # build with Vulkan support on desktop platforms, although clients must request to use it at run time.
        "FILAMENT_BUILD_FILAMAT",
        # Building filamat increases build times and isn't required for web, so turn it off by default.
        "FILAMENT_DISABLE_MATOPT",
        # Only optimize materials in Release mode (so error message lines match the source code)
    ]

    options = {
        "use_external_gles3": [True, False, None],
        "use_swiftshader": [True, False, None],
        "enable_lto": [True, False, None],
        "skip_samples": [True, False, None],
        "supports_xcb": [True, False, None],
        "supports_xlib": [True, False, None],
        "supports_egl_on_linux": [True, False, None],
        "supports_wayland": [True, False, None],
        "skip_sdl2": [True, False, None],
        "linux_is_mobile": [True, False, None],
        "enable_asan_ubsan": [True, False, None],
        "supports_opengl": [True, False, None],
        "supports_metal": [True, False, None],
        "supports_vulkan": [True, False, None],
        "build_filamat": [True, False, None],
        "disable_matopt": [True, False, None],
    }

    default_options = {
        "skip_samples": True,
        "skip_sdl2": True,
    }

    def validate(self):
        check_min_cppstd(self, "20")

    def system_requirements(self):
        # https://github.com/google/filament/blob/main/BUILDING.md#linux
        if self.settings.os == "Linux":
            apt = Apt(self)
            apt.install(["libglu1-mesa-dev", "libxi-dev", "libxcomposite-dev", "libxxf86vm-dev"])

    def source(self):
        git = Git(self)
        git.clone(url="https://github.com/google/filament.git", target=".")
        git.checkout("v" + self.version)

        replace_in_file(self,
                        file_path="filament/backend/CMakeLists.txt",
                        search="-Wdeprecated",
                        replace="-Wno-deprecated")

    def layout(self):
        cmake_layout(self)
        if str(self.settings.arch).startswith("arm"):
            self.cpp.package.libdirs = ["lib/arm64"]
        else:
            self.cpp.package.libdirs = ["lib/x86_64"]

        self.cpp.package.libs = [
            "backend", "filamat", "gltfio_core", "mikktspace", "viewer",
            "basis_transcoder", "filamat_lite", "ibl-lite", "shaders", "vkshaders",
            "camutils", "filament-iblprefilter", "ibl", "smol-v", "zstd",
            "civetweb", "filament", "image", "stb",
            "dracodec", "filameshio", "ktxreader", "uberarchive",
            "filabridge", "geometry", "matdbg", "uberzlib",
            "filaflat", "gltfio", "meshoptimizer", "utils",
        ]

    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()
        tc = CMakeToolchain(self)
        for variable in self.cmake_variables:
            value = self.options.get_safe(variable.replace("FILAMENT_", "").lower(), None)
            if value is not None:
                tc.variables[variable] = "ON" if value else "OFF"

        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()

    def build_requirements(self):
        self.tool_requires("cmake/3.22.6")
