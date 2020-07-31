#include "EditorSystem.h"

#include <argh.h>
#include <assets/generated.h>
#include <filesystem>
#include <spatial/spatial.h>

using namespace spatial;
namespace fs = std::filesystem;

struct SetupConfig
{
	std::string_view windowTitle;
	std::uint16_t windowWidth;
	std::uint16_t windowHeight;
};

void connect(EventQueue& ebus, Application& app, DesktopPlatformContext& windowContext);
int main(int argc, char* argv[])
{
	const auto args = argh::parser(argc, argv, argh::parser::PREFER_PARAM_FOR_UNREG_OPTION);
	const auto executablePath = fs::path{args[0]}.parent_path();

	auto config = SetupConfig{"Spatial Engine | Editor", 1280, 720};
	args({"-w", "--width"}) >> config.windowWidth;
	args({"-h", "--height"}) >> config.windowHeight;

	// clang-format off
	const auto resources = DirMapLoader{
		{"editor", MemoryLoader{
			{"fonts/Roboto_Medium.ttf", {ASSETS_ROBOTO_MEDIUM, ASSETS_ROBOTO_MEDIUM_SIZE}},
			{"materials/default.filamat", {ASSETS_DEFAULT, ASSETS_DEFAULT_SIZE}},
			{"textures/default_skybox/sh.txt", {ASSETS_SH, ASSETS_SH_SIZE}},
			{"textures/default_skybox/ibl.ktx", {ASSETS_DEFAULT_SKYBOX_IBL, ASSETS_DEFAULT_SKYBOX_IBL_SIZE}},
			{"textures/default_skybox/skybox.ktx", {ASSETS_DEFAULT_SKYBOX_SKYBOX, ASSETS_DEFAULT_SKYBOX_SKYBOX_SIZE}},
			{"materials/ui.filamat", {ASSETS_UI_BLIT, ASSETS_UI_BLIT_SIZE}}
		}},
		{"assets", AggregatorLoader{
			PhysicalDirLoader{executablePath / "assets"},
			PhysicalDirLoader{executablePath}
		}}
	};
	// clang-format on

	auto app = Application{};
	auto desktopContext = DesktopPlatformContext{};
	auto window = desktopContext.createWindow(config.windowWidth, config.windowHeight, config.windowTitle);

	auto input = InputSystem{window};
	auto rendering = RenderingSystem{fl::backend::Backend::OPENGL, window};

	auto ui = UserInterfaceSystem(rendering, window);
	ui.setDefaultMaterial(resources("editor/materials/ui.filamat").value());
	ui.setDefaultFont(resources("editor/fonts/Roboto_Medium.ttf").value());

	auto editor = EditorSystem{rendering, resources};

	// Connect all Systems to the Application Main Loop
	app >> desktopContext >> input >> rendering >> ui >> editor;

	// Connect Desktop Events to All Systems
	desktopContext >> app >> input >> rendering >> ui >> editor;

	// Connect Gui Render to Editor
	ui >> editor;

	return app.run();
}
