
add_mesh_resources(Spatial.Editor.Meshes
    SOURCES
        models/debug_cube.fbx
    OUTPUT models
    DEPENDEES Spatial.Editor
)

add_skybox_resources(Spatial.Editor.Skybox
    SOURCES
        textures/pillars_2k.hdr
        textures/railway_bridge_2k.hdr
    FORMAT ktx
    RESOLUTION 4K
    OUTPUT textures
    DEPENDEES Spatial.Editor
)

add_embed_resources(Spatial.Editor.Embed
    SOURCES
        materials/ui_blit.mat
        config/application.toml
        fonts/Roboto-Regular.ttf
        fonts/Roboto-Medium.ttf
        textures/debug_cube.png
        textures/spatial_engine_logo.png
    OUTPUT generated/
    PACKAGE resources
    DEPENDEES Spatial.Editor
)

install(TARGETS Spatial.Editor RUNTIME DESTINATION bin)
install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} DESTINATION assets)