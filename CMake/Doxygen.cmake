option(SPATIAL_ENABLE_DOXYGEN "Enable Doxygen documentation builds of source." OFF)

if (SPATIAL_ENABLE_DOXYGEN)
	set(DOXYGEN_CALLER_GRAPH YES)
	set(DOXYGEN_CALL_GRAPH YES)
	set(DOXYGEN_EXTRACT_ALL YES)
	set(DOXYGEN_COLLABORATION_GRAPH YES)
	set(DOXYGEN_EXTRACT_ALL YES)
	set(DOXYGEN_CLASS_DIAGRAMS YES)
	set(DOXYGEN_HIDE_UNDOC_RELATIONS NO)
	set(DOXYGEN_HAVE_DOT YES)
	set(DOXYGEN_CLASS_GRAPH YES)
	
	set(DOXYGEN_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/docs)
	
	find_package(Doxygen REQUIRED dot)
	doxygen_add_docs(Spatial.Core.Docs ${PROJECT_SOURCE_DIR}/Spatial.Core COMMENT "Doxygen has been setup and documentation is now available.")
endif ()
