import os
import os.path
import sys
from dataclasses import dataclass
from typing import Optional


@dataclass(init=True)
class Package:
    name: str
    version: str


@dataclass(init=True)
class PackageExport:
    dir: str
    name: str


@dataclass(init=True)
class CmakeConfigureOptions:
    generator: str
    source_dir: str
    build_dir: str
    install_dir: str
    build_type: str
    build_tests: bool
    conan_profile: str


@dataclass(init=True)
class CmakeBuildOptions:
    build_dir: str
    build_type: str


def run(command: str):
    print("\n > %s\n" % command)
    result = os.system(command)
    if result != 0:
        print("The command returned a non-zero response: \n > {}".format(command), file=sys.stderr)
        sys.exit(result)


def conan_export(export: PackageExport):
    return "conan export %s %s" % (export.dir, export.name)


def vendor_package(package: Package):
    return "%s/%s@vendor/stable" % (package.name, package.version)


def package_path(source_path: str):
    def h(package: Package):
        return os.path.join(source_path, "vendor", "%s.py" % package.name)

    return h


def to_package_export(source_path):
    to_package_path = package_path(source_path)

    def h(package: Package) -> PackageExport:
        return PackageExport(dir=to_package_path(package), name=vendor_package(package))

    return h


def cmake_configure(options: CmakeConfigureOptions):
    commands = ["cmake -S {} -B {}".format(options.source_dir, options.build_dir)]

    if options.generator:
        commands += ["-G \"{}\"".format(options.generator)]

    if options.install_dir:
        commands += ["-DCMAKE_INSTALL_PREFIX={}".format(options.install_dir)]

    if options.conan_profile:
        commands += ["-DCONAN_PROFILE={}".format(options.conan_profile)]

    if options.build_type:
        commands += ["-DCMAKE_BUILD_TYPE={}".format(str(options.build_type).capitalize())]

    commands += ["-DSPATIAL_BUILD_TESTS={}".format("ON" if options.build_tests else "OFF")]

    return " ".join(commands)


def cmake_build(options: CmakeBuildOptions):
    return "cmake --build {build_dir} --config {build_type} --target all".format(
        build_dir=options.build_dir,
        build_type=options.build_type
    )


def cmake_install(options: CmakeBuildOptions):
    return "cmake --build {build_dir} --target install".format(
        build_dir=options.build_dir,
        build_type=options.build_type
    )


def parse_option_value(name: str):
    prop = "--{}".format(name)

    def h(args) -> Optional[str]:
        found = next((arg for arg in args if arg.find(prop) == 0), None)
        if not found:
            return None

        return found[len(prop) + 1:]

    return h


def parse_option_bool(name: str):
    prop = "--{}".format(name)

    def h(args) -> bool:
        found = next((arg for arg in args if arg.find(prop) == 0), None)
        return not not found

    return h


def parse_args(args):
    result = {"enable-tests": parse_option_bool("enable-tests")(args)}

    options = ["conan-profile", "build-type", "cmake-generator", "build-path", "install-path", "source-path"]
    for option in options:
        value = parse_option_value(option)(args)
        if value:
            result[option] = value

    return result


def setup(args):
    parsed_args = parse_args(args)
    source_path = parsed_args.get("source-path", os.path.abspath(os.path.dirname(__file__)))

    packages = [
        Package("filament", "1.18.0"),
        Package("imgui", "docking"),
        Package("v8", "10.1.69"),
    ]

    to_export = to_package_export(source_path)

    for package in packages:
        run(conan_export(to_export(package)))


def configure(args):
    parsed_args = parse_args(args)
    source_path = parsed_args.get("source-path", os.path.abspath(os.path.dirname(__file__)))
    build_path = parsed_args.get("build-path", os.path.join("out", "build"))
    install_path = parsed_args.get("install-path", os.path.join("out", "install"))
    recommended_sys_generator = {
        "win32": "Visual Studio 16 2019",
        "cygwin": "Visual Studio 16 2019",
        "darwin": "CodeBlocks - Unix Makefiles",
        "linux": "Ninja"
    }.get(sys.platform, None)

    generator = parsed_args.get("cmake-generator", recommended_sys_generator)
    build_tests = parsed_args.get("enable-tests", False)
    build_type = parsed_args.get("build-type", "Debug")
    conan_profile = parsed_args.get("conan-profile")

    setup(args)

    run(cmake_configure(CmakeConfigureOptions(
        source_dir=source_path,
        build_dir=build_path,
        install_dir=install_path,
        generator=generator,
        build_tests=build_tests,
        build_type=build_type,
        conan_profile=conan_profile,
    )))


def build(args):
    parsed_args = parse_args(args)
    build_path = parsed_args.get("build-path", os.path.join("out", "build"))
    build_type = parsed_args.get("build-type", "Debug")
    run(cmake_build(CmakeBuildOptions(
        build_dir=build_path,
        build_type=build_type
    )))


def install(args):
    parsed_args = parse_args(args)
    build_path = parsed_args.get("build-path", os.path.join("out", "build"))
    build_type = parsed_args.get("build-type", "Debug")
    build_options = CmakeBuildOptions(
        build_dir=build_path,
        build_type=build_type
    )

    run(cmake_install(build_options))


USAGE = "Usage:" \
        "\n\t python cli.py configure [--enable-tests] [--build-type=] [--conan-profile=] [--cmake-generator=]" \
        "\n\t python cli.py build [--build-type=]" \
        "\n\t python cli.py install [--build-type=]" \
        "\nExample:" \
        "\n\t python cli.py configure --conan-profile=default" \
        "\n\t python cli.py configure --build-type=Release --enable-tests" \
        "\n\t python cli.py configure --cmake-generator=\"Unix Makefiles\"" \
        "\n\t python cli.py build --build-type=Release" \
        "\n\t python cli.py install --build-type=Release"


def main():
    args = sys.argv[1:]
    if len(args) == 0:
        print(USAGE)
        return

    action = {
        "setup": setup,
        "configure": configure,
        "build": build,
        "install": install
    }[args[0]]

    action(args[1:])


if __name__ == "__main__":
    main()
