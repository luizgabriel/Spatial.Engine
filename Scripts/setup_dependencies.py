#!/usr/bin/python3

import os
import shutil
from platform import system


TMP_DIR = "__tmp"


def conan_repo(name, url):
    return os.system('conan remote add %s %s > /dev/null 2>&1' % (name, url))


def conan_export(user, package, export_name):
    print("Exporting %s" % package)
    package_dir = os.path.join(TMP_DIR, package)
    
    result = 0
    result += os.system("git clone https://github.com/%s/%s.git %s" % (user, package, package_dir))
    result += os.system("conan export %s %s" % (package_dir, export_name))

    return result


if __name__ == "__main__":
    os.makedirs(TMP_DIR)

    result = 0
    result += conan_export("luizgabriel", "conan-filament", "google/stable")
    result += conan_export("luizgabriel", "conan-imgui", "ocornut/testing")
    conan_repo("public", "https://api.bintray.com/conan/bincrafters/public-conan")

    if result == 0:
        print("Done! Successfully exported all extra CONAN packages")

    shutil.rmtree(TMP_DIR)

    if system() == 'Linux':
        print("\nDetected Linux Environment.")
        print("Please, install required dependencies with: ")
        print("> sudo apt-get install clang-9 libc++abi-dev ninja-build libsdl2-dev libxi-dev mesa-common-dev libgl1-mesa-dev")

    if system() == 'Windows':
        print("\nDetected Windows Environment.")
        print("Please, install required dependencies with vcpkg: ")
        print("> C://vcpkg/vcpkg.exe install sdl2")