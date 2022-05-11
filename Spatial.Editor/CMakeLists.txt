add_subdirectory(assets)

set(SPATIAL_EDITOR_WINDOWS_ICON ${CMAKE_CURRENT_SOURCE_DIR}/assets/AppIcon.rc)
set(SPATIAL_EDITOR_MACOS_ICON ${CMAKE_CURRENT_SOURCE_DIR}/assets/AppIcon.icns)
set(SPATIAL_EDITOR_IMGUI_SETTINGS ${CMAKE_CURRENT_SOURCE_DIR}/assets/config/imgui.ini)

set_source_files_properties(${SPATIAL_EDITOR_MACOS_ICON} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
set_source_files_properties(${SPATIAL_EDITOR_IMGUI_SETTINGS} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")

add_executable(Spatial.Editor MACOSX_BUNDLE WIN32
        src/main.cpp
        src/EditorSystem.cpp
        src/EditorCameraController.h
        src/EditorCameraController.cpp
        src/EditorCamera.h
        src/Jobs.h
        src/Materials.h
        src/Materials.cpp
        src/Serialization.h
        src/Serialization.cpp
        src/CustomComponents.h
        src/CustomComponents.cpp
        src/EditorSettings.cpp
        src/Settings.h
        src/FileSystem.h
        src/FileSystem.cpp
        ${SPATIAL_EDITOR_MACOS_ICON}
        ${SPATIAL_EDITOR_IMGUI_SETTINGS}
        ${SPATIAL_EDITOR_WINDOWS_ICON})

set_target_properties(Spatial.Editor PROPERTIES MACOSX_BUNDLE_ICON_FILE "AppIcon.icns")

set_project_warnings(Spatial.Editor)
enable_sanitizers(Spatial.Editor)
target_precompile_headers(Spatial.Editor REUSE_FROM Spatial.Core)

target_link_libraries(Spatial.Editor PRIVATE
        Spatial.Editor.Resources
        Spatial.Core
        Spatial.UI
        CONAN_PKG::fmt
        CONAN_PKG::argh)

cmake_policy(SET CMP0091 NEW)
set_property(
        TARGET Spatial.Editor
        PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

install(TARGETS Spatial.Editor
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        BUNDLE DESTINATION .
        COMPONENT binaries)

if (WIN32)
        configure_file(${CMAKE_CURRENT_SOURCE_DIR}/assets/AppIcon.ico ${CMAKE_CURRENT_BINARY_DIR}/ COPYONLY)
        configure_file(${SPATIAL_EDITOR_IMGUI_SETTINGS} ${CMAKE_CURRENT_BINARY_DIR}/ COPYONLY)
        install(FILES
                assets/AppIcon.ico
                assets/config/imgui.ini
        DESTINATION ${CMAKE_INSTALL_BINDIR}
        COMPONENT configs)
endif()