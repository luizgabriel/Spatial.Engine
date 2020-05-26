#include "Sandbox.h"

#include <spatial/init.h>

using namespace spatial;
using namespace std::filesystem;

int main(int argc, char* argv[])
{
	Asset::init(path{argv[0]}.parent_path() / "assets");
	const auto config = readConfigFile("config/application.toml");

	return setup(config, [](auto& app, auto& services) {
		auto sandbox = System<Sandbox>{app, services.rendering};

		return app.run();
	});
}
