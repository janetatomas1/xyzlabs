
rm third-party -rf

conan install deps/conanfiles/xyzlabs --build=missing --output-folder=./third-party/gcc --deployer-folder=third-party/gcc --deployer=full_deploy --profile:all=$(pwd)/profiles/gcc

conan install deps/conanfiles/xyzlabs --build=missing --output-folder=./third-party/clang --deployer-folder=third-party/clang --deployer=full_deploy --profile:all=$(pwd)/profiles/clang

rm -f CMakeUserPresets.json
