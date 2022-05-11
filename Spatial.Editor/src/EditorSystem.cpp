#include "EditorSystem.h"
#include "CustomComponents.h"
#include "EditorCamera.h"
#include "Materials.h"
#include "Serialization.h"
#include "Tags.h"

#include <boost/algorithm/string/predicate.hpp>

#include <spatial/render/Resources.h>

#include <spatial/ui/components/FilesExplorer.h>
#include <spatial/ui/components/Window.h>

#include <spatial/ui/components/styles/WindowPaddingStyle.h>

#include <spatial/core/Clock.h>
#include <spatial/core/Logger.h>
#include <spatial/ecs/Builder.h>
#include <spatial/ecs/Relation.h>
#include <spatial/ecs/SceneView.h>
#include <spatial/ecs/Texture.h>
#include <spatial/render/MaterialController.h>
#include <spatial/render/TextureUtils.h>
#include <spatial/resources/PhysicalFileSystem.h>
#include <spatial/ui/components/Menu.h>
#include <spatial/ui/components/MenuBar.h>
#include <spatial/ui/components/SceneView.h>
#include <spatial/ui/components/Search.h>

namespace fl = filament;
namespace fs = std::filesystem;

using namespace spatial::math;
using namespace entt::literals;

namespace spatial::editor
{

static auto gLogger = createDefaultLogger();

EditorSystem::EditorSystem(FileSystem& fileSystem)
	: mFileSystem{fileSystem},
	  mPlatformContext{},

	  mRegistry{},
	  mEditorRegistry{},

	  mEditorCameraController{},
	  mScriptController{mFileSystem, mPlatformContext.createIsolate()},

	  mJobQueue{},

