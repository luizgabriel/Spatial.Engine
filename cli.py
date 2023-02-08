import os
import os.path
import platform
import sys
from dataclasses import dataclass
from typing import Optional
import subprocess


USAGE = "Usage:" \
        "\n\t python cli.py [setup|configure|build|install|test|run-editor] [--preset=] [--docs] [--tests]" \
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


@dataclass(init=True)
class Package:
    name: str
    version: str


VENDOR_PACKAGES = (
    Package("filament", "1.25.6"),
    Package("v8", "10.1.69"),
    Package("imgui", "docking"),
)


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
    generate_docs: bool
    build_tests: bool

    def with_tests(self):
        return Arguments(self.preset, self.source_path, self.generate_docs, True)

    @property
    def example_args(self) -> str:
        docs = "--docs" if self.generate_docs else ""
        tests = "--tests" if self.build_tests else ""
        return f"--preset={self.preset} {docs} {tests}"


@dataclass(init=True)
class CommandResult:
    code: int
    reason: Optional[str] = None

    @staticmethod
    def success():
        return CommandResult(0)

    def with_reason(self, reason: str):
        return CommandResult(self.code, reason)

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
    expression: list[str]

    def __str__(self):
        return " ".join(self.expression)


def run(command: Command) -> CommandResult:
    print("\n > %s\n" % command)
    result = subprocess.call(command.expression)
    return CommandResult(result)


def exit_on_error(result: CommandResult):
    if result.is_error:
        print("\n-- [SPATIAL-CLI] The command returned a non-zero response.", file=sys.stderr)
        if result.reason:
            print(f"{result.reason}", file=sys.stderr)
        sys.exit(result.code)

    return result


def conan_add_remote(remote: Remote) -> Command:
    return Command(["conan", "remote", "add", remote.name, remote.url])


def conan_export(export: PackageExport) -> Command:
    return Command(["conan", "export", export.dir, export.name])


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
    docs = ["-DSPATIAL_ENABLE_DOXYGEN=ON"] if options.generate_docs else []
    return Command(["cmake", "-S", options.source_path, "--preset", options.preset] + docs)


def cmake_build(options: Arguments) -> Command:
    targets = ["Spatial.Editor"]
    test_targets = ["Spatial.Core.Tests", "Spatial.Graphics.Tests"] if options.build_tests else []
    docs_targets = ["Spatial.Docs"] if options.generate_docs else []

    return Command(["cmake", "--build", "--preset", options.preset, "--target"] + targets + test_targets + docs_targets)


def cmake_install(options: Arguments) -> Command:
    build_path = os.path.join(options.source_path, "out", "build", options.preset)
    build_type = options.preset.split("-")[-1]
    return Command(["cmake", "--install", build_path, "--config", build_type])


def cmake_package(options: Arguments) -> Command:
    return Command(["cmake", "--build", "--preset", options.preset, "--target", "package"])


def cmake_test(options: Arguments) -> Command:
    build_type = options.preset.split("-")[-1]
    return Command(["ctest", "--verbose", "--preset", options.preset, "--config", build_type])


def run_editor(options: Arguments) -> Command:
    return Command(["cmake", "--build", "--preset", options.preset, "--target", "Spatial.Game"])


def parse_option_value(name: str):
    prop = "--{}".format(name)

    def h(args: list[str]) -> Optional[str]:
        found = next((arg for arg in args if arg.find(prop) == 0), None)
        if not found:
            return None

        return found[len(prop) + 1:]

    return h


def parse_option_bool(name: str):
    prop = "--{}".format(name)

    def h(args: list[str]) -> bool:
        found = next((arg for arg in args if arg.find(prop) == 0), None)
        return not not found

    return h


def parse_args(args: list[str]) -> Arguments:
    return Arguments(
        source_path=parse_option_value("source-path")(args),
        preset=parse_option_value("preset")(args),
        generate_docs=parse_option_bool("docs")(args),
        build_tests=parse_option_bool("tests")(args)
    )


def make_preset(system: str, build_type: str) -> str:
    return "%s-%s" % (system, build_type)


def setup_cli(args: Arguments, packages=VENDOR_PACKAGES) -> CommandResult:
    to_export = to_package_export(args.source_path)

    result = CommandResult.success()
    for package in packages:
        result = result.then(lambda _: run(conan_export(to_export(package))))

    return result


def configure_cli(args: Arguments) -> CommandResult:
    return setup_cli(args) \
        .then(lambda _: run(cmake_configure(args))).with_reason(
        "There was an error configuring the project."
        "\nMake sure you have all system requirements installed."
        "\nSee the README for more information.")


def build_cli(args: Arguments) -> CommandResult:
    return run(cmake_build(args)).with_reason(f"Have you run `python cli.py configure {args.example_args}`?")


def install_cli(args: Arguments) -> CommandResult:
    return run(cmake_install(args)).with_reason(f"Have you run `python cli.py build {args.example_args}`?")


def package_cli(args: Arguments) -> CommandResult:
    return run(cmake_package(args)).with_reason(f"Have you run `python cli.py build {args.example_args}`?")


def test_cli(args: Arguments) -> CommandResult:
    return run(cmake_test(args)).with_reason(f"Have you run `python cli.py build {args.with_tests().example_args}`?")


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
    "install": install_cli,
    "package": package_cli
}


def main():
    sys_args = sys.argv[1:]
    if len(sys_args) == 0:
        print(USAGE)
        return

    action = CLI_COMMANDS.get(sys_args[0], None)
    if action is None:
        print(f"ERROR: Command not found\n---------\n\n{USAGE}")
        return

    cmd_args = parse_args(sys_args[1:])
    cmd_args.source_path = cmd_args.source_path if cmd_args.source_path else DEFAULT_SOURCE_DIR
    cmd_args.preset = cmd_args.preset if cmd_args.preset else make_preset(
        DETECTED_OS, DEFAULT_BUILD_TYPE)

    action(cmd_args).catch(exit_on_error)


if __name__ == "__main__":
    main()
