add_library(Spatial.UI

        ## HEADERS
        include/spatial/ui/system/ImGuiHelpers.h
        include/spatial/ui/system/UserInterfaceSystem.h
        include/spatial/ui/system/ImGuiRenderer.h
        include/spatial/ui/system/UserInterfaceInput.h

        include/spatial/ui/components/Window.h
        include/spatial/ui/components/Collapse.h
        include/spatial/ui/components/DockSpace.h
        include/spatial/ui/components/Components.h
        include/spatial/ui/components/MenuBar.h
        include/spatial/ui/components/Menu.h
        include/spatial/ui/components/PopupModal.h
        include/spatial/ui/components/FilesExplorer.h
        include/spatial/ui/components/DirectionInput.h
        include/spatial/ui/components/VectorInput.h
        include/spatial/ui/components/DragAndDrop.h
        include/spatial/ui/components/Popup.h
        include/spatial/ui/components/Search.h
        include/spatial/ui/components/Combo.h
        include/spatial/ui/components/SceneView.h
        include/spatial/ui/components/MaterialInputs.h
        include/spatial/ui/components/PreviewTexture.h

        include/spatial/ui/components/Icons.h

        include/spatial/ui/components/styles/BorderlessWindowStyle.h
        include/spatial/ui/components/styles/WindowPaddingStyle.h

        ## SOURCES
        src/system/ImGuiHelpers.cpp
        src/system/UserInterfaceSystem.cpp
        src/system/UserInterfaceRenderer.cpp
        src/system/UserInterfaceInput.cpp

        src/components/Window.cpp
        src/components/Collapse.cpp
        src/components/DockSpace.cpp
        src/components/Components.cpp
        src/components/MenuBar.cpp
        src/components/Menu.cpp
        src/components/PopupModal.cpp
        src/components/FilesExplorer.cpp
        src/components/VectorInput.cpp
        src/components/DragAndDrop.cpp
        src/components/DirectionInput.cpp
        src/components/Popup.cpp
        src/components/Combo.cpp
        src/components/SceneView.cpp
        src/components/Search.cpp
        src/components/MaterialInputs.cpp
        src/components/PreviewTexture.cpp

        src/components/styles/BorderlessWindowStyle.cpp
        src/components/styles/WindowPaddingStyle.cpp)

set_project_warnings(Spatial.UI)
enable_sanitizers(Spatial.UI)

cmake_policy(SET CMP0091 NEW)
set_property(
        TARGET Spatial.UI
        PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

target_include_directories(Spatial.UI
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include>
        PRIVATE src)

# LINK PLATFORM INDEPENDENT DEPENDENCIES
target_link_libraries(Spatial.UI
        PUBLIC
        Spatial.Core
        CONAN_PKG::imgui)

install(
        TARGETS Spatial.UI
        EXPORT Spatial.Export
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        COMPONENT libraries
)

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} COMPONENT headers)