add_executable(Spatial.Res src/main.cpp)
set_project_warnings(Spatial.Res)
enable_sanitizers(Spatial.Res)

find_package(argh REQUIRED)
find_package(fmt REQUIRED)

target_link_libraries(Spatial.Res PUBLIC argh fmt::fmt)

target_precompile_headers(Spatial.Res PRIVATE <iostream> <filesystem> <fstream> <string> <locale>)