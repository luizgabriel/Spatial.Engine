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

## Using Chocolatey

You can also install these dependencies using the [Chocolatey](https://chocolatey.org/install)
> Make sure to run this command using PowerShell (in Administrator mode)
```ps
choco install python
choco install ninja
choco install cmake --installargs 'ADD_CMAKE_TO_PATH=System'
choco install conan --version=1.58.0
```

## Configuring Conan Profile

You must create a default conan profile:
```sh
conan profile new default --detect
```

This will create a file in: `C:/Users/{YourUser}/.conan/profiles/default`
Change profile might look like this:
```
[settings]
arch=x86_64
build_type=Release
compiler=msvc
compiler.cppstd=14
compiler.runtime=dynamic
compiler.version=193
os=Windows
```

## Install V8 Library Requirement

You must install "Windows 10 SDK version 10.0.19041.0" including the "Debugging Tools for Windows" feature [(Choco Package)](https://community.chocolatey.org/packages/windows-sdk-10-version-2004-windbg).
```ps
choco install windows-sdk-10-version-2004-windbg
```

If you already the SDK installed, here is [how to enable to "Debugging Tools for Windows"](https://docs.microsoft.com/en-us/windows-hardware/drivers/debugger/debugger-download-tools#:~:text=If%20the%20Windows%20SDK%20is,add%20Debugging%20Tools%20for%20Windows.)

Now, go to [Building the Engine](#building-the-engine) topic. 

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
python ./cli.py run-editor --profile=default
```

> The first time you run takes a lot of time to download and build the external dependencies.
