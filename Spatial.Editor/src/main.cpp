#include "SceneEditorSystem.h"
#include "Settings.h"
#include "Serialization.h"

#include <assets/generated.h>
#include <spatial/core/Application.h>
#include <spatial/core/ApplicationUtils.h>
#include <spatial/desktop/InputSystem.h>
#include <spatial/desktop/InputSystemUtils.h>
#include <spatial/desktop/PlatformEventUtils.h>
#include <spatial/desktop/Window.h>
#include <spatial/render/RenderingSystem.h>
#include <spatial/render/RenderingSystemUtils.h>
#include <spatial/ui/system/UserInterfaceSystem.h>
#include <spatial/ui/system/UserInterfaceUtils.h>

using namespace spatial;


int main(int argc, char* argv[])
{
	const auto config = editor::Settings::load(argc, argv);

	auto app = Application{};
	auto desktopContext = desktop::PlatformContext{};

	auto window = desktopContext.createWindow(config.windowWidth, config.windowHeight, config.windowTitle);
	auto rendering = RenderingSystem{window};
	auto input = desktop::InputSystem{window};

	auto ui = UserInterfaceSystem{rendering.getEngine(), window};
	ui.getRenderer().setMaterial(ASSETS_UI_BLIT_FILAMAT, ASSETS_UI_BLIT_FILAMAT_SIZE);
	ui.getRenderer().addFont(ASSETS_ROBOTO_MEDIUM_TTF, ASSETS_ROBOTO_MEDIUM_TTF_SIZE);
	ui.getRenderer().createFontTextureAtlas();

	auto editor = editor::SceneEditorSystem{config,
											rendering.getEngine(),
											window};

	// Connect all Systems to the Application Main Loop
	app >> desktopContext >> input >> rendering >> ui >> editor;

	// Connect Desktop Events to All Systems
	desktopContext >> app >> input >> rendering >> ui >> editor;

	// Connect Gui Render to Editor
	ui >> editor;

	// Connect Rendering to Editor and UI
	rendering >> editor >> ui;

	// Connect Input to Editor
	input >> editor;

	return app.run();
}