	  mScenePath{},
	  mCurrentPath{PROJECT_DIR}

{
}

void EditorSystem::onStart()
{
	mJobQueue.connect<OpenProjectEvent>(*this);
	mJobQueue.connect<ClearSceneEvent>(*this);
	mJobQueue.connect<LoadSceneEvent>(*this);
	mJobQueue.connect<SaveSceneEvent>(*this);

	createDefaultEditorEntities();

	mIconTexture = ecs::Resource::create<ecs::tags::IsImageTexture>(mEditorRegistry, "editor/textures/icons.png");
}

void EditorSystem::onStartFrame(float)
{
	mJobQueue.update();
}

void EditorSystem::onUpdateFrame(float delta)
{
	mScriptController.onUpdateFrame(mRegistry, delta);

	if (mIsCameraViewWindowHovered && mIsCameraControlEnabled)
		mEditorCameraController.onUpdateFrame(mRegistry, delta);

	render::MaterialController::updateMaterial<GridMaterial>(mRegistry);
	render::MaterialController::updateMaterial<ColorMaterial>(mRegistry);
	render::MaterialController::updateMaterial<StandardOpaqueMaterial>(mRegistry);
	render::MaterialController::updateMaterial<SkyBoxMaterial>(mRegistry);
}

void EditorSystem::onDrawGui()
{
	static ecs::Entity selectedEntity{ecs::NullEntity};
	static ecs::Entity selectedView{ecs::NullEntity};
	static bool showDebugEntities{false};
	static bool showDebugComponents{false};
	static std::string rootPath = {};

	const auto* icons = render::getTexture(mEditorRegistry, mIconTexture);
	const auto cameraEntity = mRegistry.getFirstEntity<const ecs::Transform, EditorCamera>();
	const auto* cameraTransform = mRegistry.tryGetComponent<const ecs::Transform>(cameraEntity);
	const auto createEntityPosition =
		cameraTransform
			? (cameraTransform->position + (cameraTransform->getForwardVector() * 10.0f) - (math::axisY * 0.1f))
			: math::float3{};

	ui::MenuBar::show([&]() {
		ui::EditorMainMenu::fileMenu(icons, mMenuAction);
		ui::EditorMainMenu::createMenu(mRegistry, selectedEntity, createEntityPosition);
		ui::EditorMainMenu::viewOptionsMenu(showDebugEntities, showDebugComponents);
	});

	if (mMenuAction != ui::EditorMainMenu::Action::None)
	{
		if (mMenuAction == ui::EditorMainMenu::Action::NewScene)
			ui::PopupModal::open("New Scene");

		if (mMenuAction == ui::EditorMainMenu::Action::OpenScene)
			ui::PopupModal::open("Open Scene");

		if (mMenuAction == ui::EditorMainMenu::Action::SaveScene)
			ui::PopupModal::open("Save Scene");

		if (mMenuAction == ui::EditorMainMenu::Action::OpenProject)
			ui::PopupModal::open("Open Project");

		mMenuAction = ui::EditorMainMenu::Action::None;
	}

	ui::PopupModal::show("New Scene", [&]() {
		if (ui::EditorModals::newScene())
			mJobQueue.enqueue<ClearSceneEvent>();
	});

	ui::PopupModal::show("Open Scene", [&]() {
		if (ui::EditorModals::openScene(mScenePath))
			mJobQueue.enqueue<OpenProjectEvent>(mScenePath);
	});

	ui::PopupModal::show("Save Scene", [&]() {
		if (ui::EditorModals::saveScene(mScenePath))
			mJobQueue.enqueue<SaveSceneEvent>(mScenePath);
	});

	ui::PopupModal::show("Open Project", [&]() {
		if (ui::EditorModals::openProject(rootPath))
			mJobQueue.enqueue<OpenProjectEvent>(rootPath);
	});

	{
		auto style = ui::WindowPaddingStyle{};
		auto window = ui::Window{"Scene View", ui::WindowFlags::NoScrollbar};
		mIsCameraViewWindowHovered = ImGui::IsWindowHovered();

		if (!mRegistry.isValid(selectedView))
			selectedView = mRegistry.getFirstEntity<ecs::SceneView, tags::IsEditorView>();

		const auto imageSize = window.getSize() - math::float2{0, 24};
		ui::SceneView::image(mRegistry, selectedView, imageSize);

		if (ui::EditorDragAndDrop::loadScene(mScenePath, selectedEntity))
			mJobQueue.enqueue<LoadSceneEvent>(mScenePath);

		ui::EditorDragAndDrop::loadMeshInstance(mRegistry, selectedEntity, createEntityPosition);
		ui::SceneView::selector(mRegistry, selectedView);
	}

	ui::Window::show("Scene Tree", [&]() {
		static std::string search;
		ui::Search::text(search, icons);
		ui::SceneTree::displayTree(mRegistry, selectedEntity, showDebugEntities, search);

		ui::Popup::show("Scene Graph Popup", [&]() {
			ui::SceneOptionsMenu::createEntitiesMenu(mRegistry, selectedEntity, createEntityPosition);
			ui::SceneOptionsMenu::addChildMenu(mRegistry, selectedEntity, createEntityPosition);
			ui::SceneOptionsMenu::removeMenu(mRegistry, selectedEntity);
		});
	});

	ui::Window::show("Resources Manager", [&]() {
		static std::string search;
		static ui::ResourceManager::ResourceType type = ui::ResourceManager::ResourceType::All;

		ui::ResourceManager::header(search, type, icons);
		ui::ResourceManager::list(mRegistry, selectedEntity, search, type, showDebugEntities);

		ui::Popup::show("AssetManagerPopup", [&]() { ui::SceneOptionsMenu::removeMenu(mRegistry, selectedEntity); });
	});

	ui::Window::show("Materials Manager", [&]() {
		static std::string search;

		ui::Search::text(search, icons);
		ui::MaterialsManager::list(mRegistry, selectedEntity, search, showDebugEntities);

		ui::Popup::show("Asset Manager Popup", [&]() {
			ui::MaterialsManager::createMenu(mRegistry, selectedEntity);
			ui::SceneOptionsMenu::removeMenu(mRegistry, selectedEntity);
		});
	});

	ui::Window::show("Components", [&]() {
		ui::EntityProperties::popup(mRegistry, selectedEntity);
		ui::EntityProperties::displayComponents(mRegistry, selectedEntity, icons, showDebugComponents);
	});

	ui::Window::show("Assets Explorer", [&]() {
		ui::FilesExplorer::displayFiles(mFileSystem, mCurrentPath, icons);
		if (mFileSystem.list(PROJECT_DIR).size() == 0) {
			ImGui::Text("No files inside this project.");
			if (ImGui::Button("Open Project"))
				mMenuAction = ui::EditorMainMenu::Action::OpenProject;
		}
	});
}

void EditorSystem::onPublishRegistry(ecs::RegistryCollection& publisher)
{
	publisher.append(mRegistry);
	publisher.append(mEditorRegistry);
}

void EditorSystem::onUpdateInput(const desktop::InputState& input)
{
	mIsCameraControlEnabled = input.pressed(Key::MouseRight);
	mEditorCameraController.onUpdateInput(input);

	if (input.combined(Key::LControl, Key::N))
		mMenuAction = ui::EditorMainMenu::Action::NewScene;

	if (input.combined(Key::LControl, Key::S))
		mMenuAction = ui::EditorMainMenu::Action::SaveScene;

	if (input.combined(Key::LControl, Key::LShift, Key::O))
		mMenuAction = ui::EditorMainMenu::Action::OpenProject;

	if (input.combined(Key::LControl, Key::O))
		mMenuAction = ui::EditorMainMenu::Action::OpenScene;
}

void EditorSystem::setScenePath(const std::string& path)
{
	mScenePath = path;
}

void EditorSystem::clearScene()
{
	mRegistry = ecs::Registry{};
	createDefaultEditorEntities();
}

void EditorSystem::loadScene()
{
	auto path = getScenePath();

	auto stream = mFileSystem.openReadStream(path);
	if (stream->fail())
	{
		gLogger.error("Could not open scene file: {}", path);
		return;
	}

	try
	{

		mRegistry = parseRegistry(*stream);
	}
	catch (const std::exception& e)
	{
		gLogger.warn("Could not load scene: {}", e.what());
	}
}

void EditorSystem::saveScene()
{
	auto path = getScenePath();

	auto stream = mFileSystem.openWriteStream(path);
	if (stream->fail())
	{
		gLogger.error("Could not open scene file: {}", path);
		return;
	}

	try
	{
		writeRegistry(mRegistry, *stream);
	}
	catch (const std::exception& e)
	{
		gLogger.warn("Could not save scene: {}", e.what());
	}
}

std::string EditorSystem::getScenePath() const
{
	auto path = mScenePath;
	auto projectPath = std::string{"project"} + FileSystem::SEPARATOR;
	if (!boost::starts_with(mScenePath, projectPath))
		path = projectPath + path;

	if (!boost::ends_with(mScenePath, ".spatial.json"))
		path = path + ".spatial.json";

	return path;
}

void EditorSystem::setRootPath(const std::filesystem::path& path)
{
	if (!std::filesystem::exists(path) && !std::filesystem::is_directory(path))
		return;

	mCurrentPath = PROJECT_DIR;
	auto result = mFileSystem.resolve(PROJECT_DIR);
	dynamic_cast<PhysicalFileSystem*>(&result.fileSystem)->setRootPath(path);
}

void EditorSystem::onEvent(const ClearSceneEvent&)
{
	clearScene();
}

void EditorSystem::onEvent(const LoadSceneEvent& event)
{
	setScenePath(event.path);
	loadScene();
}

void EditorSystem::onEvent(const SaveSceneEvent& event)
{
	setScenePath(event.path);
	saveScene();
}

void EditorSystem::onEvent(const OpenProjectEvent& event)
{
	clearScene();
	setRootPath(event.path);
	setScenePath("scenes/default.spatial.json");
}

void EditorSystem::createDefaultEditorEntities()
{
	ecs::Builder::create(mRegistry)
		.asResource()
		.withPath("engine/skybox")
		.with<ecs::tags::IsMesh>()
		.with(ecs::RuntimeMesh{{{-1.0f, -1.0f, 1.0f}, {3.0f, -1.0f, 1.0f}, {-1.0f, 3.0f, 1.0f}}, {0, 1, 2}});

	ecs::Builder::create(mRegistry)
		.asResource()
		.withPath("engine/dummy_cubemap")
		.with<ecs::tags::IsCubeMapTexture>()
		.with<ecs::DummyCubeMapTexture>();

	ecs::Builder::create(mRegistry)
		.asResource()
		.withPath("engine/white")
		.with<ecs::tags::IsImageTexture>()
		.with(ecs::RuntimeTexture{{0xFFFFFFFF}, 1});

	ecs::Builder::create(mRegistry)
		.asResource()
		.withPath("engine/black")
		.with<ecs::tags::IsImageTexture>()
		.with(ecs::RuntimeTexture{{0xFF000000}, 1});

	ecs::Builder::create(mRegistry)
		.withName("SkyBox Material")
		.asMaterialInstance<SkyBoxMaterial>()
		.withMaterial("editor/materials/skybox.filamat")
		.withProps({
			false,
			{math::float3{.0f}, 1.0f},
			ecs::Resource::findOrCreate<ecs::tags::IsCubeMapTexture>(mRegistry, "editor/textures/skybox/texture.ktx"),
		});

	ecs::Builder::create(mRegistry)
		.withName("Grid Material")
		.with<tags::IsEditorEntity>()
		.asMaterialInstance<GridMaterial>()
		.withMaterial("editor/materials/grid.filamat")
		.withProps({});

	ecs::Builder::create(mRegistry)
		.withName("Grid Plane")
		.with<tags::IsEditorEntity>()
		.asTransform()
		.withScale({100.0f, 1.0f, 100.0f})
		.withPosition({.0f, -0.01f, .0f})
		.asMeshInstance()
		.withMesh("editor/meshes/plane.filamesh")
		.withDefaultMaterialInstance(mRegistry.getFirstEntity<GridMaterial>());

	ecs::Builder::create(mRegistry)
		.withName("SkyBox")
		.asMeshInstance()
		.withMesh("engine/skybox")
		.withDefaultMaterialInstance(mRegistry.getFirstEntity<SkyBoxMaterial>())
		.withShadowOptions(false, false)
		.withCulling(false)
		.withPriority(0x7);

	ecs::Builder::create(mRegistry)
		.withName("Editor View")
		.with<tags::IsEditorEntity>()
		.with<tags::IsEditorView>()
		.asSceneView()
		.withIndirectLight(ecs::Builder::create(mRegistry)
							   .withName("Indirect Light")
							   .with<tags::IsEditorEntity>()
							   .asIndirectLight()
							   .withReflectionsTexture("editor/textures/skybox/ibl.ktx")
							   .withIrradianceValues("editor/textures/skybox/sh.txt"))
		.withCamera(ecs::Builder::create(mRegistry)
						.withName("Editor Camera")
						.with(EditorCamera{.5f, 10.0f})
						.with<tags::IsEditorEntity>()
						.asTransform()
						.withPosition({3.0f, 3.0f, 20.0f})
						.asPerspectiveCamera()
						.withFieldOfView(60.0)
						.withAspectRatio(19.0 / 6.0));
}

} // namespace spatial::editor
