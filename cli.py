import os
import os.path
import sys
from dataclasses import dataclass
from typing import Optional

USAGE = "Usage:" \
        "\n\t python cli.py configure [--with-tests] [--build-type=] [--conan-profile=] [--cmake-generator=]" \
        "\n\t python cli.py build [--build-type=]" \
        "\n\t python cli.py install [--build-type=]" \
        "\n\t python cli.py test [--desktop-tests]" \
        "\n\t python cli.py run-editor [--build-type=] [--conan-profile=] [--cmake-generator=]" \
        "\nExample:" \
        "\n\t python cli.py run-editor --build-type=Release --conan-profile=default" \
        "\n\t python cli.py configure --conan-profile=default" \
        "\n\t python cli.py configure --build-type=Release --with-tests" \
        "\n\t python cli.py configure --cmake-generator=\"Unix Makefiles\"" \
        "\n\t python cli.py build --build-type=Release" \
        "\n\t python cli.py install --build-type=Release"

DEFAULT_SOURCE_DIR = os.path.abspath(os.path.dirname(__file__))
DEFAULT_BUILD_DIR = os.path.join(DEFAULT_SOURCE_DIR, "out", "build")
DEFAULT_INSTALL_DIR = os.path.join(DEFAULT_SOURCE_DIR, "out", "install")
DEFAULT_BUILD_TYPE = "Debug"
DEFAULT_CMAKE_GENERATOR = {
    "win32": "Visual Studio 16 2019",
    "cygwin": "Visual Studio 16 2019",
    "darwin": "CodeBlocks - Unix Makefiles",
    "linux": "Ninja"
}.get(sys.platform, None)
IS_WINDOWS = (sys.platform in {"win32", "cygwin"})


# <editor-fold desc="Data Objects">

@dataclass(init=True)
class Package:
    name: str
    version: str


@dataclass(init=True)
class PackageExport:
    dir: str
    name: str


@dataclass(init=True)
class Remote:
    name: str
    url: str


@dataclass(init=True)
class CmakeConfigureOptions:
    generator: str
    source_dir: str
    build_dir: str
    install_dir: str
    build_type: str
    with_tests: bool
    conan_profile: str

    @property
    def is_debug(self) -> bool:
        return "debug" in self.build_type.lower()

    @property
    def is_msvc(self) -> bool:
        return "Visual Studio" in self.generator


@dataclass(init=True)
class CmakeBuildOptions:
    build_dir: str
    build_type: str
    with_tests: bool


@dataclass(init=True)
class TestOptions:
    build_dir: str
    desktop_tests: bool


@dataclass(init=True)
class RunEditorOptions:
    build_dir: str
    project_dir: str
    is_windows: bool


@dataclass(init=True)
class CommandResult:
    code: int

    @property
    def is_error(self):
        return self.code != 0

    def then(self, func):
        if self.is_error:
            return self

        return func(self)

    def catch(self, func):
        if not self.is_error:
            return self

        return func(self)


@dataclass(init=True)
class Command:
    expression: str
    working_directory: str = None

# </editor-fold>


def run(command: Command) -> CommandResult:
    print("\n > %s\n" % command.expression)
    current_working_directory = os.getcwd()
    if command.working_directory:
        os.chdir(command.working_directory)

    result = CommandResult(os.system(command.expression))

    if command.working_directory:
        os.chdir(current_working_directory)

    return result


def exit_on_error(result: CommandResult):
    if result.is_error:
        print("The command returned a non-zero response", file=sys.stderr)
        sys.exit(result.code)

    return result


def conan_add_remote(remote: Remote) -> Command:
    return Command("conan remote add %s %s" % (remote.name, remote.url))


def conan_export(export: PackageExport) -> Command:
    return Command("conan export %s %s" % (export.dir, export.name))


def vendor_package(package: Package):
    return "%s/%s@spatial/stable" % (package.name, package.version)


def package_path(source_path: str):
    def h(package: Package):
        return os.path.join(source_path, "vendor", "%s.py" % package.name)

    return h


def to_package_export(source_path):
    to_package_path = package_path(source_path)

    def h(package: Package) -> PackageExport:
        return PackageExport(dir=to_package_path(package), name=vendor_package(package))

    return h


def cmake_configure(options: CmakeConfigureOptions) -> Command:
    commands = ["cmake"]

    commands += ["-S {}".format(options.source_dir)]

    commands += ["-B {}".format(options.build_dir)]

    commands += ["-G \"{}\"".format(options.generator)]

    if options.install_dir:
        commands += ["-DCMAKE_INSTALL_PREFIX={}".format(options.install_dir)]

    if options.conan_profile:
        commands += ["-DCONAN_PROFILE={}".format(options.conan_profile)]

    # Force the use of MT / MTd compiler runtime on Visual Studio (Filament Library Requirement)
    if not options.conan_profile and options.is_msvc:
        commands += ["-DCMAKE_CXX_FLAGS=/MT{}".format(
            "d" if options.is_debug else "")]

    if options.build_type:
        commands += ["-DCMAKE_BUILD_TYPE={}".format(
            str(options.build_type).capitalize())]

    commands += ["-DSPATIAL_BUILD_TESTS={}".format(
        "ON" if options.with_tests else "OFF")]

    return Command(" ".join(commands))


