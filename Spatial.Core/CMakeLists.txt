add_library(Spatial.Core
	src/hello.cpp
)

target_include_directories(Spatial.Core PUBLIC include)
target_link_libraries(Spatial.Core PUBLIC ${SPATIAL_CORE_DEPENDENCIES})