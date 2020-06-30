add_executable(Spatial.ResourceCompiler
    src/main.cpp
)
set_project_warnings(Spatial.ResourceCompiler)
enable_sanitizers(Spatial.ResourceCompiler)
target_precompile_headers(Spatial.ResourceCompiler PRIVATE <iostream>)