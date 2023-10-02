---
title: MacOS
layout: default
nav_order: 1
parent: Building the Editor
---

## Installing Requirements

Install all these tools using [Homebrew](https://brew.sh/):

```sh
brew update
brew install cmake conan llvm ninja
xcode-select --install
```

## Configuring a Conan Profile

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
compiler=apple-clang
compiler.cppstd=20
compiler.libcxx=libc++
compiler.version=13
os=Macos
```

For Apple Silicon chips (M1, M2, etc...):

```
[settings]
arch=armv8
build_type=Release
compiler=clang
compiler.cppstd=20
compiler.libcxx=libc++
compiler.version=13
os=Macos
```

## Cloning the Engine Respository

First, let's clone the repository:

```sh
git clone https://github.com/luizgabriel/Spatial.Engine.git
cd Spatial.Engine
```

## Building the Engine from the Command Line

Now, inside the project's folder.

```
conan install . --build=missing
cmake --preset conan-release
cmake --build . --preset conan-release
```

## Running the Engine Editor

```
cmake --build . --preset conan-release --target Spatial.Game
```

> The first time you run takes a lot of time to download and build the external dependencies.
