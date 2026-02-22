

# XYZLabs
==========================

A hardware accelerated UI framework wrapping [Dear ImGui](https://github.com/ocornut/imgui).
Still a hobby project. Use with caution. Be happy to extend.

## Features

- Create GPU rendered windows and widgets
- Event loop
- ObjectStore for things like datasets, analysis results, ...
- Simple thread safe task system for one shot and periodic tasks
- Settings system, that enables editing, storage, serialization, and loading of both basic and custom setting types
- Optional ViewPortWidget that allows OpenGL rendering.


## Build



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
| WITH_3D  | Build xyzlabs3d library. Requires [Magnum](https://github.com/mosra/magnum) installed | OFF   |
| WITH_EXAMPLES  | Build examples | OFF   |
| WITH_TESTS  | Build tests. Requires [GTest](https://github.com/google/googletest) installed | OFF   |
