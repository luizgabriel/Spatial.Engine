import os
import sys
import shutil
import subprocess
import re

from conans import ConanFile, CMake, tools
from conans.errors import ConanInvalidConfiguration


class V8Conan(ConanFile):
    name = "v8"
    version = "10.4.66"
    license = "BSD"
    homepage = "https://v8.dev"
    url = "https://github.com/v8/v8"
    description = "V8 is Google's open source JavaScript engine."
    topics = ("javascript", "interpreter", "compiler", "virtual-machine", "javascript-engine")

    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False]}
    default_options = {"shared": False}
    generators = "cmake"
    revision_mode = "hash"
    short_paths = True

    exports_sources = [
        "v8_msvc_crt.gn",
        "v8_linux_toolchain.gn",
        "v8_libcxx_config.gn"
    ]

    _source_subfolder = "source_subfolder"
    _build_subfolder = "build_subfolder"

    def _check_python_version(self):
        """depot_tools requires python >= 2.7.5 or >= 3.8 for python 3 support."""
        python_exe = tools.which("python")
        if not python_exe:
            msg = ("Python must be available in PATH "
                   "in order to build v8")
            raise ConanInvalidConfiguration(msg)
        # In any case, check its actual version for compatibility
        from six import StringIO  # Python 2 and 3 compatible
        version_buf = StringIO()
        cmd_v = "\"{}\" --version".format(python_exe)
        self.run(cmd_v, output=version_buf)
        p = re.compile(r'Python (\d+\.\d+\.\d+)')
        verstr = p.match(version_buf.getvalue().strip()).group(1)
        if verstr.endswith('+'):
            verstr = verstr[:-1]
        version = tools.Version(verstr)
        # >= 2.7.5 & < 3
        py2_min = "2.7.5"
        py2_max = "3.0.0"
        py3_min = "3.8.0"
        if (version >= py2_min) and (version < py2_max):
            msg = ("Found valid Python 2 required for v8:"
                   " version={}, path={}".format(version_buf.getvalue().strip(), python_exe))
            self.output.success(msg)
        elif version >= py3_min:
            msg = ("Found valid Python 3 required for v8:"
                   " version={}, path={}".format(version_buf.getvalue().strip(), python_exe))
            self.output.success(msg)
        else:
            msg = ("Found Python in path, but with invalid version {}"
                   " (v8 requires >= {} and < "
                   "{} or >= {})".format(verstr, py2_min, py2_max, py3_min))
            raise ConanInvalidConfiguration(msg)

    def system_requirements(self):
        if tools.os_info.is_linux:
            if not tools.which("lsb-release"):
                tools.SystemPackageTool().install("lsb-release")
        self._check_python_version()

    def build_requirements(self):
        if not tools.which("ninja"):
            self.build_requires("ninja/1.10.0")
        if self.settings.os != "Windows":
            if not tools.which("bison"):
                self.build_requires("bison/3.5.3")
            if not tools.which("gperf"):
                self.build_requires("gperf/3.1")
            if not tools.which("flex"):
                self.build_requires("flex/2.6.4")

    def _set_environment_vars(self):
        """set the environment variables, such that the google tooling is found (including the bundled python2)"""
        os.environ["PATH"] = os.path.join(self.source_folder, "depot_tools") + os.pathsep + os.environ["PATH"]
        os.environ["DEPOT_TOOLS_PATH"] = os.path.join(self.source_folder, "depot_tools")
        if tools.os_info.is_windows:
            os.environ["DEPOT_TOOLS_WIN_TOOLCHAIN"] = "0"
            os.environ["GYP_MSVS_VERSION"] = "2017" if str(self.settings.compiler.version) == "15" else "2019"
        if tools.os_info.is_macos and self.gn_arch == "arm64":
            os.environ["VPYTHON_BYPASS"] = "manually managed python not supported by chrome operations"

    def source(self):
        tools.Git(folder="depot_tools").clone("https://chromium.googlesource.com/chromium/tools/depot_tools.git",
                                              shallow=True)

        if tools.os_info.is_macos and self.gn_arch == "arm64":
            self.run("mkdir v8")
            with tools.chdir("v8"):
                self.run("echo \"mac-arm64\" > .cipd_client_platform")

        self._set_environment_vars()
        self.run("fetch v8")

        with tools.chdir("v8"):
            self.run("git checkout {}".format(self.version))
            self.run("gclient sync")

    @property
    def gn_arch(self):
        arch_map = {
            "x86_64": "x64",
            "armv8": "arm64"
        }

        arch = str(self.settings.arch)
        return arch_map.get(arch, arch)

    def _install_system_requirements_linux(self):
        """some extra script must be executed on linux"""
        self.output.info("Calling v8/build/install-build-deps.sh")
        os.environ["PATH"] += os.pathsep + os.path.join(self.source_folder, "depot_tools")
        sh_script = self.source_folder + "/v8/build/install-build-deps.sh"
        self.run("chmod +x " + sh_script)
        cmd = sh_script + " --unsupported --no-arm --no-nacl --no-backwards-compatible --no-chromeos-fonts --no-prompt "
        cmd = cmd + ("--syms" if str(self.settings.build_type) == "Debug" else "--no-syms")
        cmd = "export DEBIAN_FRONTEND=noninteractive && " + cmd
        self.run(cmd)

    def _patch_gn_build_system(self, source_file, dest_folder):
        if os.path.exists(os.path.join(dest_folder, "BUILD.gn")):
            return True
        tools.mkdir(dest_folder)
        shutil.copy(
            os.path.join(self.source_folder, source_file),
            os.path.join(dest_folder, "BUILD.gn"))
        return False

    def _patch_msvc_runtime(self):
        v8_source_root = os.path.join(self.source_folder, "v8")
        msvc_config_folder = os.path.join(v8_source_root, "build", "config", "conan", "msvc")
        self._patch_gn_build_system("v8_msvc_crt.gn", msvc_config_folder)

    def _define_conan_toolchain(self):
        v8_source_root = os.path.join(self.source_folder, "v8")
        conan_toolchain_folder = os.path.join(v8_source_root, "build", "toolchain", "conan", "linux")
        self._patch_gn_build_system("v8_linux_toolchain.gn", conan_toolchain_folder)

    def _path_compiler_config(self):
        v8_source_root = os.path.join(self.source_folder, "v8")
        libcxx_config_folder = os.path.join(v8_source_root, "build", "config", "conan", "libcxx")
        self._patch_gn_build_system("v8_libcxx_config.gn", libcxx_config_folder)

    def _gen_arguments(self):
        # Refer to v8/infra/mb/mb_config.pyl
        is_debug = "true" if str(self.settings.build_type) == "Debug" else "false"
        is_clang = "true" if ("clang" in str(self.settings.compiler).lower()) else "false"
        gen_arguments = [
            "is_debug = " + is_debug,
            "enable_iterator_debugging = " + is_debug,

            "target_cpu = \"%s\"" % self.gn_arch,
            "is_component_build = false",
            "is_chrome_branded = false",
            "treat_warnings_as_errors = false",
            "is_clang = " + is_clang,
            "use_glib = false",
            "use_sysroot = false",
            "use_custom_libcxx = false",
            "use_custom_libcxx_for_host = false",

            # V8 specific settings
            "v8_monolithic = true",
            "v8_static_library = true",
            "v8_use_external_startup_data = false",
            # "v8_enable_backtrace = false",
        ]

        if tools.os_info.is_windows:
            gen_arguments += [
                "conan_compiler_runtime = \"%s\"" % str(self.settings.compiler.runtime)
            ]

        if tools.os_info.is_linux:
            toolchain_to_use = "//build/toolchain/conan/linux:%s_%s" % (self.settings.compiler, self.settings.arch)
            gen_arguments += [
                "custom_toolchain=\"%s\"" % toolchain_to_use,
                "host_toolchain=\"%s\"" % toolchain_to_use
            ]

        if tools.os_info.is_linux or tools.os_info.is_macos:
            gen_arguments += [
                "conan_compiler_name = \"%s\"" % self.settings.compiler,
                "conan_compiler_libcxx = \"%s\"" % self.settings.compiler.libcxx
            ]

        return gen_arguments

    def build(self):
        v8_source_root = os.path.join(self.source_folder, "v8")
        self._set_environment_vars()

        if tools.os_info.is_linux:
            self._install_system_requirements_linux()
            self._define_conan_toolchain()

        if tools.os_info.is_linux or tools.os_info.is_macos:
            self._path_compiler_config()

        with tools.chdir(v8_source_root):
            if tools.os_info.is_windows and str(self.settings.compiler) == "Visual Studio":
                self._patch_msvc_runtime()

            args = self._gen_arguments()
            args_gn_file = os.path.join(self.build_folder, "args.gn")
            with open(args_gn_file, "w") as f:
                f.write("\n".join(args))

            generator_call = "gn gen {folder}".format(folder=self.build_folder)

            self.run("python --version")
            print(generator_call)
            self.run(generator_call)
            self.run("ninja -v -C {folder} v8_monolith".format(folder=self.build_folder))

    def package(self):
        self.copy(pattern="LICENSE*", dst="licenses", src="v8")
        self.copy(pattern="*v8_monolith.a", dst="lib", keep_path=False)
        self.copy(pattern="*v8_monolith.lib", dst="lib", keep_path=False)
        self.copy(pattern="*.h", dst="include/v8/include", src="v8/include", keep_path=True)

    def package_info(self):
        self.cpp_info.libs = ["v8_monolith"]
        self.cpp_info.includedirs.append("include/v8")
        self.cpp_info.includedirs.append("include/v8/include")

        # Pre-configured settings come with conan-v8
        self.cpp_info.defines += [
            "V8_COMPRESS_POINTERS"
        ]
        if self.settings.compiler in ["gcc", "clang", "apple-clang"]:
            self.cpp_info.cxxflags += [
                "-std=c++14"
            ]
        if self.settings.os == "Windows":
            self.cpp_info.system_libs += [
                "winmm.lib",
                "dbghelp.lib"
            ]
            # self.cpp_info.defines += [
            #     "_HAS_ITERATOR_DEBUGGING=0"
            # ]
        else:
            self.cpp_info.cxxflags += ["-pthread"]
