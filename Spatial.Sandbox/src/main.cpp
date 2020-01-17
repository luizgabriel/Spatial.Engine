#include "SandboxInterface.h"
#include "Sandbox.h"

#include <filesystem>

namespace fl = filament;
using namespace spatial;
using namespace std::filesystem;
using namespace spatial::core;
using namespace spatial::render;
using namespace spatial::input;
using namespace spatial::sandbox;

int main(int arc, char *argv[])
{
    Asset::init(path{argv[0]}.parent_path() / "assets");

    auto app = Application{};
    auto window = app.getWindowContext().createWindow(1280, 720, "Spatial Engine");
    auto rendering = RenderingSystem{app, std::move(window)};
    auto input = InputSystem{app};
    auto sandboxInterface = SandboxInterface{app};
    auto sandbox = Sandbox{app, rendering, sandboxInterface};
    auto ui = UserInterfaceSystem{app, rendering};

    return app.run();
}