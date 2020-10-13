#include "Resources.h"

#include <spatial/assets/AggregatorLoader.h>
#include <spatial/assets/ResourcesLoader.h>
#include <spatial/assets/DirMapLoader.h>
#include <spatial/assets/MemoryLoader.h>
#include <spatial/assets/PhysicalDirLoader.h>
#include <assets/generated.h>

namespace spatial::editor
{

assets::AggregatorLoader gResourceLoader{};

assets::ResourceData load(const std::string_view resourceName)
{
	return gResourceLoader.load(resourceName);
}

void initAssets(const std::filesystem::path& executablePath)
{
	// clang-format off
	editor::gResourceLoader.emplace(
		assets::DirMapLoader{}
			.emplace("editor", assets::MemoryLoader{
				{"fonts/Roboto_Medium.ttf", {ASSETS_ROBOTO_MEDIUM, ASSETS_ROBOTO_MEDIUM_SIZE}},
				{"materials/default.filamat", {ASSETS_DEFAULT, ASSETS_DEFAULT_SIZE}},
				{"textures/uv.png", {ASSETS_UV, ASSETS_UV_SIZE}},
				{"textures/default_skybox/sh.txt", {ASSETS_SH, ASSETS_SH_SIZE}},
				{"textures/default_skybox/ibl.ktx", {ASSETS_DEFAULT_SKYBOX_IBL, ASSETS_DEFAULT_SKYBOX_IBL_SIZE}},
				{"textures/default_skybox/skybox.ktx", {ASSETS_DEFAULT_SKYBOX_SKYBOX, ASSETS_DEFAULT_SKYBOX_SKYBOX_SIZE}},
				{"materials/ui.mat", {ASSETS_UI_BLIT, ASSETS_UI_BLIT_SIZE}},
				{"meshes/plane.filamesh", {ASSETS_PLANE, ASSETS_PLANE_SIZE}},
				{"meshes/cube.filamesh", {ASSETS_CUBE, ASSETS_CUBE_SIZE}},
				{"meshes/sphere.filamesh", {ASSETS_SPHERE, ASSETS_SPHERE_SIZE}},
				{"meshes/cylinder.filamesh", {ASSETS_CYLINDER, ASSETS_CYLINDER_SIZE}},
			})
			.emplace("assets", assets::AggregatorLoader{}
				.emplace<assets::PhysicalDirLoader>(executablePath / "assets")
				.emplace<assets::PhysicalDirLoader>(executablePath))
	);
	// clang-format on
}

} // namespace spatial::editor