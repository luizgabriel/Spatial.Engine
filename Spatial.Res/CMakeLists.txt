add_executable(Spatial.Res src/main.cpp)
target_link_libraries(Spatial.Res PUBLIC CONAN_PKG::argh CONAN_PKG::fmt)

target_precompile_headers(Spatial.Res PRIVATE <iostream> <filesystem> <fstream> <string> <locale>)