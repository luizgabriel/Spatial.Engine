#include "Sandbox.h"

#include <spatial/init.h>

using namespace spatial;
using namespace std::filesystem;

int main(int argc, char* argv[])
{
	Asset::init(path{argv[0]}.parent_path() / "assets");
	const auto config = readConfigFile("config/application.toml");

	return start(config, [](auto& app, auto& renderingSystem)
	{
		auto sandbox = System<Sandbox>{app,
			 renderingSystem.getEngine(),
			 renderingSystem.getMainCamera(),
			 renderingSystem.getMainView()
		 };
	});
}
