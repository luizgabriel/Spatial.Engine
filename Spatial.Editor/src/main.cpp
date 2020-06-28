#include "EditorSystem.h"

#include <spatial/init.h>
#include <streambuf>
#include <sstream>
#include <spatial/common/ResourceUtils.h>

using namespace spatial;
using namespace std::filesystem;

constexpr uint8_t buff[4] = { 0x00, 0x01, 0x02, 0x03 };

Asset::ResourceStream loadEmbedResource(const path& resourcePath)
{
	if (resourcePath == "config/application.toml")
		return MemoryStream{{buff}};

	throw std::runtime_error("Unknown embed resource.");
}

int main(int argc, char* argv[])
{
	Asset::define("assets", DefaultAssetLoader{path{argv[0]}.parent_path() / "assets"});
	Asset::define<&loadEmbedResource>("embed");

	const auto config = readConfigFile("embed://config/application.toml");

	return setup(config, [](auto& app, auto& services) {
		auto sandbox = System<EditorSystem>{app, services.rendering};

		return app.run();
	});
}
