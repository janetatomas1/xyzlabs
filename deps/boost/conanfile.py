
from conan import ConanFile
from conan.tools.files import get, copy
from conan.tools.build import build_jobs
import os


class Boost(ConanFile):
    name = "boost"
    version = "1.88.0"
    license = "Boost License"
    author = "tomas.janeta@proton.me>"
    directory = "boost_{}".format(version.replace(".", "_"))
    url = "https://archives.boost.io/release/{}/source/{}".format(version, directory)
    settings = "os", "compiler", "build_type", "arch"

    def source(self):
        get(self, f"{self.url}.tar.gz")

    def build(self):
        prefix = f"{self.build_folder}/release"
        os.mkdir(prefix)
        if self.settings.os == "Windows":
            self.run(
                "bootstrap.bat --with-libraries=all --with-toolset={}".format(self.settings.compiler),
                cwd=f"{self.build_folder}/{self.directory}"
            )
            self.run(
                    f"b2 --toolset={self.settings.compiler} --prefix={prefix} install --link=static --runtime-link=static",
                    cwd=f"{self.build_folder}/{self.directory}"
            )
        else:
            self.run(
                "./bootstrap.sh --with-libraries=all --with-toolset={}".format(self.settings.compiler),
                cwd=f"{self.build_folder}/{self.directory}"
            )
            self.run(
                f"./b2 --toolset={self.settings.compiler} --prefix={prefix} install --link=static --runtime-link=static",
                cwd=f"{self.build_folder}/{self.directory}"
            )

    def package(self):
        prefix = f"{self.build_folder}/release"
        copy(self, src=f"{prefix}/", dst=self.package_folder, pattern="include/*", keep_path=True)
        copy(self, src=f"{prefix}/", dst=self.package_folder, pattern="lib/*.lib", keep_path=True)

    def package_info(self):
        self.cpp_info.libs = ["boost"]
