#include "FileSystem.h"
#include <assets/generated.h>
#include <spatial/resources/MemoryFileSystem.h>
#include <spatial/resources/PhysicalFileSystem.h>

namespace spatial::editor
{

SharedFileSystem createEditorFileSystem()
{
	auto editorFs = std::make_shared<MemoryFileSystem>();
	#if defined(SPATIAL_PLATFORM_WINDOWS)
	editorFs->define("AppIcon.png", {ASSETS_APPICON_PNG, ASSETS_APPICON_PNG_SIZE});
	#endif
	editorFs->define("meshes/cube.filamesh", {ASSETS_CUBE_FILAMESH, ASSETS_CUBE_FILAMESH_SIZE});
	editorFs->define("meshes/sphere.filamesh", {ASSETS_SPHERE_FILAMESH, ASSETS_SPHERE_FILAMESH_SIZE});
	editorFs->define("meshes/plane.filamesh", {ASSETS_PLANE_FILAMESH, ASSETS_PLANE_FILAMESH_SIZE});
	editorFs->define("meshes/cylinder.filamesh", {ASSETS_CYLINDER_FILAMESH, ASSETS_CYLINDER_FILAMESH_SIZE});
	editorFs->define("materials/ui_blit.filamat", {ASSETS_UI_BLIT_FILAMAT, ASSETS_UI_BLIT_FILAMAT_SIZE});
	editorFs->define("materials/standard_lit.filamat", {ASSETS_STANDARD_LIT_FILAMAT, ASSETS_STANDARD_LIT_FILAMAT_SIZE});
	editorFs->define("materials/color.filamat", {ASSETS_COLOR_FILAMAT, ASSETS_COLOR_FILAMAT_SIZE});
	editorFs->define("materials/skybox.filamat", {ASSETS_SKYBOX_FILAMAT, ASSETS_SKYBOX_FILAMAT_SIZE});
	editorFs->define("materials/grid.filamat", {ASSETS_GRID_FILAMAT, ASSETS_GRID_FILAMAT_SIZE});
	editorFs->define("fonts/roboto_medium.ttf", {ASSETS_ROBOTO_MEDIUM_TTF, ASSETS_ROBOTO_MEDIUM_TTF_SIZE});
	editorFs->define("textures/icons.png", {ASSETS_ICONS_PNG, ASSETS_ICONS_PNG_SIZE});
	editorFs->define("textures/skybox/ibl.ktx", {ASSETS_DEFAULT_SKYBOX_IBL_KTX, ASSETS_DEFAULT_SKYBOX_IBL_KTX_SIZE});
	editorFs->define("textures/skybox/texture.ktx",
					 {ASSETS_DEFAULT_SKYBOX_SKYBOX_KTX, ASSETS_DEFAULT_SKYBOX_SKYBOX_KTX_SIZE});
	editorFs->define("textures/skybox/sh.txt", {ASSETS_SH_TXT, ASSETS_SH_TXT_SIZE});

	return editorFs;
}

SharedFileSystem createDefaultFileSystem(const editor::Settings& settings)
{
	auto fileSystem = std::make_shared<AggregateFileSystem>();
	fileSystem->mount("editor", createEditorFileSystem());
	fileSystem->mount<PhysicalFileSystem>("project", settings.projectFolder);

	return fileSystem;
}

} // namespace spatial::editor