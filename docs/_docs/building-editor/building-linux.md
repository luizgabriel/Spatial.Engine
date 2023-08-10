---
title: Linux
layout: default
nav_order: 3
parent: Building the Editor
---

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

Now, go to [Building the Engine](#building-the-engine) topic.

### Windows

Install all these tools:

- [CMake](https://cmake.org/download/)
- [Conan](https://conan.io/downloads.html)
- [Visual Studio 2022 Community](https://visualstudio.microsoft.com/pt-br/thank-you-downloading-visual-studio/?sku=Community&channel=Release&version=VS2022&source=VSLandingPage&cid=2030&passive=false)

## Configuring Conan Profile

You must create a default conan profile:

```sh
conan profile detect
```

> **V8 Library Requirement:**
>
> You must install "Windows 10 SDK version 10.0.19041.0" including the "Debugging Tools for Windows"
> feature [(Choco Package)](https://community.chocolatey.org/packages/windows-sdk-10-version-2004-windbg).
>
> > If you already the SDK installed, here
> >
> > is [how to enable to "Debugging Tools for Windows"](https://docs.microsoft.com/en-us/windows-hardware/drivers/debugger/debugger-download-tools#:~:text=If%20the%20Windows%20SDK%20is,add%20Debugging%20Tools%20for%20Windows.)

Now, go to [Building the Engine](#building-the-engine) topic.

### Linux

After, install [CMake](https://cmake.org/install/).

```sh
# Install some build dependencies
sudo apt-get install ninja-build build-essential mesa-common-dev libxi-dev libxxf86vm-dev

# Install LLVM Clang (Because it's required to build the filament library)
sudo apt-get install clang libc++-dev libc++abi-dev

# Install conan
pip install conan
```

Now, go to [Building the Engine](#building-the-engine) topic.

## Building the Engine

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
