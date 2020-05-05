#include "Sandbox.h"

#include <spatial/core/Initializer.h>
#include <spatial/core/Asset.h>
#include <spatial/core/System.h>
#include <spatial/core/ConfigurationLoader.h>

using namespace spatial;
using namespace std::filesystem;

int main(int argc, char* argv[])
{
	Asset::init(path{argv[0]}.parent_path() / "assets");

	return tryRun([]()
	{
		const auto config = readConfigFile("config/application.toml");

		auto app = Application{config};
		auto sandbox = System<Sandbox>{
			app,
			app.getRenderingSystem().getEngine(),
			app.getRenderingSystem().getMainCamera(),
			app.getRenderingSystem().getMainView()
		};
		
		return app.run();
	});
}
