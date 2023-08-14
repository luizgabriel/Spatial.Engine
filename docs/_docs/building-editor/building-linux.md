---
title: Linux
layout: default
nav_order: 3
parent: Building the Editor
---

## Installing Requirements

First, you need to install cmake:

- [CMake](https://cmake.org/download/)

```
sudo snap install cmake --classic
```

### Linux

Then you'll need to install a bunch of system dependencies:

```sh
# Install some build dependencies
sudo apt-get install ninja-build build-essential

# Install LLVM Clang (Because it's required to build the filament library)
sudo apt-get install clang libc++-dev libc++abi-dev

# Install X11 Core Libraries
sudo apt-get install libglu1-mesa-dev libxi-dev libxcomposite-dev libxxf86vm-dev libx11-dev libx11-xcb-dev libfontenc-dev libice-dev libsm-dev libxau-dev

# Install X11 Additional Libraries
sudo apt-get install libxaw7-dev libxcursor-dev libxdamage-dev libxinerama-dev libxmuu-dev libxrandr-dev libxres-dev libxss-dev libxtst-dev libxv-dev libxvmc-dev

# Install XCB Libraries
sudo apt-get install libxcb-render-util0-dev libxcb-xkb-dev libxcb-icccm4-dev libxcb-image0-dev libxcb-keysyms1-dev libxcb-randr0-dev libxcb-shape0-dev libxcb-sync-dev libxcb-xfixes0-dev libxcb-xinerama0-dev libxcb-dri3-dev libxcb-cursor-dev libxcb-util-dev
```

Use `pip` to install the [Conan Package Manager](https://conan.io/):

```
pip install conan
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
compiler=clang
compiler.cppstd=20
compiler.libcxx=libc++
compiler.version=10
os=Linux
```

## Building the Engine

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
