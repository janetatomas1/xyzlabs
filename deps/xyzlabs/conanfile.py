
from conan import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain, CMakeDeps

class XYZLabs(ConanFile):
    name = "xyzlabs"
    version = "0.0"
    license = "Proprietary"
    author = "Tomas Janeta tomas.janeta@proton.me>"
    url = "https://github.com/janetatomas1/xyzlabs"
    settings = "os", "compiler", "build_type", "arch"
    exports_sources = "src/*", "include/*"
    requires = [
        "glfw/3.4",
        "spdlog/1.15.1",
        "boost/1.88.0"
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
        self.cpp_info.libs = ["xyzlabs"]
