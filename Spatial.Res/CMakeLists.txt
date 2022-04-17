add_executable(Spatial.Res src/main.cpp)
set_property(TARGET Spatial.Res PROPERTY CXX_STANDARD 20)
set_project_warnings(Spatial.Res)
enable_sanitizers(Spatial.Res)

target_link_libraries(Spatial.Res PUBLIC CONAN_PKG::argh CONAN_PKG::fmt)
set_property(
        TARGET Spatial.Res
        PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

target_precompile_headers(Spatial.Res PRIVATE <iostream> <filesystem> <fstream> <string> <locale>)