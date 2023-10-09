#include "EditorSystem.h"
#include "FileSystem.h"
#include "Serialization.h"
#include "Settings.h"

#include <spatial/core/Application.h>
#include <spatial/core/ApplicationUtils.h>
#include <spatial/desktop/Window.h>
#include <spatial/desktop/WindowEventUtils.h>
#include <spatial/graphics/RegistryRenderingSystem.h>
#include <spatial/graphics/RegistryRenderingSystemUtils.h>
#include <spatial/graphics/RenderingSystem.h>
#include <spatial/graphics/RenderingSystemUtils.h>
#include <spatial/input/InputSystem.h>
#include <spatial/input/InputSystemUtils.h>
#include <spatial/ui/UserInterfaceSystem.h>
#include <spatial/ui/UserInterfaceUtils.h>

using namespace spatial;

int main(int argc, char* argv[])
{
	const auto config = editor::Settings::load(argc, argv);
	auto editorFs = editor::createEditorFileSystem();

	auto app = Application{};
	auto input = desktop::InputSystem{};
	auto desktopContext = desktop::PlatformContext{};

	auto window = desktopContext.createWindow(config.windowDimensions.value_or(desktopContext.getMonitorSize()),
											  config.windowTitle);
#if defined(SPATIAL_PLATFORM_WINDOWS)
	window.setIcon(*editorFs, "textures/icons/AppIcon.png");
#endif

	auto rendering = graphics::RenderingSystem{};
	rendering.attach(window);

	auto ui = ui::UserInterfaceSystem{rendering.getEngine()};
	ui.setMaterial(*editorFs, "materials/ui_blit.filamat");
	ui.addFont(*editorFs, "fonts/roboto_medium.ttf");
	ui.getRenderer().createFontTextureAtlas();

	auto editor = editor::EditorSystem{};
	editor.getFileSystem().mount("editor", editor::createEditorFileSystem());

	auto projectFs = editor::createProjectFileSystem(config);
	if (projectFs) editor.getFileSystem().mount("project", *projectFs);

	auto scene = graphics::RegistryRenderingSystem{editor.getFileSystem()};

	// Connect All Systems to the Application Main Loop
	app >> desktopContext >> window >> input >> rendering >> ui >> scene >> editor;

	// Connect Window Events to All Systems
	window >> app >> input >> rendering >> ui >> editor;

	// Connect Gui Render to Editor
	ui >> editor;

	// Connect Rendering to Scene Rendering Bridge and UI
	rendering >> scene >> ui;

	// Connect Window to Rendering System
	window >> rendering;

	// Connect Scene Rendering Bridge to Editor
	scene >> editor;

	// Connect Input to Editor
	input >> editor;

	return app.run();
}
