#include "Editor.h"
#include "SceneEditorSystem.h"

#include <argh.h>
#include <filesystem>

#include <spatial/core/Application.h>
#include <spatial/core/ApplicationUtils.h>
#include <spatial/desktop/PlatformEventUtils.h>
#include <spatial/desktop/Window.h>
#include <spatial/input/Input.h>
#include <spatial/input/InputSystem.h>
#include <spatial/input/InputSystemUtils.h>
#include <spatial/ui/UserInterfaceSystem.h>
#include <spatial/ui/UserInterfaceUtils.h>

using namespace spatial;
namespace fs = std::filesystem;

struct SetupConfig
{
	std::string_view windowTitle;
	std::uint16_t windowWidth;
	std::uint16_t windowHeight;
};

int main(int argc, char* argv[])
{
	const auto args = argh::parser(argc, argv, argh::parser::PREFER_PARAM_FOR_UNREG_OPTION);
	const auto executablePath = fs::path{args[0]}.parent_path();

	editor::initAssets(executablePath);

	auto config = SetupConfig{"Spatial Engine | Editor", 1280, 720};
	args({"-w", "--width"}) >> config.windowWidth;
	args({"-h", "--height"}) >> config.windowHeight;

	auto app = Application{};
	auto desktopContext = DesktopPlatformContext{};
	auto window = desktopContext.createWindow(config.windowWidth, config.windowHeight, config.windowTitle);

	auto input = InputSystem{};
	Input::with(input.getState());

	auto rendering = RenderingSystem{fl::backend::Backend::OPENGL, window};

	auto ui = UserInterfaceSystem{rendering, window};
	ui.setMaterial(editor::load("editor/materials/ui.mat").value());
	ui.setFont(editor::load("editor/fonts/Roboto_Medium.ttf").value());

	auto editor = editor::SceneEditorSystem{rendering};

	// Connect all Systems to the Application Main Loop
	app >> desktopContext >> input >> rendering >> ui >> editor;

	// Connect Desktop Events to All Systems
	desktopContext >> app >> input >> rendering >> ui >> editor;

	// Connect Gui Render to Editor
	ui >> editor;

	// Connect Input to Window
	input >> window;

	return app.run();
}
