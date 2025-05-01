

# XYZLabs

==========================

Hobby project for physics simulation, numerical methods, e.t.c.

==========================

## Download

To download the project run:

`git clone --recursive git@github.com:janetatomas1/xyzlabs.git`

This also downloads the dependencies that are located in the submodules directory (ImGui, ImPlot).

===========================

## Build

To build the project install
- [cmake](https://cmake.org/)
- [conan](https://docs.conan.io/2/)
- [gcc](https://gcc.gnu.org/)
- [clang](https://clang.llvm.org)

Run from the project root directory

```
./deps/install.sh
mkdir build
cd build
cmake ..
make -j8
./xyzlabs
```

