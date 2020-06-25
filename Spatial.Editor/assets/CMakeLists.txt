add_material_resources(Spatial.Editor
    SOURCES
        assets/materials/default.mat
        assets/materials/ui_blit.mat
    OUTPUT assets/materials
)

add_mesh_resources(Spatial.Editor
    SOURCES
        assets/models/debug_cube.fbx
    OUTPUT
        assets/models
)

add_skybox_resources(Spatial.Editor
    SOURCES
        assets/textures/pillars_2k.hdr
        assets/textures/railway_bridge_2k.hdr
        FORMAT ktx
        RESOLUTION 4K
    OUTPUT assets/textures
)

add_resources(Spatial.Editor
    SOURCES
        assets/config/application.toml
    OUTPUT
        assets/config
)

add_resources(Spatial.Editor
    SOURCES
        assets/fonts/Roboto-Regular.ttf
        assets/fonts/Roboto-Medium.ttf
    OUTPUT
        assets/fonts
)

add_resources(Spatial.Editor
    SOURCES
        assets/textures/debug_cube.png
        assets/textures/spatial_engine_logo.png
    OUTPUT
        assets/textures
)

install(TARGETS Spatial.Editor RUNTIME DESTINATION bin)
install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/assets/ DESTINATION assets)