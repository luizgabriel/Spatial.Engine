#include "FileSystem.h"
#include <assets/generated.h>
#include <spatial/resources/MemoryFileSystem.h>

namespace spatial::editor
{

SharedFileSystem createEditorFileSystem()
{
	auto meshes = std::make_shared<MemoryFileSystem>(MemoryFileSystem{{
		{"cube.filamesh", toString({ASSETS_CUBE_FILAMESH, ASSETS_CUBE_FILAMESH_SIZE})},
		{"sphere.filamesh", toString({ASSETS_SPHERE_FILAMESH, ASSETS_SPHERE_FILAMESH_SIZE})},
		{"plane.filamesh", toString({ASSETS_PLANE_FILAMESH, ASSETS_PLANE_FILAMESH_SIZE})},
		{"cylinder.filamesh", toString({ASSETS_CYLINDER_FILAMESH, ASSETS_CYLINDER_FILAMESH_SIZE})},
	}});

	auto materials = std::make_shared<MemoryFileSystem>(MemoryFileSystem{{
		{"ui_blit.filamat", toString({ASSETS_UI_BLIT_FILAMAT, ASSETS_UI_BLIT_FILAMAT_SIZE})},
		{"standard_lit.filamat", toString({ASSETS_STANDARD_LIT_FILAMAT, ASSETS_STANDARD_LIT_FILAMAT_SIZE})},
		{"color.filamat", toString({ASSETS_COLOR_FILAMAT, ASSETS_COLOR_FILAMAT_SIZE})},
		{"skybox.filamat", toString({ASSETS_SKYBOX_FILAMAT, ASSETS_SKYBOX_FILAMAT_SIZE})},
		{"grid.filamat", toString({ASSETS_GRID_FILAMAT, ASSETS_GRID_FILAMAT_SIZE})},
	}});

	auto fonts = std::make_shared<MemoryFileSystem>(MemoryFileSystem{{
		{"roboto_medium.ttf", toString({ASSETS_ROBOTO_MEDIUM_TTF, ASSETS_ROBOTO_MEDIUM_TTF_SIZE})},
	}});

	auto icons = std::make_shared<MemoryFileSystem>(MemoryFileSystem{{
		{"AppIcon.png", toString({ASSETS_APPICON_PNG, ASSETS_APPICON_PNG_SIZE})},
		{"icons.png", toString({ASSETS_ICONS_PNG, ASSETS_ICONS_PNG_SIZE})},
	}});

	auto skybox = std::make_shared<MemoryFileSystem>(MemoryFileSystem{{
		{"ibl.ktx", toString({ASSETS_DEFAULT_SKYBOX_IBL_KTX, ASSETS_DEFAULT_SKYBOX_IBL_KTX_SIZE})},
		{"texture.ktx", toString({ASSETS_DEFAULT_SKYBOX_SKYBOX_KTX, ASSETS_DEFAULT_SKYBOX_SKYBOX_KTX_SIZE})},
		{"sh.txt", toString({ASSETS_SH_TXT, ASSETS_SH_TXT_SIZE})},
	}});

	auto textures = std::make_shared<AggregateFileSystem>(AggregateFileSystem{{
		{"icons", icons},
		{"skybox", skybox},
	}});

	return std::make_shared<AggregateFileSystem>(AggregateFileSystem{{
		{"meshes", meshes},
		{"materials", materials},
		{"fonts", fonts},
		{"textures", textures},
	}});
}

std::optional<SharedFileSystem> createProjectFileSystem(const editor::Settings& settings)
{
	if (!settings.projectFolder)
		return std::nullopt;
	return std::make_shared<PhysicalFileSystem>(*settings.projectFolder);
}

} // namespace spatial::editor