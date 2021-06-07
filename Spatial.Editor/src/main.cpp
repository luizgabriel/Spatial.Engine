#include "SceneEditorSystem.h"

#include <argh.h>
#include <ghc/filesystem.hpp>

#include <assets/generated.h>
#include <spatial/core/Application.h>
#include <spatial/core/ApplicationUtils.h>
#include <spatial/desktop/InputSystem.h>
#include <spatial/desktop/PlatformEventUtils.h>
#include <spatial/desktop/Window.h>
#include <spatial/render/RenderingSystem.h>
#include <spatial/render/RenderingSystemUtils.h>
#include <spatial/render/ResourceLoaders.h>
#include <spatial/ui/ImGuiHelpers.h>
#include <spatial/ui/UserInterfaceSystem.h>
#include <spatial/ui/UserInterfaceUtils.h>

using namespace spatial;
namespace fs = ghc::filesystem;

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

	auto config = SetupConfig{"Spatial Engine | Editor", 1280, 720};
	args({"-w", "--width"}) >> config.windowWidth;
	args({"-h", "--height"}) >> config.windowHeight;

	auto app = Application{};
	auto desktopContext = desktop::PlatformContext{};
	auto window = desktopContext.createWindow(config.windowWidth, config.windowHeight, config.windowTitle);
	auto rendering = RenderingSystem{fl::backend::Backend::OPENGL, window};
	auto input = desktop::InputSystem{window};
	auto ui = UserInterfaceSystem{rendering.getEngine(), window};
	ui.setMaterial(toShared(render::createMaterial(rendering.getEngine(), {ASSETS_UI_BLIT_FILAMAT, ASSETS_UI_BLIT_FILAMAT_SIZE})));
	ui.setFontTexture(toShared(imguiCreateTextureAtlas(rendering.getEngine(), {ASSETS_ROBOTO_MEDIUM_TTF, ASSETS_ROBOTO_MEDIUM_TTF_SIZE})));

	auto editor = editor::SceneEditorSystem{rendering.getEngine(), window, input.getState()};

	// Connect all Systems to the Application Main Loop
	app >> desktopContext >> input >> rendering >> ui >> editor;

	// Connect Desktop Events to All Systems
	desktopContext >> app >> input >> rendering >> ui >> editor;

	// Connect Gui Render to Editor
	ui >> editor;

	// Connect Rendering to Editor and UI
	rendering >> editor >> ui;

	return app.run();
}
