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
The Spatial Engine stills pretty simple and also really far from complete, but it already do some neat things.

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
```

You can take a look at the `Spatial.Core` folder to understand how the darker details have been implemented. But for now we will understand how `Spatial.Editor` makes use of the engine's features.

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
- [Visual Studio 2019 Community](https://visualstudio.microsoft.com/pt-br/downloads/)

This is the recommend conan profile to compile on Windows (`~/.conan/profiles/default`):
```
[settings]
os=Windows
os_build=Windows
arch=x86_64
arch_build=x86_64

compiler=Visual Studio
compiler.version=16
compiler.runtime=MTd
```

### Linux
After, install [CMake](https://cmake.org/install/).

```sh
# Install some build dependencies
sudo apt-get install ninja-build build-essentials mesa-common-dev libxi-dev libxxf86vm-dev 

# Install conan
pip install conan 
```

## Building the Engine

This script install all required conan dependencies which are not found by conan by default.
```
python setup.py
```

### From the Command Line
```sh
# Creates a build directory
mkdir -p out/build && cd out/build

# Runs the Cmake Configure Step
cmake ../.. -G "CodeBlocks - Unix Makefiles" \
    -DCMAKE_INSTALL_PREFIX=../install \
    -DCMAKE_BUILD_TYPE=Debug

cmake --build . --target all
```

### With VSCode

Finally, open [Visual Studio Code](https://code.visualstudio.com/).
Install these extensions:
  - Install the [C++ Extension](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools)
  - Install the [CMake Extension](https://marketplace.visualstudio.com/items?itemName=twxs.cmake)
  - Install the [CMake Tools Extension](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cmake-tools)

Create a `.vscode/settings.yml` file:
```json
{
    "cmake.buildDirectory": "${workspaceFolder}/out/build",
    "cmake.installPrefix": "${workspaceFolder}/out/install"
}
```

Now, press `Ctrl + Shift + P` and type `CMake: Configure` then `CMake: Build`

### With Clion

Open the Preferences Panel:

![Preferences Panel Menu Item](/assets/clion-file-menu.png)

Go to `Build, Execution, Deployment` > `CMake`:
- On `Build Directory` put `out/build/debug`
- On `CMake Options` put `-DSPATIAL_BUILD_TESTS=ON -DCMAKE_INSTALL_PREFIX=../../install`
- On `Build Type` put `Debug`
- On `Toolchain` put `System`

![Build, Execution, Deployment, CMake](/assets/clion-config.png)


