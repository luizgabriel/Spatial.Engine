#include "EditorSystem.h"
#include "Serialization.h"
#include "Settings.h"
#include "FileSystem.h"

#include <spatial/core/Application.h>
#include <spatial/core/ApplicationUtils.h>
#include <spatial/desktop/PlatformEventUtils.h>
#include <spatial/desktop/Window.h>
#include <spatial/input/InputSystem.h>
#include <spatial/input/InputSystemUtils.h>
#include <spatial/render/RegistryRenderingSystem.h>
#include <spatial/render/RegistryRenderingSystemUtils.h>
#include <spatial/render/RenderingSystem.h>
#include <spatial/render/RenderingSystemUtils.h>
#include <spatial/ui/system/UserInterfaceSystem.h>
#include <spatial/ui/system/UserInterfaceUtils.h>

using namespace spatial;

int main(int argc, char* argv[])
{
	const auto config = editor::Settings::load(argc, argv);
	auto fileSystem = editor::createDefaultFileSystem(config);

	auto app = Application{};
	auto desktopContext = desktop::PlatformContext{};

	auto window = desktopContext.createWindow(config.windowWidth, config.windowHeight, config.windowTitle);
	#if defined(SPATIAL_PLATFORM_WINDOWS)
	window.setIcon(*fileSystem, "editor/AppIcon.png");
	#endif

	auto rendering = render::RenderingSystem{window};
	auto input = desktop::InputSystem{};

	auto scene = render::RegistryRenderingSystem{*fileSystem};

	auto ui = UserInterfaceSystem{rendering.getEngine(), window};
	ui.setMaterial(*fileSystem, "editor/materials/ui_blit.filamat");
	ui.addFont(*fileSystem, "editor/fonts/roboto_medium.ttf");
	ui.getRenderer().createFontTextureAtlas();

	auto editor = editor::EditorSystem{*fileSystem};

	// Connect all Systems to the Application Main Loop
	app >> desktopContext >> input >> rendering >> ui >> scene >> editor;

	// Connect Desktop Events to All Systems
	desktopContext >> app >> input >> rendering >> ui >> editor;

	// Connect Gui Render to Editor
	ui >> editor;

	// Connect Rendering to Scene Rendering Bridge and UI
	rendering >> scene >> ui;

	// Connect Scene Rendering Bridge to Editor
	scene >> editor;

	// Connect Input to Editor
	input >> editor;

	return app.run();
}
