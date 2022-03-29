import os
import os.path
import sys
from dataclasses import dataclass


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


def run(command):
    print("\n > %s\n" % command)
    result = os.system(command)
    if result != 0:
        print("The command returned a non-zero response: \n\t > {}".format(command), file=sys.stderr)
        sys.exit(result)

def conan_export(export: PackageExport):
    return "conan export %s %s" % (export.dir, export.name)


def vendor_package(package: Package):
    return "%s/%s@vendor/stable" % (package.name, package.version)


def package_path(package: Package):
    return os.path.join("vendor", "%s.py" % package.name)


def to_package_export(package: Package) -> PackageExport:
    return PackageExport(package_path(package), vendor_package(package))


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


def setup(args):
    packages = [
        Package("filament", "1.18.0"),
        Package("imgui", "docking"),
        Package("v8", "10.1.69"),
    ]

    map(lambda p: run(conan_export(to_package_export(p))), packages)


def parse_option_value(name):
    prop = "--{}".format(name)

    def h(args):
        found = next((arg for arg in args if arg.find(prop) == 0), None)
        if not found:
            return None

        return found[len(prop) + 1:]

    return h


def parse_option_bool(name):
    prop = "--{}".format(name)

    def h(args):
        found = next((arg for arg in args if arg.find(prop) == 0), None)
        return not not found

    return h


def parse_args(args):
    result = {}

    conan_profile = parse_option_value("conan-profile")(args)
    if conan_profile: result["conan-profile"] = conan_profile

    build_type = parse_option_value("build-type")(args)
    if build_type: result["build-type"] = build_type

    generator = parse_option_value("cmake-generator")(args)
    if generator: result["cmake-generator"] = generator

    result["enable-tests"] = parse_option_value("enable-tests")(args)

    build_path = parse_option_value("build-path")(args)
    if build_path: result["build-path"] = build_path

    install_path = parse_option_value("install-path")(args)
    if install_path: result["install-path"] = install_path

    source_path = parse_option_value("source-path")(args)
    if source_path: result["source-path"] = source_path

    return result

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
    build_tests = parsed_args.get("enable-tests")
    build_type = parsed_args.get("build-type", "debug")
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
    build_type = parsed_args.get("build-type", "debug")
    run(cmake_build(CmakeBuildOptions(
        build_dir=build_path,
        build_type=build_type
    )))


def install(args):
    parsed_args = parse_args(args)
    build_path = parsed_args.get("build-path", os.path.join("out", "build"))
    build_type = parsed_args.get("build-type", "debug")
    build_options = CmakeBuildOptions(
        build_dir=build_path,
        build_type=build_type
    )

    run(cmake_install(build_options))

USAGE = "Usage:"
"\n\t python cli.py setup"
"\n\t python cli.py configure [--enable-tests] [--build-type=] [--cmake-generator=] [--conan-profile=]"
"\n\t python cli.py build [--build-type=]"
"\n\t python cli.py install [--build-type=]"
"\nExample:"
"\n\t python cli.py configure --build-type=Debug --conan-profile=default"
"\n\t python cli.py configure --build-type=Release --build-tests"
"\n\t python cli.py configure --cmake-generator=\"Unix Makefiles\""
"\n\t python cli.py build --build-type=Release"
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