def cmake_build(options: CmakeBuildOptions) -> Command:
    targets = {"Spatial.Editor"}
    if options.with_tests:
        targets.add("Spatial.Core.Tests")

    return Command("cmake --build {build_dir} --config {build_type} --target {targets}".format(
        build_dir=options.build_dir,
        build_type=options.build_type,
        targets=" ".join(targets)
    ))


def cmake_install(options: CmakeBuildOptions) -> Command:
    return Command("cmake --install {build_dir}".format(
        build_dir=options.build_dir,
        build_type=options.build_type
    ))


def run_tests(options: TestOptions) -> Command:
    desktop_tests = "--gtest_filter='-Application.*:DesktopWindow.*'"

    return Command(
        expression="./Spatial.Core.Tests {options}".format(
            options=desktop_tests if options.desktop_tests else ""),
        working_directory=os.path.join(options.build_dir, "Spatial.Core/tests")
    )


def run_editor(options: RunEditorOptions) -> Command:
    editor_executable = "Spatial.Editor" + (".exe" if options.is_windows else "")

    return Command(
        expression=os.path.join(options.build_dir, "Spatial.Editor", editor_executable), 
        working_directory=options.project_dir
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
    result = {"with-tests": parse_option_bool("with-tests")(args), "desktop-tests": parse_option_bool("desktop-tests")(args)}

    options = ["conan-profile", "build-type", "cmake-generator",
               "build-path", "install-path", "source-path"]
    for option in options:
        value = parse_option_value(option)(args)
        if value:
            result[option] = value

    return result


def setup_cli(args) -> CommandResult:
    source_path = args.get("source-path", DEFAULT_SOURCE_DIR)

    run(conan_add_remote(Remote("luizgabriel",
        "https://luizgabriel.jfrog.io/artifactory/api/conan/luizgabriel-conan")))

    packages = [
        Package("filament", "1.18.0"),
        Package("imgui", "docking"),
        Package("v8", "10.1.69"),
    ]

    to_export = to_package_export(source_path)

    result = CommandResult(0)
    for package in packages:
        result = result.then(lambda _: run(conan_export(to_export(package))))

    return result


def configure_cli(args) -> CommandResult:
    command = cmake_configure(CmakeConfigureOptions(
        source_dir=args.get("source-path", DEFAULT_SOURCE_DIR),
        build_dir=args.get("build-path", DEFAULT_BUILD_DIR),
        install_dir=args.get("install-path", DEFAULT_INSTALL_DIR),
        generator=args.get("cmake-generator", DEFAULT_CMAKE_GENERATOR),
        with_tests=args.get("with-tests", False),
        build_type=args.get("build-type", DEFAULT_BUILD_TYPE),
        conan_profile=args.get("conan-profile"),
    ))

    return setup_cli(args).then(lambda _: run(command))


def build_cli(args) -> CommandResult:
    return run(cmake_build(CmakeBuildOptions(
        build_dir=args.get("build-path", DEFAULT_BUILD_DIR),
        build_type=args.get("build-type", DEFAULT_BUILD_TYPE),
        with_tests=args.get("with-tests", False)
    )))


def install_cli(args) -> CommandResult:
    return run(cmake_install(CmakeBuildOptions(
        build_dir=args.get("build-path", DEFAULT_BUILD_DIR),
        build_type=args.get("build-type", DEFAULT_BUILD_TYPE)
    )))


def test_cli(args) -> CommandResult:
    command = run_tests(TestOptions(
                build_dir=args.get("build-path", DEFAULT_BUILD_DIR),
                desktop_tests=args.get("desktop-tests", True)))

    return run(command)


def run_editor_cli(args) -> CommandResult:
    source_dir = args.get("source-path", DEFAULT_SOURCE_DIR)

    command = run_editor(RunEditorOptions(
        build_dir=args.get("build-path", DEFAULT_BUILD_DIR),
        project_dir=os.path.join(source_dir, "Spatial.Game"),
        is_windows=IS_WINDOWS
    ))

    return configure_cli(args) \
        .then(lambda _: build_cli(args)) \
        .then(lambda _: run(command))


def main():
    args = sys.argv[1:]
    if len(args) == 0:
        print(USAGE)
        return


    action = {
        "setup": setup_cli,
        "configure": configure_cli,
        "build": build_cli,
        "test": test_cli,
        "run-editor": run_editor_cli,
        "install": install_cli
    }[args[0]]

    parsed_args = parse_args(args[1:])
    action(parsed_args).catch(exit_on_error)


if __name__ == "__main__":
    main()
