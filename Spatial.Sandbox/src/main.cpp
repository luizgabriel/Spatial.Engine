#include "SandboxInterface.h"
#include "Sandbox.h"

#include <spatial/core/Application.h>
#include <spatial/core/ApplicationEvents.h>
#include <spatial/core/Asset.h>
#include <spatial/core/SubSystem.h>

#include <spatial/render/RenderingSystem.h>
#include <spatial/input/InputSystem.h>
#include <spatial/ui/UserInterfaceSystem.h>

using namespace spatial;
using namespace std::filesystem;

int main(int argc, char* argv[]) 
try {
	const auto binaryPath = path{argv[0]}.parent_path();
	Asset::init(binaryPath / "assets");

	auto app = Application{};
	auto window = app.getWindowContext().createWindow(1280, 720, "Spatial Engine");
	auto rendering = SubSystem<RenderingSystem>{app, std::move(window)};
	auto input = SubSystem<InputSystem>{app};
	auto ui = SubSystem<UserInterfaceSystem>{app, rendering.get(), path{"fonts"} / "Roboto-Medium.ttf"};
	auto sandbox = SubSystem<Sandbox>{app, rendering.get()};
	
	return app.run();
}
catch (const std::runtime_error& e)
{
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "[Spatial Engine] Something went wrong", e.what(), nullptr);

	return -1;
}
