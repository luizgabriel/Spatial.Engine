---
title: Windows
layout: default
nav_order: 2
parent: Building the Editor
---

## Installing Requirements

Install all these tools:

- [CMake](https://cmake.org/download/)
- [Conan](https://conan.io/downloads.html)
- [Visual Studio 2022 Community](https://visualstudio.microsoft.com/pt-br/thank-you-downloading-visual-studio/?sku=Community&channel=Release&version=VS2022&source=VSLandingPage&cid=2030&passive=false)
- [Python 3](https://www.python.org/downloads/)

### Using Chocolatey

You can also install these dependencies using the [Chocolatey](https://chocolatey.org/install)

> Make sure to run this command using PowerShell (in Administrator mode)

```ps
choco install python
choco install ninja
choco install cmake --installargs 'ADD_CMAKE_TO_PATH=System'
choco install conan
```

## Configuring a Conan Profile

You must create a default conan profile:

```sh
conan profile detect
```

This will create a file in: `C:/Users/{YourUser}/.conan2/profiles/default`
Change profile might look like this:

```
[settings]
arch=x86_64
build_type=Release
compiler=msvc
compiler.cppstd=20
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

Now, inside the project's folder.

```
conan install . --build=missing
cmake --preset conan-default
cmake --build . --preset conan-default
```

## Running the Engine Editor

```
cmake --build . --preset conan-default --package Spatial.Game
```

> The first time you run takes a lot of time to download and build the external dependencies.
