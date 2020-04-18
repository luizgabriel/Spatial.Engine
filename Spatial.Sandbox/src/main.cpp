#include "Sandbox.h"

#include <spatial/core/Initializer.h>
#include <spatial/core/Asset.h>
#include <spatial/core/System.h>

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

	return tryRun([]()
	{
		const auto config = getConfig("config/application.toml");

		auto app = Application{config};
		auto sandbox = System<Sandbox>{app, app.getRenderingSystem()};
		
		return app.run();
	});
}
