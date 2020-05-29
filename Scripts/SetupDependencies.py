#!python

import os

CONAN_DIR = ".conan"
VENDOR_DIR = "%s\\vendor" % CONAN_DIR
DEFAULT_PROFILE = "default"


def run(command):
    print("\n\n > %s\n" % command)
    return os.system(command)


def conan_repo(name, url):
    return run('conan remote add %s %s' % (name, url))


def conan_export(user, package, export_name, branch="master"):
    print("Exporting %s" % package)
    package_dir = os.path.join(VENDOR_DIR, package)

    result = 0
    result += run("git clone -b %s https://github.com/%s/%s.git %s" % (branch, user, package, package_dir))
    result += run("conan export %s %s" % (package_dir, export_name))

    return result


def conan_install():
    return run("cd %s && conan install .. -pr %s -b missing" % (CONAN_DIR, DEFAULT_PROFILE))


if __name__ == "__main__":
    try:
        os.makedirs(CONAN_DIR)
    except:
        pass

    conan_repo("bincrafters", "https://api.bintray.com/conan/bincrafters/public-conan")

    conan_export("luizgabriel", "conan-filament", "google/stable", "v1.7.0")
    conan_export("luizgabriel", "conan-imgui", "ocornut/testing", "docking")
    conan_export("luizgabriel", "conan-tomlplusplus", "marzer/stable")

    if conan_install() == 0:
        print("Done! Spatial Engine is now ready for compilation")
