---
title: MacOS
layout: default
nav_order: 1
parent: Building the Editor
---

## Installing Requirements

First of all. Make sure your graphics card supports OpenGL 4.1+.

- [MacOS OpenGL Version Support](https://support.apple.com/HT202823)

Now, install all these tools using [Homebrew](https://brew.sh/):

```sh
brew update
brew install cmake conan llvm ninja
xcode-select --install
```

## Configuring Conan Profile

You must create a default conan profile:

```sh
conan profile detect
```

This will create a file in: `~/.conan2/profiles/default`
Change profile might look like this:

```
[settings]
arch=x86_64
build_type=Release
compiler=msvc
compiler.cppstd=17
compiler.runtime=dynamic
compiler.version=193
os=Windows
```

## Cloning the Engine Respository

First, let's clone the repository:

```sh
git clone https://github.com/luizgabriel/Spatial.Engine.git
cd Spatial.Engine
```

## Building the Engine from the Command Line

Now, inside the project's folder. Run this script install all required conan dependencies which are not found by conan
by default.

```
conan install . --build=missing -s build_type=Debug
cmake --preset conan-debug
cmake --build . --preset conan-debug
```

## Running the Engine Editor

```
cmake --build . --preset conan-debug --package Spatial.Game
```

> The first time you run takes a lot of time to download and build the external dependencies.
