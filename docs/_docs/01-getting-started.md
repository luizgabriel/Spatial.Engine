---
title: Getting Started
layout: default
nav_order: 1
---

## Table of contents
{: .no_toc .text-delta }

1. TOC
{:toc}

## Why do we need another game engine?

Maybe you're asking yourself: "Wait a minute. Why do we need another game engine?" - and you're right. 
There are plenty of well known game engines like Unity, Unreal Engine, some open-source like Panda 3D, Xenko and Lumberyard Engine, etc. 
So, if you REALLY want to make a game, go straight away with those engines. 
But, if you need a simple visualization tool with game engine like tools and functions, I think that the Spatial Engine is a nice solution to consider.

Programming a game engine is hard and the necessary skill are completely different from creating a game. 
The Spatial Engine is still pretty simple and also really far from complete, but it already do some neat things.

## The Project Structure
Now, let's dive into the usage details of the Spatial Engine.

The project is organized by grouping some bigger modules:
```
Spatial.Engine
│   README.md
└── Spatial.Core (Contains everything the engine offers for you)
└── Spatial.UI (The user interface library)
└── Spatial.Editor (The engine scene editor built using with all the Core tools)
└── Spatial.Res (A C++ code generator for embedding resource files inside the Spatial.Editor binary)
└── Spatial.Game (An example game with some assets for you to play with the engine editor)
```

You can take a look at the `Spatial.Core` folder to understand how the darker details have been implemented.

## First time here, buddy?

So, the first step to get used to this engine is to build it from source. 
We'll need to install the correct compiler, the package manager, run some installation scripts, wait for all required
dependencies to build and finally you'll be able see something on your screen. 
You might take some time in this step if you're not used to all complexities of a C++ project.

When you get used with the engine, we can bootstrap a simple game project using the engine's core library.

## Let us known if you're having any trouble
As a reminder, this engine is improving constantly and changes may happen which could deprecate this tutorial steps. 
So, if you have any problems, feel free to leave an issue on Github.

[Create a Github Issue](https://github.com/luizgabriel/Spatial.Engine/issues/new){: .btn }

## Installing Requirements

First of all. Make sure your graphics card supports OpenGL 4.1+.
- [MacOS OpenGL Version Support](https://support.apple.com/HT202823)

### MacOS
Now, install all these tools:
```sh
brew update
brew install cmake conan
xcode-select --install
```

### Windows
Install all these tools:
- [CMake](https://cmake.org/download/)
- [Conan](https://conan.io/downloads.html)
- [Visual Studio 2019 Community](https://my.visualstudio.com/Downloads?q=visual%20studio%20community%202019&wt.mc_id=o~msft~vscom~older-downloads)

This is the recommend conan profile to compile on Windows (`~/.conan/profiles/default`):
> If there's no `profiles/default` file, run: `conan profile new default --detect`

```toml
[settings]
os=Windows
os_build=Windows
arch=x86_64
arch_build=x86_64

compiler=Visual Studio
compiler.version=16
compiler.runtime=MTd
```

The most important setting is this `compiler.runtime=MTd` (This is required for the `filament` library to build properly).

### Linux
After, install [CMake](https://cmake.org/install/).

```sh
# Install some build dependencies
sudo apt-get install ninja-build build-essential mesa-common-dev libxi-dev libxxf86vm-dev 

# Install LLVM Clang (Because it's required to build the filament library)
sudo apt-get install clang libc++-dev libc++abi-dev

# Install conan
pip install conan 

# Forces Conan to use the Ninja generator (Because it's required to build the filament library)
conan config set general.cmake_generator=Ninja
```

Configure your conan profile:
```toml
[settings]
compiler=clang
compiler.version=10
compiler.libcxx=libc++

[env]
CC=/usr/bin/clang
CXX=/usr/bin/clang++
```

## Building the Engine

First, let's clone the repository:
```sh
git clone https://github.com/luizgabriel/Spatial.Engine.git
cd Spatial.Engine
```

Now, inside the project's folder. Run this script install all required conan dependencies which are not found by conan by default.
```
python setup.py
```

### From the Command Line

> Supported Compilers:
> - LLVM Clang on Linux
> - Apple Clang on MacOS
> - Visual Studio 2019 on Windows

If you're wishing to build from the command line, you'll just need to run these commands:

```sh
# Create a build directory
mkdir -p out/build/debug/ && cd out/build/debug

# Run the Cmake Configure Step
# Linux:    -G "Ninja"
cmake ../../.. -G "CodeBlocks - Unix Makefiles" \
    -DCMAKE_INSTALL_PREFIX=../../install \
    -DSPATIAL_BUILD_TESTS=OFF \
    -DCMAKE_BUILD_TYPE=Debug

# Run the Cmake Build Step
cmake --build . --target all
```

### With VSCode

Finally, open [Visual Studio Code](https://code.visualstudio.com/).
Install these extensions:
  - Install the [C++ Extension](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools)
  - Install the [CMake Extension](https://marketplace.visualstudio.com/items?itemName=twxs.cmake)
  - Install the [CMake Tools Extension](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cmake-tools)

Create a `.vscode/settings.json` file:
```json
{
    "cmake.buildDirectory": "${workspaceFolder}/out/build",
    "cmake.installPrefix": "${workspaceFolder}/out/install",
    "cmake.configureSettings": {
       "CMAKE_BUILD_TYPE": "${buildType}"
    }
}
```

Now, press `Ctrl + Shift + P` and type `CMake: Configure` then `CMake: Build`

### With Clion

Open the Preferences Panel:

![Preferences Panel Menu Item]({{site.baseurl}}/assets/images/clion-file-menu.png)

Go to `Build, Execution, Deployment` > `CMake`:
- On `Build Directory` put `out/build/debug`
- On `CMake Options` put `-DSPATIAL_BUILD_TESTS=ON -DCMAKE_INSTALL_PREFIX=../../install`
- On `Build ComponentType` put `Debug`
- On `Toolchain` put `System`

![Build, Execution, Deployment, CMake]({{site.baseurl}}/assets/images/clion-config.png)
