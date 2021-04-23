import os
import os.path
import sys


def run(command):
    print("\n\n > %s\n" % command)
    return os.system(command)


def conan_export(package_dir, export_name):
    return run("conan export %s %s" % (package_dir, export_name))


def conan_add_remote(remote_name, remote_url):
    return run("conan remote add %s %s" % (remote_name, remote_url))


def install(folder):
    install_folder = os.path.abspath(folder)
    vendor_path = os.path.join(install_folder, "vendor")

    conan_add_remote("bincrafters", "https://api.bintray.com/conan/bincrafters/public-conan")

    conan_export(os.path.join(vendor_path, "filament.py"), "filament/1.9.21@vendor/stable")
    conan_export(os.path.join(vendor_path, "imgui.py"), "imgui/docking@vendor/stable")


if __name__ == "__main__":
    args = iter(sys.argv)
    next(args)  # skip file name

    root_directory = next(args, os.path.dirname(__file__))
    install(root_directory)
