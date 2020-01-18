#include "SandboxInterface.h"
#include "Sandbox.h"

#include <filesystem>

namespace fl = filament;
using namespace spatial;
using namespace std::filesystem;
using namespace spatial::core;
using namespace spatial::render;
using namespace spatial::input;
using namespace spatial::ui;
using namespace spatial::sandbox;

int main(int arc, char *argv[])
{
    auto binaryPath = path{argv[0]}.parent_path();
    Asset::init(binaryPath.parent_path() / "assets");

    auto app = Application{};
    auto window = app.getWindowContext().createWindow(1280, 720, "Spatial Engine");
    auto rendering = RenderingSystem{app, std::move(window)};
    auto input = InputSystem{app};
    auto sandbox = Sandbox{app, rendering};
    auto ui = UserInterfaceSystem{app, rendering};

    return app.run();
}