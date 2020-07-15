option(SPATIAL_ENABLE_GRAPHVIZ "Generates a depedency graph file. ('.dot' graphviz)" OFF)

if (SPATIAL_ENABLE_GRAPHVIZ)
	find_program(DOT_PROGRAM dot)
	
	if (DOT_PROGRAM)
		add_custom_target(Spatial.GraphViz
			COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/graphviz
			COMMAND ${CMAKE_COMMAND} "--graphviz=graphviz/spatial.dot" .
			COMMAND ${DOT_PROGRAM} -Tpng spatial.dot -o graphviz/dependency_graph.png
			WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
			COMMENT "Generating graphviz file: ${CMAKE_BINARY_DIR}/spatial.dot")
	else ()
		message(WARNING "Could not find graphviz dot executable")
	endif ()
endif ()