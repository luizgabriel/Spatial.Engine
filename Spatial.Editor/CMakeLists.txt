add_subdirectory(assets)

# set(SPATIAL_EDITOR_MACOS_ICON ${CMAKE_CURRENT_SOURCE_DIR}/assets/AppIcon.icns)
# set_source_files_properties(${SPATIAL_EDITOR_MACOS_ICON} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")

add_executable(Spatial.Editor
        src/main.cpp
        src/SceneEditorSystem.cpp
        src/EditorCameraController.h
        src/EditorCameraController.cpp
        src/EditorCamera.h
        src/Jobs.h
        src/DefaultMaterial.h
        src/DefaultMaterial.cpp
        src/Serialization.h
        src/Serialization.cpp
        src/CustomComponents.h
        src/CustomComponents.cpp
        src/EditorSettings.cpp
        src/Settings.h
        ${SPATIAL_EDITOR_MACOS_ICON})

# set_target_properties(Spatial.Editor PROPERTIES MACOSX_BUNDLE_ICON_FILE "AppIcon.icns")

set_project_warnings(Spatial.Editor)
enable_sanitizers(Spatial.Editor)
target_precompile_headers(Spatial.Editor REUSE_FROM Spatial.Core)
target_link_libraries(Spatial.Editor PUBLIC
        Spatial.Core
        Spatial.UI
        CONAN_PKG::fmt
        CONAN_PKG::argh)

target_link_libraries(Spatial.Editor PRIVATE Spatial.Editor.Resources)

cmake_policy(SET CMP0091 NEW)
set_property(
        TARGET Spatial.Editor
        PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

install(TARGETS Spatial.Editor RUNTIME DESTINATION bin BUNDLE DESTINATION .)

