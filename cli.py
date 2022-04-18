import os
import os.path
import platform
from dataclasses import dataclass
import sys
from typing import Optional

USAGE = "Usage:" \
        "\n\t python cli.py configure [--preset=]" \
        "\n\t python cli.py build [--preset=]" \
        "\n\t python cli.py install [--preset=]" \
        "\n\t python cli.py test [--preset=]" \
        "\n\t python cli.py run-editor [--preset=]" \
        "\nExample:" \
        "\n\t python cli.py run-editor" \
        "\n\t python cli.py run-editor --preset=MacOS-Debug" \
        "\n\t python cli.py configure --preset=Linux-Release" \
        "\n\t python cli.py build --preset=Windows-Release" \
        "\n\t python cli.py install --preset=Windows-Debug"

DEFAULT_SOURCE_DIR = os.path.abspath(os.path.dirname(__file__))
OS_PRESET_MAP = {
    "Linux": "Linux",
    "Darwin": "MacOS",
    "Java": "Windows",
    "Windows": "Windows"}
DETECTED_OS = OS_PRESET_MAP[platform.system()]
DEFAULT_BUILD_TYPE = "Release"


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
class Arguments:
    preset: str
    source_path: str


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


# </editor-fold>


def run(command: Command) -> CommandResult:
    print("\n > %s\n" % command.expression)
    result = os.system(command.expression)
    return CommandResult(result)


def exit_on_error(result: CommandResult):
    if result.is_error:
        print("The command returned a non-zero response", file=sys.stderr)
        sys.exit(result.code)

    return result


def conan_add_remote(remote: Remote) -> Command:
    return Command(f"conan remote add {remote.name} {remote.url}")


def conan_export(export: PackageExport) -> Command:
    return Command(f"conan export {export.dir} {export.name}")


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


def cmake_configure(options: Arguments) -> Command:
    return Command(f"cmake -S {options.source_path} --preset {options.preset}")


def cmake_build(options: Arguments) -> Command:
    return Command(f"cmake --build --preset {options.preset} --target Spatial.Editor Spatial.Core.Tests")


def cmake_install(options: Arguments) -> Command:
    return Command(f"cmake --install --preset {options.preset}")


def cmake_test(options: Arguments) -> Command:
    return Command(f"ctest --preset {options.preset}")


def run_editor(options: Arguments) -> Command:
    return Command(f"cmake --build --preset {options.preset} --target Spatial.Game")


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


def parse_args(args) -> Arguments:
    return Arguments(
        source_path=parse_option_value("source-path")(args),
        preset=parse_option_value("preset")(args)
    )


def make_preset(system: str, build_type: str) -> str:
    return "%s-%s" % (system, build_type)


def setup_cli(args: Arguments) -> CommandResult:
    run(conan_add_remote(Remote("spatial",
                                "https://luizgabriel.jfrog.io/artifactory/api/conan/spatial-conan")))

    packages = [
        Package("filament", "1.18.0"),
        Package("v8", "10.1.69"),
    ]

    to_export = to_package_export(args.source_path)

    result = CommandResult(0)
    for package in packages:
        result = result.then(lambda _: run(conan_export(to_export(package))))

    return result


def configure_cli(args: Arguments) -> CommandResult:
    return setup_cli(args) \
        .then(lambda _: run(cmake_configure(args)))


def build_cli(args: Arguments) -> CommandResult:
    return run(cmake_build(args))


def install_cli(args: Arguments) -> CommandResult:
    return run(cmake_install(args))


def test_cli(args) -> CommandResult:
    return run(cmake_test(args))


def run_editor_cli(args) -> CommandResult:
    return configure_cli(args) \
        .then(lambda _: build_cli(args)) \
        .then(lambda _: run(run_editor(args)))


CLI_COMMANDS = {
    "setup": setup_cli,
    "configure": configure_cli,
    "build": build_cli,
    "test": test_cli,
    "run-editor": run_editor_cli,
    "install": install_cli
}


def main():
    sys_args = sys.argv[1:]
    if len(sys_args) == 0:
        print(USAGE)
        return

    action = CLI_COMMANDS[sys_args[0]]

    args = parse_args(sys_args[1:])
    args.source_path = args.source_path if args.source_path else DEFAULT_SOURCE_DIR
    args.preset = args.preset if args.preset else make_preset(
        DETECTED_OS, DEFAULT_BUILD_TYPE)

    action(args).catch(exit_on_error)


if __name__ == "__main__":
    main()
