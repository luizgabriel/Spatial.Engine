---
title: Generate Doxygen Documentation
layout: default
nav_order: 2
---

## Generate Doxygen Documentation

Make sure to install [Doxygen](https://www.doxygen.nl/) and [Graphviz](https://graphviz.org/):

```sh
brew install doxygen graphviz
```

Build the project with the docs:
```sh
conan install . --build=missing -s build_type=Debug
cmake --preset conan-debug -DSPATIAL_ENABLE_DOXYGEN=ON
cmake --build . --preset conan-debug --target Spatial.Docs
```

Run the documentation web server:
```
python3 -m http.server --directory ./build/Debug/html
```
