#include "EditorSettings.h"
#include "SceneEditorSystem.h"

#include <assets/generated.h>
#include <spatial/core/Application.h>
#include <spatial/core/ApplicationUtils.h>
#include <spatial/desktop/InputSystem.h>
#include <spatial/desktop/InputSystemUtils.h>
#include <spatial/desktop/PlatformEventUtils.h>
#include <spatial/desktop/Window.h>
#include <spatial/render/RenderingSystem.h>
#include <spatial/render/RenderingSystemUtils.h>
#include <spatial/render/ResourceLoaders.h>

#include <spatial/ui/system/ImGuiHelpers.h>
#include <spatial/ui/system/UserInterfaceSystem.h>
#include <spatial/ui/system/UserInterfaceUtils.h>

using namespace spatial;

int main(int argc, char* argv[])
{
	auto config = editor::loadEditorSettings(argc, argv);

	auto app = Application{};
	auto desktopContext = desktop::PlatformContext{};

	auto window = desktopContext.createWindow(config.windowWidth, config.windowHeight, config.windowTitle);
	window.setIconPixels(ASSETS_ICON_PNG, ASSETS_ICON_PNG_SIZE);

	auto rendering = RenderingSystem{fl::backend::Backend::OPENGL, window};
	auto input = desktop::InputSystem{window};
	auto ui = UserInterfaceSystem{rendering.getEngine(), window};
	ui.setMaterial(
		toShared(render::createMaterial(rendering.getEngine(), ASSETS_UI_BLIT_FILAMAT, ASSETS_UI_BLIT_FILAMAT_SIZE)));
	ui.setFontTexture(toShared(
		ui::imguiCreateTextureAtlas(rendering.getEngine(), ASSETS_ROBOTO_MEDIUM_TTF, ASSETS_ROBOTO_MEDIUM_TTF_SIZE)));

	auto editor = editor::SceneEditorSystem{rendering.getEngine(), window};

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
