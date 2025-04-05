
from conan import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain, CMakeDeps

class ContiniumWave(ConanFile):
    name = "continium-wave"
    version = "0.0"
    license = "Proprietary"
    author = "Tomas Janeta tomas.janeta@continiumtech.com>"
    url = "http://192.168.0.101:3000/tomas.janeta/continium-wave"
    topics = ("ADC", "fft", "analysis")
    settings = "os", "compiler", "build_type", "arch"
    exports_sources = "src/*", "include/*"
    requires = [
        "glfw/3.4",
        "spdlog/1.15.1"
    ]

    def generate(self):
        tc = CMakeToolchain(self)
        tc.generate()
        deps = CMakeDeps(self)
        deps.generate()

    def source(self):
        pass

    def build(self):
        pass

    def package(self):
        pass

    def package_info(self):
        self.cpp_info.libs = ["continiumwave"]
