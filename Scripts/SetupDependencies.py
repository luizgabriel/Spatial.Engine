#!/usr/bin/python3

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
    return run('conan remote add %s %s > /dev/null 2>&1' % (name, url))


def conan_export(user, package, export_name):
    print("Exporting %s" % package)
    package_dir = os.path.join(TMP_DIR, package)
    
    result = 0
    result += run("git clone https://github.com/%s/%s.git %s" % (user, package, package_dir))
    result += run("conan export %s %s" % (package_dir, export_name))

    return result


def conan_install():
    return run("cd %s && conan install .. -pr %s -b missing" % (CONAN_DIR, DEFAULT_PROFILE))


if __name__ == "__main__":
    try:
        os.makedirs(TMP_DIR)
    except:
        pass

    conan_export("luizgabriel", "conan-filament", "google/stable")
    conan_export("luizgabriel", "conan-imgui", "ocornut/testing")
    conan_repo("public", "https://api.bintray.com/conan/bincrafters/public-conan")

    if conan_install() == 0:
        print("Done! Spatial Engine is now ready for compilation")

    try:
        shutil.rmtree(TMP_DIR)
    except:
        print("[warn] Could not deleted \"%s\" dir. Please, delete it manually." % TMP_DIR)

    