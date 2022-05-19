set(CPACK_PACKAGE_VENDOR "Luiz Gabriel (luizgabriel.info@gmail.com)")
set(CPACK_PACKAGE_DESCRIPTION "Spatial is a cross-platform C++ game engine created on top off google's filament rendering engine. This projects uses C++17 and modern cmake features.")
set(CMAKE_PROJECT_HOMEPAGE_URL "https://luizgabriel.github.io/Spatial.Engine/")
set(CPACK_VERBATIM_VARIABLES TRUE)
set(CPACK_CREATE_DESKTOP_LINKS TRUE)
set(CPACK_DMG_BACKGROUND_IMAGE ${PROJECT_SOURCE_DIR}/Spatial.Editor/assets/BundleBackground.png)
set(CPACK_PACKAGE_ICON ${PROJECT_SOURCE_DIR}/Spatial.Editor/assets/AppIcon.bmp)
set(CPACK_WIX_PRODUCT_ICON ${PROJECT_SOURCE_DIR}/Spatial.Editor/assets/AppIcon.ico)
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_RESOURCE_FILE_LICENSE ${PROJECT_SOURCE_DIR}/LICENSE.txt)
set(CPACK_ARCHIVE_COMPONENT_INSTALL ON)
set(CPACK_COMPONENTS_ALL binaries)

include(CPack)

cpack_add_component(binaries
        DISPLAY_NAME "Spatial Engine Binaries"
        DESCRIPTION "The Spatial Engine goes here. You need it to build your own games."
        GROUP Runtime)
cpack_add_component(libraries
        DISPLAY_NAME "Static Libraries"
        DESCRIPTION "Static libraries used to build programs with the Spatial Core"
        GROUP Development)
cpack_add_component(headers
        DISPLAY_NAME "C++ Headers"
        DESCRIPTION "C/C++ header files for using the Spatial Core"
        GROUP Development)