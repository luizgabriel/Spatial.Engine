import os
import os.path


def run(command):
    print("\n\n > %s\n" % command)
    return os.system(command)


def conan_repo(name, url):
    return run('conan remote add %s %s' % (name, url))


def conan_export(clone_dir, user, package, export_name, branch="master"):
    print("Exporting %s" % package)
    package_dir = os.path.join(clone_dir, package)

    result = 0
    result += run("git clone -b %s https://github.com/%s/%s.git %s" % (branch, user, package, package_dir))
    result += run("conan export %s %s" % (package_dir, export_name))

    return result


def conan_install(path, profile):
    return run("cd %s && conan install .. -pr %s -b missing" % (path, profile))


def make_dirs(folder):
    try:
        os.makedirs(folder)
    except:
        print("Could not create folder: %s" % folder)


def install(folder, conan_profile="default"):
    install_folder = os.path.abspath(folder)
    conan_path = os.path.join(install_folder, ".conan")
    vendor_path = os.path.join(conan_path, "vendor")
    make_dirs(vendor_path)

    conan_repo("bincrafters", "https://api.bintray.com/conan/bincrafters/public-conan")

    conan_export(vendor_path, "luizgabriel", "conan-filament", "filament/1.7.0@google/stable", "v1.7.0")
    conan_export(vendor_path, "luizgabriel", "conan-imgui", "imgui/docking@ocornut/testing", "docking")
    conan_export(vendor_path, "luizgabriel", "conan-tomlplusplus", "toml++/1.2.0@marzer/stable")
    conan_export(vendor_path, "skypjack", "entt", "entt/3.4.0@skypjack/stable", "v3.4.0")

    if conan_install(conan_path, conan_profile) == 0:
        print("Done! Spatial Engine is now ready for compilation")


if __name__ == "__main__":
    install(os.path.abspath(os.path.dirname(__file__)))
