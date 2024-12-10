import os

from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMake, cmake_layout, CMakeDeps
from conan.tools.files import copy
from conan.tools.build import check_min_cppstd

class SpatialRecipe(ConanFile):
    name = "spatial"
    version = "1.3.21"
    package_type = "application"

    # Optional metadata
    license = "Apache 2.0"
    author = "Luiz Gabriel luizgabriel.info@gmail.com"
    url = "https://github.com/luizgabriel/Spatial.Engine"
    description = "Spatial is a cross-platform C++ game engine created on top off google's filament rendering engine."
    topics = ("cross-platform", "graphics", "engine", "glfw", "spatial", "conan", "cpp17", "3d", "filament", "entt")

    # Binary configuration
    settings = "os", "compiler", "build_type", "arch"

    # Sources are located in the same place as this recipe, copy them to the recipe
    exports_sources = "CMakeLists.txt", "CMake/*", "docs/*", "vendor/*", "Spatial.Core/*", "Spatial.Editor/*", "Spatial.Game/*", "Spatial.Graphics/*", \
        "Spatial.Res/*", "Spatial.UI/*"
    
    cpack_generators = {
        "Windows": "WIX",
        "Macos": "DragNDrop",
        "Linux": "DEB"
    }

    options = {
        "backend": ["opengl", "vulkan", "metal"],
    }

    def validate(self):
        check_min_cppstd(self, "20")

    def build_requirements(self):
        self.tool_requires("cmake/3.26.4")

    def configure(self):
        backend = self.options.get_safe("backend")
        if backend == None:
            if self.settings.os == "Macos":
                self.options.backend = "metal"
            else:
                self.options.backend = "opengl"

    def requirements(self):
        self.requires("boost/1.86.0", options={
            "without_locale": True,
            "without_stacktrace": True,
        })
        
        self.requires("argh/1.3.1")
        self.requires("glfw/3.3.8")
        self.requires("gtest/1.11.0")
        self.requires("spdlog/1.10.0")
        self.requires("stb/cci.20220909")
        self.requires("entt/3.14.0")
        self.requires("cereal/1.3.1")
        self.requires("magic_enum/0.8.0")
        self.requires("glm/0.9.9.8")
        self.requires("imgui/cci.20230105+1.89.2.docking")
        self.requires_local("filament/1.55.0", options={
            f"supports_{self.options.backend}": True,
        })

    def layout(self):
        cmake_layout(self)

    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()
        tc = CMakeToolchain(self)
        cpack_generator = self.cpack_generators.get(str(self.settings.os))
        if cpack_generator is not None:
            tc.variables["CPACK_GENERATOR"] = cpack_generator
        tc.variables["SPATIAL_GRAPHICS_BACKEND"] = self.options.backend
        tc.generate()

        for dep in self.dependencies.values():
            for f in dep.cpp_info.bindirs:
                copy(self, "*", f, self.generators_folder)

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()

    def requires_local(self, package, options=None):
        if options is None:
            options = {}

        name, version = package.split("/")
        recipes_path = os.path.join(self.folders.source, "vendor")
        self.run("conan export %s.py --user user --channel stable" % name, cwd=recipes_path)
        self.requires("%s/%s@user/stable" % (name, version), options=options)



    

    
