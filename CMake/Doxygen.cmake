option(SPATIAL_ENABLE_DOXYGEN "Enable Doxygen documentation builds of source." OFF)

if (SPATIAL_ENABLE_DOXYGEN)
    set(DOXYGEN_CALLER_GRAPH YES)
    set(DOXYGEN_CALL_GRAPH YES)
    set(DOXYGEN_COLLABORATION_GRAPH YES)
    set(DOXYGEN_EXTRACT_ALL YES)
    set(DOXYGEN_CLASS_DIAGRAMS YES)
    set(DOXYGEN_HIDE_UNDOC_RELATIONS NO)
    set(DOXYGEN_HAVE_DOT YES)
    set(DOXYGEN_CLASS_GRAPH YES)
    set(DOXYGEN_USE_MDFILE_AS_MAINPAGE ${PROJECT_SOURCE_DIR}/README.md)

    find_package(Doxygen REQUIRED dot)
    doxygen_add_docs(Spatial.Docs 
        ${PROJECT_SOURCE_DIR}/README.md 
        ${PROJECT_SOURCE_DIR}/Spatial.Core 
        ${PROJECT_SOURCE_DIR}/Spatial.Graphics
        ${PROJECT_SOURCE_DIR}/Spatial.Res
        ${PROJECT_SOURCE_DIR}/Spatial.UI
        ${PROJECT_SOURCE_DIR}/Spatial.Editor
    COMMENT "Generating doxygen for Spatial.Engine")
endif ()
