#include "EditorSystem.h"
#include "CustomComponents.h"
#include "EditorCamera.h"
#include "Materials.h"
#include "Serialization.h"
#include "Tags.h"

#include <boost/algorithm/string/predicate.hpp>

#include <spatial/render/Resources.h>

#include <spatial/ui/components/AssetsExplorer.h>
#include <spatial/ui/components/DockSpace.h>
#include <spatial/ui/components/Window.h>

#include <spatial/ui/components/styles/WindowPaddingStyle.h>

#include <spatial/core/Logger.h>
#include <spatial/ecs/Builder.h>
#include <spatial/ecs/Relation.h>
#include <spatial/ecs/SceneView.h>
#include <spatial/ecs/Texture.h>
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

EditorSystem::EditorSystem(filament::Engine& engine, desktop::Window& window, FileSystem& fileSystem)
	: mEngine{engine},
	  mWindow{window},

	  mFileSystem{fileSystem},
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

	ecs::Builder::create(mEditorRegistry).asResource().withPath("editor/textures/icons.png");

	createDefaultEditorEntities();
}

void EditorSystem::onStartFrame(float)
{
	mJobQueue.update();
}

void EditorSystem::onUpdateFrame(float delta)
{
	mScriptController.onUpdateFrame(mRegistry, delta);
	mEditorCameraController.onUpdateFrame(mRegistry, delta);

	render::MaterialController::updateMaterial<GridMaterial>(mRegistry);
	render::MaterialController::updateMaterial<ColorMaterial>(mRegistry);
	render::MaterialController::updateMaterial<StandardOpaqueMaterial>(mRegistry);
	render::MaterialController::updateMaterial<SkyBoxMaterial>(mRegistry);
}

