#!python

import os

CONAN_DIR = ".conan"
VENDOR_DIR = os.path.join(CONAN_DIR, "vendor")
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


def export_dependencies():
    conan_repo("bincrafters", "https://api.bintray.com/conan/bincrafters/public-conan")

    conan_export("luizgabriel", "conan-filament", "filament/1.7.0@google/stable", "v1.7.0")
    conan_export("luizgabriel", "conan-imgui", "imgui/docking@ocornut/testing", "docking")
    conan_export("luizgabriel", "conan-tomlplusplus", "toml++/1.2.0@marzer/stable")
    conan_export("skypjack", "entt", "entt/3.4.0@skypjack/stable", "v3.4.0")


if __name__ == "__main__":
    try:
        os.makedirs(CONAN_DIR)
    except:
        pass

    export_dependencies()

    if conan_install() == 0:
        print("Done! Spatial Engine is now ready for compilation")
