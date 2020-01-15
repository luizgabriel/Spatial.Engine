![Spatial Engine](/Spatial.Sandbox/assets/textures/spatial_engine_logo.png)


# Spatial Engine

Spatial is a cross-platform c++ game engine created on top off google's [filament](https://github.com/google/filament) rendering engine. This projects uses C++17 and modern cmake features.

## Building

Most of the dependencies are handled by the [Conan Package Manager](https://conan.io).

### Configuring the Conan Profile

Cmake will automatically install the conan dependencies for you (once you configured the profile correctly). 
This project requires [LLVM](http://releases.llvm.org/download.html) to be built. So you need to create a `clang` profile like this:

```
[settings]
os=Windows
os_build=Windows
arch=x86_64
arch_build=x86_64

compiler=Visual Studio
compiler.version=16
compiler.toolset=LLVM
compiler.runtime=MTd
compiler.cppstd=17

build_type=Debug
```

Notice that the `compiler.toolset=LLVM` is necessary to inform Visual Studio to build the project using the `clang-cl` compiler. To use the LLVM toolset you need to download [this extension](https://marketplace.visualstudio.com/items?itemName=LLVMExtensions.llvm-toolchain).

### Installing the Dependencies

In a bash terminal (on windows you may use git bash or wls bash), run:

```
./Scripts/SetupDependencies.sh
```

### Cmake

This project is configured using [Cmake](https://cmake.org/download/).
When you run the cmake configure command, it automagically downloads its dependencies and builds then for you (when everything else is well configured).

```shell
# Create the output directory
mkdir out/build && cd out/build

# Generate the configuration files (windows with vcpkg example)
cmake ../.. -G "Visual Studio 16 2019" -A x64 -T LLVM \
      -DCMAKE_BUILD_TYPE=Debug \
      -DCMAKE_TOOLCHAIN_FILE="%VCKPG_DIR%/scripts/buildsystems/vcpkg.cmake" # You can ignore this line if your not using vcpkg to install sdl2

# Build the engine
cmake --build . --config Debug --target Spatial.Sandbox
```

> Remember to change `%VCKPG_DIR%` to your respective VCPKG directory.