void EditorSystem::onDrawGui()
{
	const auto* iconTexture = render::getTexture(mEditorRegistry, "editor/textures/icons.png");

	auto dockSpace = ui::DockSpace{"Spatial"};

	static ecs::Entity selectedEntity{ecs::NullEntity};
	static bool showDebugEntities{false};
	static bool showDebugComponents{false};
	static ui::EditorMainMenu::FileMenuAction fileMenuAction{ui::EditorMainMenu::FileMenuAction::Unknown};
	static std::filesystem::path rootPath = std::filesystem::current_path();

	const auto cameraEntity = mRegistry.getFirstEntity<const ecs::Transform, EditorCamera>();
	const auto* cameraTransform = mRegistry.tryGetComponent<const ecs::Transform>(cameraEntity);
	const auto createEntityPosition =
		cameraTransform
			? (cameraTransform->position + (cameraTransform->getForwardVector() * 10.0f) - (math::axisY * 0.1f))
			: math::float3{};

	ui::MenuBar::show([&]() {
		fileMenuAction = ui::EditorMainMenu::fileMenu(iconTexture);
		ui::EditorMainMenu::createMenu(mRegistry, selectedEntity, createEntityPosition);
		ui::EditorMainMenu::viewOptionsMenu(showDebugEntities, showDebugComponents);
	});

	if (fileMenuAction != ui::EditorMainMenu::FileMenuAction::Unknown)
	{
		switch (fileMenuAction)
		{
		case ui::EditorMainMenu::FileMenuAction::OpenProject:
			ui::OpenProjectModal::open();
			break;
		case ui::EditorMainMenu::FileMenuAction::SaveScene:
			ui::SaveSceneModal::open();
			break;
		case ui::EditorMainMenu::FileMenuAction::OpenScene:
			ui::OpenSceneModal::open();
			break;
		case ui::EditorMainMenu::FileMenuAction::NewScene:
			ui::NewSceneModal::open();
			break;
		case ui::EditorMainMenu::FileMenuAction::Unknown:
			break;
		}

		fileMenuAction = ui::EditorMainMenu::FileMenuAction::Unknown;
	}

	if (ui::OpenProjectModal::show(rootPath))
		mJobQueue.enqueue<OpenProjectEvent>(rootPath);

	if (ui::NewSceneModal::show())
		mJobQueue.enqueue<ClearSceneEvent>();

	if (ui::OpenSceneModal::show(mScenePath))
		mJobQueue.enqueue<LoadSceneEvent>(mScenePath);

	if (ui::SaveSceneModal::show(mScenePath))
		mJobQueue.enqueue<SaveSceneEvent>(mScenePath);

	{
		auto sceneView = mRegistry.getFirstEntity<ecs::SceneView, render::TextureView, tags::IsEditorView>();
		auto style = ui::WindowPaddingStyle{};
		auto window = ui::Window{"Scene View", ui::WindowFlags::NoScrollbar};

		const auto imageSize = window.getSize() - math::float2{0, 24};
		ui::SceneView::image(mRegistry, sceneView, imageSize);

		{
			auto style2 = ui::WindowPaddingStyle{4};
			ui::Popup::show("Scene View Popup", [&]() {
				auto menu = ui::Menu{"Create"};
				if (!menu.isOpen())
					return;
				ui::SceneOptionsMenu::createEntitiesMenu(mRegistry, selectedEntity, createEntityPosition);
			});
		}

		// TODO: Move cursor exactly to the center of the scene window (Not the center of the screen)
		if (ImGui::IsWindowFocused() && ImGui::IsItemClicked() && mEditorCameraController.toggleControl())
			mWindow.warpMouse(mWindow.getSize() * .5f);

		if (!ImGui::IsWindowFocused() || (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Right)))
			mEditorCameraController.disable();

		if (ui::EditorDragAndDrop::loadScene(mScenePath, selectedEntity))
			mJobQueue.enqueue<LoadSceneEvent>(mScenePath);

		ui::EditorDragAndDrop::loadMeshInstance(mRegistry, selectedEntity, createEntityPosition);
	}

	ui::Window::show("Scene Tree", [&]() {
		static std::string search;
		ui::Search::searchText(search);
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

		ui::ResourceManager::header(search, type);
		ui::ResourceManager::list(mRegistry, selectedEntity, search, type, showDebugEntities);

		ui::EditorDragAndDrop::loadResource(mRegistry, selectedEntity);

		ui::Popup::show("Asset Manager Popup", [&]() { ui::SceneOptionsMenu::removeMenu(mRegistry, selectedEntity); });
	});

	ui::Window::show("Materials Manager", [&]() {
		static std::string search;

		ui::MaterialsManager::header(search);
		ui::MaterialsManager::list(mRegistry, selectedEntity, search, showDebugEntities);

		ui::Popup::show("Asset Manager Popup", [&]() {
			ui::MaterialsManager::createMenu(mRegistry, selectedEntity);
			ui::SceneOptionsMenu::removeMenu(mRegistry, selectedEntity);
		});
	});

	ui::Window::show("Components", [&]() {
		ui::EntityProperties::popup(mRegistry, selectedEntity);
		ui::EntityProperties::displayComponents(mRegistry, selectedEntity, iconTexture, showDebugComponents);
	});

	ui::Window::show("Assets Explorer",
					 [&]() { ui::AssetsExplorer::displayFiles(mFileSystem, mCurrentPath, iconTexture); });
}

void EditorSystem::onPublishRegistry(ecs::RegistryCollection& publisher)
{
	publisher.append(mRegistry);
	publisher.append(mEditorRegistry);
}

void EditorSystem::onUpdateInput(const desktop::InputState& input)
{
	mEditorCameraController.onUpdateInput(input);
	if (input.released(Key::Escape))
		mEditorCameraController.disable();

	if (input.combined(Key::LControl, Key::S))
		mJobQueue.enqueue<SaveSceneEvent>();
}

void EditorSystem::setScenePath(const std::filesystem::path& path)
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
	const auto path = getScenePath();

	auto stream = mFileSystem.openReadStream(path.string());
	if (stream->fail())
	{
		gLogger.error("Could not open scene file: {}", path.string());
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

	createDefaultEditorEntities();
}

