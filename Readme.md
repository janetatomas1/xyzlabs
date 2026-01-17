

# XYZLabs

==========================

[Dear ImGui](https://github.com/ocornut/imgui)

==========================

## Download

To download the project run:

`git clone --recursive git@github.com:janetatomas1/xyzlabs.git`

===========================

## Build

To build the project install
- [cmake](https://cmake.org/)
- [conan](https://docs.conan.io/2/)
- [gcc](https://gcc.gnu.org/) or [clang](https://clang.llvm.org)

Run from the project root directory

```
mkdir build && cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE_=/path/to/conan/toolchaine/file
cmake --build .
```
