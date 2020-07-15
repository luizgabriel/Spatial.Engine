#include "EditorSystem.h"

#include <argh.h>
#include <filesystem>
#include <spatial/spatial.h>
#include <spatial/ui/UserInterfaceSystem.h>
#include <generated.h>

using namespace spatial;
namespace fs = std::filesystem;

template <typename BufferType = BasicBufferType>
auto buildAssetTree(std::filesystem::path executablePath)
{
	// clang-format off
	return DirMapLoader<BufferType>{
		{"editor"_hs, MemoryLoader<BufferType>{
			{"fonts/Roboto_Medium.ttf"_hs, {GENERATED_ROBOTO_MEDIUM_DATA, static_cast<size_t>(GENERATED_ROBOTO_MEDIUM_SIZE)}},
			{"materials/default.filamat"_hs, {GENERATED_DEFAULT_DATA, static_cast<size_t>(GENERATED_DEFAULT_OFFSET)}},
			{"materials/ui.filamat"_hs, {GENERATED_UI_BLIT_DATA, static_cast<size_t>(GENERATED_UI_BLIT_SIZE)}}
		}},
		{"assets"_hs, AggregatorLoader<BufferType>{
			PhysicalDirLoader<BufferType>{executablePath / "assets"},
			PhysicalDirLoader<BufferType>{executablePath}
		}}
	};
	// clang-format on
}

int main(int argc, char* argv[])
{
	const auto args = argh::parser(argc, argv, argh::parser::PREFER_PARAM_FOR_UNREG_OPTION);
	const auto executablePath = fs::path{args[0]};
	auto setupConfig = SetupConfig{"Spatial Engine | Editor", 1280, 720};
	args({"-w", "--width"}) >> setupConfig.windowWidth;
	args({"-h", "--height"}) >> setupConfig.windowHeight;

	const auto assets = buildAssetTree(executablePath);
	auto uiFont = asyncReadAll(assets, "editor/fonts/Roboto_Medium.ttf");
	auto uiMaterial = asyncReadAll(assets, "editor/materials/ui.filamat");

	return setup(setupConfig, [&](auto& app, auto& services) {
		auto ui = System<UserInterfaceSystem>(app, services.rendering, services.window);

		if (auto uiFontData = uiFont.get(); uiFontData)
			ui->setDefaultFont({uiFontData->data(), uiFontData->size()});

		if (auto uiMaterialData = uiMaterial.get(); uiMaterialData)
			ui->setDefaultMaterial({uiMaterialData->data(), uiMaterialData->size()});

		auto sandbox = System<EditorSystem>{app, services.rendering};

		return app.run();
	});
}