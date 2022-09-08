add_subdirectory(assets)

set(SPATIAL_EDITOR_MACOS_ICON ${CMAKE_CURRENT_SOURCE_DIR}/assets/AppIcon.icns)
set(SPATIAL_EDITOR_IMGUI_SETTINGS ${CMAKE_CURRENT_SOURCE_DIR}/assets/config/imgui.ini)

set_source_files_properties(${SPATIAL_EDITOR_MACOS_ICON} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
set_source_files_properties(${SPATIAL_EDITOR_IMGUI_SETTINGS} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")

add_executable(Spatial.Editor MACOSX_BUNDLE WIN32
        src/main.cpp
        src/EditorSystem.cpp
        src/EditorCameraController.h
        src/EditorCameraController.cpp
        src/Jobs.h
        src/Serialization.h
        src/Serialization.cpp
        src/EditorSettings.cpp
        src/Settings.h
        src/FileSystem.h src/FileSystem.cpp
        src/ecs/Materials.h src/ecs/Materials.cpp
        src/ecs/Components.h src/ecs/Components.cpp
        src/ui/Icons.h
        src/ui/Components.h src/ui/Components.cpp
        src/ui/Managers.h src/ui/Managers.cpp
        src/ui/Modals.h src/ui/Modals.cpp
        src/ui/Menu.h src/ui/Menu.cpp
        src/ui/EntityProperties.h src/ui/EntityProperties.cpp
        src/ui/DragAndDrop.h src/ui/DragAndDrop.cpp
        src/ui/Search.h src/ui/Search.cpp
        src/ui/MaterialInputs.h src/ui/MaterialInputs.cpp
        src/ui/FilesExplorer.h src/ui/FilesExplorer.cpp
        ${SPATIAL_EDITOR_MACOS_ICON}
        ${SPATIAL_EDITOR_IMGUI_SETTINGS})

set_target_properties(Spatial.Editor PROPERTIES MACOSX_BUNDLE_ICON_FILE "AppIcon.icns")

set_project_warnings(Spatial.Editor)
enable_sanitizers(Spatial.Editor)
target_precompile_headers(Spatial.Editor REUSE_FROM Spatial.Core)

target_link_libraries(Spatial.Editor PRIVATE
        Spatial.Editor.Resources
        Spatial.Core
        Spatial.Graphics
        Spatial.UI
        CONAN_PKG::fmt
        CONAN_PKG::argh)

cmake_policy(SET CMP0091 NEW)
set_property(
        TARGET Spatial.Editor
        PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
        VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:Spatial.Editor>"
        VS_DEBUGGER_COMMAND "$<TARGET_FILE:Spatial.Editor>")

install(TARGETS Spatial.Editor
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        BUNDLE DESTINATION .
        COMPONENT binaries)

if (WIN32)
    install(FILES assets/config/imgui.ini
            DESTINATION ${CMAKE_INSTALL_BINDIR}
            COMPONENT configs)
endif ()