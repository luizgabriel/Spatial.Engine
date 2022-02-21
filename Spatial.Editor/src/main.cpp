#include "SceneEditorSystem.h"
#include "Serialization.h"
#include "Settings.h"

#include <assets/generated.h>
#include <spatial/core/Application.h>
#include <spatial/core/ApplicationUtils.h>
#include <spatial/desktop/InputSystem.h>
#include <spatial/desktop/InputSystemUtils.h>
#include <spatial/desktop/PlatformEventUtils.h>
#include <spatial/desktop/Window.h>
#include <spatial/render/RenderingSystem.h>
#include <spatial/render/RenderingSystemUtils.h>
#include <spatial/script/PlatformContext.h>
#include <spatial/ui/system/UserInterfaceSystem.h>
#include <spatial/ui/system/UserInterfaceUtils.h>
#include <spatial/resources/MemoryFileSystem.h>
#include <spatial/render/RegistryRenderingSystem.h>
#include <spatial/render/RegistryRenderingSystemUtils.h>

using namespace spatial;

RootFileSystem createDefaultFileSystem()
{
	auto fileSystem = RootFileSystem{};
	auto editorFs = fileSystem.mount<MemoryFileSystem>("editor");
	editorFs->define("meshes/cube.filamesh", {ASSETS_CUBE_FILAMESH, ASSETS_CUBE_FILAMESH_SIZE});
	editorFs->define("meshes/sphere.filamesh", {ASSETS_SPHERE_FILAMESH, ASSETS_SPHERE_FILAMESH_SIZE});
	editorFs->define("meshes/plane.filamesh", {ASSETS_PLANE_FILAMESH, ASSETS_PLANE_FILAMESH_SIZE});
	editorFs->define("meshes/cylinder.filamesh", {ASSETS_CYLINDER_FILAMESH, ASSETS_CYLINDER_FILAMESH_SIZE});
	editorFs->define("materials/ui_blit.filamat", {ASSETS_UI_BLIT_FILAMAT, ASSETS_UI_BLIT_FILAMAT_SIZE});
	editorFs->define("fonts/roboto_medium.ttf", {ASSETS_ROBOTO_MEDIUM_TTF, ASSETS_ROBOTO_MEDIUM_TTF_SIZE});
	editorFs->define("textures/skybox/ibl.ktx", {ASSETS_DEFAULT_SKYBOX_IBL_KTX, ASSETS_DEFAULT_SKYBOX_IBL_KTX_SIZE});
	editorFs->define("textures/skybox/texture.ktx", {ASSETS_DEFAULT_SKYBOX_SKYBOX_KTX, ASSETS_DEFAULT_SKYBOX_SKYBOX_KTX_SIZE});
	editorFs->define("textures/skybox/sh.txt", {ASSETS_SH_TXT, ASSETS_SH_TXT_SIZE});

	return fileSystem;
}

int main(int argc, char* argv[])
{
	const auto config = editor::Settings::load(argc, argv);

	auto fileSystem = createDefaultFileSystem();

	auto app = Application{};
	auto desktopContext = desktop::PlatformContext{};

	auto window = desktopContext.createWindow(config.windowWidth, config.windowHeight, config.windowTitle);
	auto rendering = render::RenderingSystem{window};
	auto input = desktop::InputSystem{window};

	auto scene = render::RegistryRenderingSystem{rendering.getEngine(), fileSystem};

	auto ui = UserInterfaceSystem{rendering.getEngine(), fileSystem, window};
	ui.setMaterial("editor/materials/ui_blit.filamat");
	ui.addFont("editor/fonts/roboto_medium.ttf");
	ui.getRenderer().createFontTextureAtlas();

	auto scriptingContext = script::PlatformContext{};

	auto editor = editor::SceneEditorSystem{rendering.getEngine(), window, fileSystem, scriptingContext};
	editor.setRootPath(config.projectFolder);

	// Connect all Systems to the Application Main Loop
	app >> desktopContext >> input >> rendering >> ui >> editor >> scene;

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
