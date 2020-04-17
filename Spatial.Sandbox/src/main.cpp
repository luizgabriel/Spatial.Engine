#include "SandboxInterface.h"
#include "Sandbox.h"

#include <spatial/core/Initializer.h>
#include <spatial/core/Asset.h>
#include <spatial/core/System.h>
#include <spatial/core/CoreSystemBundle.h>
#include <spatial/ui/UserInterfaceSystem.h>

#include <fstream> // Required for parse_file

using namespace spatial;
using namespace std::filesystem;

auto getConfig(const path& filePath)
{
	const auto configPath = Asset::absolute(filePath).generic_u8string();
	return toml::parse_file(configPath);
}

int main(int argc, char* argv[])
{
	Asset::init(path{argv[0]}.parent_path() / "assets");

	auto app = Application{};

	return tryRun([&app]()
	{
		auto config = getConfig("config/application.toml");
		
		auto core = CoreSystemBundle{app, config};
		auto sandbox = System<Sandbox>{app, core.getRenderingSystem()};
		return app.run();
	});
}