void EditorSystem::saveScene()
{
	const auto path = getScenePath();

	auto stream = mFileSystem.openWriteStream(path.string());
	if (stream->fail())
	{
		gLogger.error("Could not open scene file: {}", path.string());
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

std::filesystem::path EditorSystem::getScenePath() const
{
	auto path = mScenePath;
	if (!boost::starts_with(path.parent_path().c_str(), "project/"))
		path = "project" / path;

	if (!boost::ends_with(mScenePath.filename().c_str(), ".spatial.json"))
		path = std::filesystem::path{path.string() + ".spatial.json"};

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
	auto skyboxMesh = ecs::Resource::findOrCreate(mRegistry, "engine/skybox");
	if (!mRegistry.hasAllComponents<ecs::tags::IsResourceLoaded>(skyboxMesh))
	{
		auto ib = toShared(render::createFullScreenIndexBuffer(mEngine));
		mRegistry.addComponent<ecs::tags::IsResourceLoaded>(skyboxMesh);
		mRegistry.addOrReplaceComponent<ecs::tags::IsMesh>(skyboxMesh);
		mRegistry.addOrReplaceComponent(skyboxMesh,
										toShared(render::createFullScreenVertexBuffer(mEngine)));
		mRegistry.addOrReplaceComponent(skyboxMesh, render::MeshGeometries{{0, ib->getIndexCount()}});
		mRegistry.addOrReplaceComponent(skyboxMesh, std::move(ib));
	}

	auto dummyCubemap = ecs::Resource::findOrCreate(mRegistry, "engine/dummy_cubemap");
	if (!mRegistry.hasAllComponents<ecs::tags::IsResourceLoaded>(dummyCubemap)) {
		mRegistry.addComponent<ecs::tags::IsResourceLoaded>(dummyCubemap);
		mRegistry.addOrReplaceComponent<ecs::tags::IsCubeMapTexture>(dummyCubemap);
		mRegistry.addOrReplaceComponent(dummyCubemap, toShared(render::createDummyCubemap(mEngine)));
	}

	auto dummyWhite = ecs::Resource::findOrCreate(mRegistry, "engine/dummy_texture_white");
	if (!mRegistry.hasAllComponents<ecs::tags::IsResourceLoaded>(dummyWhite)) {
		mRegistry.addComponent<ecs::tags::IsResourceLoaded>(dummyWhite);
		mRegistry.addOrReplaceComponent<ecs::tags::IsImageTexture>(dummyWhite);
		mRegistry.addOrReplaceComponent(dummyWhite, toShared(render::createDummyTexture<0xFFFFFFFF>(mEngine)));
	}

	auto dummyBlack = ecs::Resource::findOrCreate(mRegistry, "engine/dummy_texture_black");
	if (!mRegistry.hasAllComponents<ecs::tags::IsResourceLoaded>(dummyBlack)) {
		mRegistry.addComponent<ecs::tags::IsResourceLoaded>(dummyBlack);
		mRegistry.addOrReplaceComponent<ecs::tags::IsImageTexture>(dummyBlack);
		mRegistry.addOrReplaceComponent(dummyBlack, toShared(render::createDummyTexture<0x00000000>(mEngine)));
	}

	if (!mRegistry.existsAny<SkyBoxMaterial>())
		ecs::Builder::create(mRegistry)
			.withName("SkyBox Material")
			.asMaterialInstance<SkyBoxMaterial>()
			.withMaterial("editor/materials/skybox.filamat")
			.withProps({
				false,
				{math::float3{.0f}, 1.0f},
				ecs::Resource::findOrCreate(mRegistry, "editor/textures/skybox/texture.ktx"),
			});

	if (!mRegistry.existsAny<GridMaterial>())
		ecs::Builder::create(mRegistry)
			.withName("Grid Material")
			.with<tags::IsEditorEntity>()
			.asMaterialInstance<GridMaterial>()
			.withMaterial("editor/materials/grid.filamat")
			.withProps({});

	if (!mRegistry.existsAny<tags::IsGridPlane>())
		ecs::Builder::create(mRegistry)
			.withName("Grid Plane")
			.with<tags::IsEditorEntity>()
			.with<tags::IsGridPlane>()
			.asTransform()
			.withScale({100.0f, 1.0f, 100.0f})
			.withPosition({.0f, -0.01f, .0f})
			.asMeshInstance()
			.withMesh("editor/meshes/plane.filamesh")
			.withDefaultMaterial(mRegistry.getFirstEntity<GridMaterial>());

	if (!mRegistry.existsAny<ecs::MeshInstance, tags::IsSkyBox>())
		ecs::Builder::create(mRegistry)
			.withName("SkyBox")
			.with<tags::IsSkyBox>()
			.asMeshInstance()
			.withMesh("engine/skybox")
			.withDefaultMaterial(mRegistry.getFirstEntity<SkyBoxMaterial>())
			.withShadowOptions(false, false)
			.withCulling(false)
			.withPriority(0x7);

	if (!mRegistry.existsAny<tags::IsEditorView>())
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
