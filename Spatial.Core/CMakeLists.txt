add_library(Spatial.Core
	src/desktop/Window.cpp
	src/rendering/RenderingSubsystem.cpp
	src/EntryPoint.cpp
)

target_include_directories(Spatial.Core PUBLIC include)
target_link_libraries(Spatial.Core PUBLIC ${SPATIAL_CORE_DEPENDENCIES})
mathfu_configure_flags(Spatial.Core)

set_property(TARGET Spatial.Core PROPERTY CXX_STANDARD 17)
set_property(TARGET Spatial.Core PROPERTY CXX_STANDARD_REQUIRED ON)