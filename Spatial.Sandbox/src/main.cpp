#include "SandboxInterface.h"
#include "Sandbox.h"

#include <spatial/core/Application.h>
#include <spatial/core/ApplicationEvents.h>
#include <spatial/core/Asset.h>

#include <spatial/render/RenderingSystem.h>
#include <spatial/input/InputSystem.h>
#include <spatial/ui/UserInterfaceSystem.h>

using namespace spatial;
using namespace std::filesystem;

int main(int argc, char* argv[])
{
	auto binaryPath = path{argv[0]}.parent_path();
	Asset::init(binaryPath / "assets");

	auto app = Application{};
	auto window = app.getWindowContext().createWindow(1280, 720, "Spatial Engine");
	auto rendering = RenderingSystem{std::move(window)};
	auto input = InputSystem{};
	auto ui = UserInterfaceSystem{rendering, path{"fonts"} / "Roboto-Medium.ttf"};
	auto sandbox = Sandbox{rendering};

	connect(app, &rendering, &input, &ui, &sandbox);

	return app.run();
}