---
title: External Dependencies
layout: default
nav_order: 2
---

## Table of contents
{: .no_toc .text-delta }

1. TOC
{:toc}

## External dependencies
We all know that most of C++ projects try to avoid external dependencies due to the exponential increase of complexity added to the build configuration step. Although, it was necessary to not start from completely scratch, because that are plenty of very good libraries out there and we're not here to reinvent the wheel, just to improve it. 

### Necessary tools
So, to be able to simplify the process when we're integrating a new external library, I chose to use [Conan](https://conan.io) as our main package management tool. And for simplifying the build and configuration step I chose [CMake](https://cmake.org).

### Rendering Engine
Some game engines implement their own rendering library, but as I said, there are very powerful, well-tested and open-source solutions out there. I chose google's [filament](https://github.com/google/filament) rendering engine. You can click and see for yourself all their documentation. They did a pretty good job [documentation](https://google.github.io/filament/Filament.html) and I recommend you to read it all as a homework.

### Other external libraries
The other external libraries are described in the [`conanfile.txt`](https://github.com/luizgabriel/Spatial.Engine/blob/master/conanfile.txt).

### Backend
The supported backends are the same that the filament engine supports, so I'll just copy and paste then here:
- **OpenGL 4.1+ for Linux, macOS and Windows**
- OpenGL ES 3.0+ for Android and iOS
- Metal for macOS and iOS
- Vulkan 1.0 for Android, Linux, macOS, and Windows
- WebGL 2.0 for all platforms

The only platform that I succeeded testing was the desktop ones (Linux, Windows and MacOS) with OpenGL 4.1+. So feel free to try to compile the engine to other supported ones.