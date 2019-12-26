![Spatial Engine](/Spatial.Sandbox/assets/textures/spatial_engine_logo.png)


# Spatial Engine

Spatial is a cross-platform c++ game engine created on top off google's [filament](https://github.com/google/filament) rendering engine. This projects uses C++17 and modern cmake features.

## Dependencies

Most of the dependencies are handled by the [Conan Package Manager](https://conan.io).

When you run the cmake configure command it automagically download its dependencies and builds then for you. 

Only the SDL2 dependency is necessary to installed manually. If you are on a gnu based operating system, you're one comand line of distance for installing the SDL2. But if you're on windows, I recomend you to install [VCPKG](https://github.com/microsoft/vcpkg) to make your life easier.

```shell
# Create the output directory
mkdir out/build && cd out/build

# Generate the configuration files (windows with vcpkg example)
cmake ../.. -G "Visual Studio 16 2019" -A x64 -DCMAKE_BUILD_TYPE=Debug -T LLVM -DCMAKE_TOOLCHAIN_FILE="%VCKPG_DIR%/scripts/buildsystems/vcpkg.cmake"

# Build the engine
cmake --build . --config Debug --target Spatial.Sandbox
```

### Conan

Cmake will automatically install the conan dependencies for you once you configured the profile correctly. This project requires Clang to be built. So you need to create a `clang` profile like this:

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

Notice that the `compiler.toolset=LLVM` is necessary to inform Visual Studio to build the project using the `clang-cl` compiler. To use the [LLVM](https://marketplace.visualstudio.com/items?itemName=LLVMExtensions.llvm-toolchain) toolset you need to download the extension.

### Filament Engine

The filament engine has no official conan package script provided, so you'll need to clone and install mine.

```
git clone https://github.com/luizgabriel/conan-filament.git
cd conan-filament
conan create . google/stable -pr clang
```