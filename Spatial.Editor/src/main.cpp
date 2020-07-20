#include "EditorSystem.h"

#include <argh.h>
#include <filesystem>
#include <generated.h>
#include <spatial/spatial.h>
#include <spatial/ui/UserInterfaceSystem.h>

using namespace spatial;
namespace fs = std::filesystem;

const std::string_view robotoFontData = {reinterpret_cast<const char*>(GENERATED_ROBOTO_MEDIUM_DATA), static_cast<size_t>(GENERATED_ROBOTO_MEDIUM_SIZE)};
const std::string_view defaultMaterialData = {reinterpret_cast<const char*>(GENERATED_DEFAULT_DATA), static_cast<size_t>(GENERATED_DEFAULT_OFFSET)};
const std::string_view defaultUiMaterialData = {reinterpret_cast<const char*>(GENERATED_UI_BLIT_DATA), static_cast<size_t>(GENERATED_UI_BLIT_SIZE)};

auto buildAssetTree(std::filesystem::path executablePath)
{
	// clang-format off
	return DirMapLoader{
		{"editor", MemoryLoader{
			{"fonts/Roboto_Medium.ttf", robotoFontData},
			{"materials/default.filamat", defaultMaterialData},
			{"materials/ui.filamat", defaultUiMaterialData}
		}},
		{"assets", AggregatorLoader{
			PhysicalDirLoader{executablePath / "assets"},
			PhysicalDirLoader{executablePath}
		}}
	};
	// clang-format on
}

int main(int argc, char* argv[])
{
	const auto args = argh::parser(argc, argv, argh::parser::PREFER_PARAM_FOR_UNREG_OPTION);
	const auto executablePath = fs::path{args[0]}.parent_path();

	auto setupConfig = SetupConfig{"Spatial Engine | Editor", 1280, 720};
	args({"-w", "--width"}) >> setupConfig.windowWidth;
	args({"-h", "--height"}) >> setupConfig.windowHeight;

	return setup(setupConfig, [&](auto& app, auto& services) {
		auto ui = System<UserInterfaceSystem>(app, services.rendering, services.window);
		ui->setDefaultMaterial(defaultUiMaterialData);
		ui->setDefaultFont(robotoFontData);

		const auto resources = buildAssetTree(executablePath);
		auto sandbox = System<EditorSystem>{app, resources, services.rendering};

		return app.run();
	});
}