#!python

import os
import shutil
from platform import system


CONAN_DIR = ".conan"
TMP_DIR = "%s/tmp" % CONAN_DIR
DEFAULT_PROFILE = "clang"


def run(command):
    print("\n\n > %s\n" % command)
    return os.system(command)


def conan_repo(name, url):
    return run('conan remote add %s %s' % (name, url))


def conan_install():
    return run("cd %s && conan install .. -pr %s -b missing" % (CONAN_DIR, DEFAULT_PROFILE))


if __name__ == "__main__":
    conan_repo("bincrafters", "https://api.bintray.com/conan/bincrafters/public-conan")
    conan_repo("luizgabriel", "https://api.bintray.com/conan/luizgabriel/conan-repo")

    if conan_install() == 0:
        print("Done! Spatial Engine is now ready for compilation")