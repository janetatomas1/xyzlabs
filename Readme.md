

# XYZLabs
==========================

A hardware accelerated UI framework wrapping [Dear ImGui](https://github.com/ocornut/imgui).
Still a hobby project. Use with caution. Be happy to extend.

## Features

- Currenly only supporting GLFW and OpenGL
- Create GPU rendered windows and widgets
- Event loop
- ObjectStore for things like datasets, analysis results, ...
- Simple thread safe task system for one shot and periodic tasks
- Settings system, that enables editing, storage, serialization, and loading of both basic and custom setting types
- Optional ViewPortWidget that allows OpenGL rendering.


## Build
```
git clone https://github.com/janetatomas1/xyzlabs
cd xyzlabs
mkdir build
cmake -B build -S .
cmake --build build
```


## Project structure

- [Headers](include)
- [Sources](src)
- [Examples](examples)  
- [Tests](tests)
- [CMake scripts](cmake)
- [Utilities scripts](scripts)
- [Changelog](Changelog.md)

## CMake parameters

| Parameter name | Description | Deault value |
|----------|----------|----------|
| CMAKE_TOOLCHAIN_FILE | Path to toolchain file containing all the dependencies | No default value - must be entered |
| WITH_3D  | Build xyzlabs3d library. Requires [Magnum](https://github.com/mosra/magnum) installed | OFF   |
| WITH_EXAMPLES  | Build examples | OFF   |
| WITH_TESTS  | Build tests. Requires [GTest](https://github.com/google/googletest) installed | OFF   |

## Dependencies
- [glfw](https://github.com/glfw/glfw) for window managment
- [glew](https://github.com/nigels-com/glew) for OpenGL loading
- [Dear ImGui](https://github.com/ocornut/imgui) for UI
- [spdlog](https://github.com/gabime/spdlog) for string formatting
- [GTest](https://github.com/google/googletest) for unit testing
- [CMake](https://cmake.org/) for build system

## Platform support

| Parameter | Value |
|----------|----------|
| arch | x86_64 |
| OS | Window, Linux |
| Compiler | G++, Clang, MSVC |

Tested on Windows 11, 10 and Ubuntu 22.04, 24.04 (Gnome).
