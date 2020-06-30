#include "EditorSystem.h"

#include <spatial/init.h>
#include <streambuf>
#include <sstream>
#include <unordered_map>
#include <filesystem>
#include <spatial/common/ResourceUtils.h>
#include <spatial/common/StringHelpers.h>
#include <spatial/core/Logger.h>

using namespace spatial;
namespace fs = std::filesystem::path;

constexpr char _roboto_medium_ttf[] = {1, 2, 3};
constexpr char _materials_default[] = {1, 2, 3};

auto gLogger = createDefaultLogger();

void mountVirtualFileSystem(argh::parser& args);

int main(int argc, char* argv[])
{
	const auto args = argh::parser(argc, argv);
	mountVirtualFileSystem(args);

	const auto setupConfig = SetupConfig{
		.windowTitle = "Spatial Engine | Editor",
		.windowWidth = args({"w", "width"}, 1280),
		.windowHeight = args({"h", "height"}, 1280),
		.uiFontResourceId = "editor/fonts/Roboto-Medium.ttf"_sh32
	};

	return setup(setupConfig, [](auto& app, auto& services) {
		auto sandbox = System<EditorSystem>{app, services.rendering};

		return app.run();
	});
}

void mountVirtualFileSystem(const argh::parser& args)
{
	Asset::mount<MemoryDisk>("editor", {
		{"fonts/Roboto-Medium.ttf", _roboto_medium_ttf},
		{"materials/default", _materials_default}
	});

	for (auto i = 1, auto file = args[i]; !file.empty(); i++) {
		auto filePath = fs::path{file};
		if (fs::exists(filePath)) {
			if (file.ends_with(".zip"))
				Asset::mount<ArchiveDisk>("assets", filePath);
			else if (file.ends_with(".json"))
				Asset::mount<ResourceDescriberDisk>("assets", filePath);
			else
				Asset::mount<PhysicalDisk>("assets", filePath);
		} else {
			gLogger.error("Could not found: {}", file);
		}
	}
}