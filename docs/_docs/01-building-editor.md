---
title: Building the Editor
layout: default
nav_order: 1
---

## Table of contents
{: .no_toc .text-delta }

1. TOC
{:toc}



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
- [Visual Studio 2019 Community](https://my.visualstudio.com/Downloads?q=visual%20studio%20community%202019&wt.mc_id=o~msft~vscom~older-downloads)
> *V8 Library Requirement:*
> You must install "Windows 10 SDK version 10.0.19041.0" including the ["Debugging Tools for Windows"](https://docs.microsoft.com/en-us/windows-hardware/drivers/debugger/debugger-download-tools#:~:text=If%20the%20Windows%20SDK%20is,add%20Debugging%20Tools%20for%20Windows.) feature.

> You might also have problems with your antivirus when building Boost and its dependencies. Make sure to disable it if you have problems.

This is the recommend conan profile to compile on Windows (`~/.conan/profiles/default`):
> If there's no `profiles/default` file, run: `conan profile new default --detect`

```toml
[settings]
os=Windows
os_build=Windows
arch=x86_64
arch_build=x86_64
build_type=Release

compiler=Visual Studio
compiler.version=16
compiler.runtime=MT
```

The most important setting is this `compiler.runtime=MT` (This is required for the `filament` library to build properly).

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

# Forces Conan to use the Ninja generator (Because it's required to build the filament library)
conan config set general.cmake_generator=Ninja
```

Configure your conan profile:
```toml
[settings]
compiler=clang
compiler.version=10
compiler.libcxx=libc++
build_type=Release

[env]
CC=/usr/bin/clang
CXX=/usr/bin/clang++
```

Now, go to [Building the Engine](#building-the-engine) topic.

## Building the Engine

First, let's clone the repository:
```sh
git clone https://github.com/luizgabriel/Spatial.Engine.git
cd Spatial.Engine
```

Now, inside the project's folder. Run this script install all required conan dependencies which are not found by conan by default.
```
python ./cli.py run-editor --build-type=Release --conan-profile=default
```

> The first time you run takes a lot of time to download and build the external dependencies.