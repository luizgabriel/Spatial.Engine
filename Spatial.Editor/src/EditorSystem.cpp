#include "EditorSystem.h"
#include "CustomComponents.h"
#include "EditorCamera.h"
#include "Materials.h"
#include "Serialization.h"
#include "Tags.h"

#include <boost/algorithm/string/predicate.hpp>

#include <assets/generated.h>

#include <spatial/render/Resources.h>
#include <spatial/render/SkyboxResources.h>

#include <spatial/ui/components/AssetsExplorer.h>
#include <spatial/ui/components/DockSpace.h>
#include <spatial/ui/components/Window.h>

#include <spatial/ui/components/styles/WindowPaddingStyle.h>

#include <spatial/core/Logger.h>
#include <spatial/ecs/RegistryUtils.h>
#include <spatial/ecs/Relation.h>
#include <spatial/ecs/SceneView.h>
#include <spatial/resources/MemoryFileSystem.h>
#include <spatial/resources/PhysicalFileSystem.h>
#include <spatial/ui/components/MenuBar.h>
#include <spatial/ui/components/SceneView.h>
#include <spatial/ui/components/Search.h>
#include <spatial/ui/components/Menu.h>

namespace fl = filament;
namespace fs = std::filesystem;

using namespace spatial::math;

namespace spatial::editor
{

static auto gLogger = createDefaultLogger();

void createDefaultEditorEntities(ecs::Registry& registry)
{
	if (!ecs::handleOf<SkyBoxMaterial>(registry))
	{
		auto skyboxMaterial = ecs::handleOf(
			registry, ecs::PrecompiledMaterial::findOrCreate(registry, "editor/materials/skybox.filamat"));
		skyboxMaterial.addOrReplace<tags::IsEditorEntity>();

		ecs::build(registry)
			.withName("SkyBox Material")
			.asMaterialInstance<SkyBoxMaterial>()
			.withMaterial(skyboxMaterial)
			.withProps({
				false,
				{math::float3{.0f}, 1.0f},
				{"editor/textures/skybox/texture.ktx"},
			});
	}

	if (!ecs::handleOf<GridMaterial>(registry))
	{
		auto gridMaterial =
			ecs::handleOf(registry, ecs::PrecompiledMaterial::findOrCreate(registry, "editor/materials/grid.filamat"));
		gridMaterial.addOrReplace<tags::IsEditorEntity>();

		ecs::build(registry)
			.withName("Grid Material")
			.with<tags::IsEditorEntity>()
			.asMaterialInstance<GridMaterial>()
			.withMaterial(gridMaterial)
			.withProps({});
	}

	if (!ecs::handleOf<tags::IsGridPlane>(registry))
	{
		auto planeMesh = ecs::handleOf(registry, ecs::Mesh::findOrCreate(registry, "editor/meshes/plane.filamesh"));
		planeMesh.addOrReplace<tags::IsEditorEntity>();

		ecs::build(registry)
			.withName("Grid Plane")
			.with<tags::IsEditorEntity>()
			.with<tags::IsGridPlane>()
			.asTransform()
			.withScale({100.0f, 1.0f, 100.0f})
			.withPosition({.0f, -0.01f, .0f})
			.asMeshInstance()
			.withMesh(planeMesh)
			.withMaterialAt(0, registry.getFirstEntity<GridMaterial>());
	}

	if (!ecs::handleOf<tags::IsSkyBox>(registry))
	{
		auto skyboxMesh = ecs::handleOf(registry, ecs::Mesh::findOrCreate(registry, "engine/skybox"));
		skyboxMesh.addOrReplace<tags::IsSkyBoxMeshResource>();
		skyboxMesh.addOrReplace<tags::IsEditorEntity>();

		ecs::build(registry)
			.withName("SkyBox")
			.with<tags::IsSkyBox>()
			.asMeshInstance()
			.withMesh(skyboxMesh)
			.withMaterialAt(0, registry.getFirstEntity<SkyBoxMaterial>())
			.withShadowOptions(false, false)
			.withCulling(false)
			.withPriority(0x7);
	}

	if (!ecs::handleOf<tags::IsEditorView>(registry))
		ecs::build(registry)
			.withName("Editor View")
			.with<tags::IsEditorEntity>()
			.with<tags::IsEditorView>()
			.asSceneView()
			.withIndirectLight(ecs::build(registry)
								   .withName("Indirect Light")
								   .with<tags::IsEditorEntity>()
								   .asIndirectLight()
								   .withReflectionsTexturePath("editor/textures/skybox/ibl.ktx")
								   .withIrradianceValuesPath("editor/textures/skybox/sh.txt"))
			.withCamera(ecs::build(registry)
							.withName("Editor Camera")
							.with(EditorCamera{.5f, 10.0f})
							.with<tags::IsEditorEntity>()
							.asTransform()
							.withPosition({3.0f, 3.0f, 20.0f})
							.asPerspectiveCamera()
							.withFieldOfView(60.0)
							.withAspectRatio(19.0 / 6.0));
}

EditorSystem::EditorSystem(filament::Engine& engine, desktop::Window& window, FileSystem& fileSystem)
	: mEngine{engine},
	  mWindow{window},

	  mIconTexture{mEngine, nullptr},

	  mFileSystem{fileSystem},
	  mPlatformContext{},

	  mRegistry{},

	  mEditorCameraController{},
	  mMaterialController{mEngine, mFileSystem},
	  mScriptController{mFileSystem, mPlatformContext.createIsolate()},

	  mJobQueue{},

	  mScenePath{},
	  mCurrentPath{PROJECT_DIR}

{
	{
		auto data = mFileSystem.readBinary("editor/textures/icons.png");
		mIconTexture = render::createTexture(mEngine, data.data(), data.size());
	}

	mJobQueue.connect<OpenProjectEvent>(*this);
	mJobQueue.connect<ClearSceneEvent>(*this);
	mJobQueue.connect<LoadSceneEvent>(*this);
	mJobQueue.connect<SaveSceneEvent>(*this);
}

void EditorSystem::onStart()
{
	mMaterialController.load("engine/dummy_cubemap"_hs, render::createDummyCubemap(mEngine));
	mMaterialController.load("engine/dummy_texture_white"_hs, render::createDummyTexture<0xFFFFFFFF>(mEngine));
	mMaterialController.load("engine/dummy_texture_black"_hs, render::createDummyTexture<0x00000000>(mEngine));

	createDefaultEditorEntities(mRegistry);
}

void EditorSystem::onStartFrame(float)
{
	mJobQueue.update();

	auto skyboxMeshResource = ecs::handleOf<tags::IsSkyBoxMeshResource>(mRegistry);
	if (skyboxMeshResource && !skyboxMeshResource.has<ecs::tags::IsMeshLoaded>())
	{
		auto ib = toShared(render::createFullScreenIndexBuffer(mEngine));
		skyboxMeshResource.add<ecs::tags::IsMeshLoaded>();
		skyboxMeshResource.addOrReplace(toShared(render::createFullScreenVertexBuffer(mEngine)));
		skyboxMeshResource.addOrReplace(render::MeshGeometries{{0, ib->getIndexCount()}});
		skyboxMeshResource.addOrReplace(std::move(ib));
	}

	mMaterialController.onStartFrame();
}

void EditorSystem::onUpdateFrame(float delta)
{
	mScriptController.onUpdateFrame(mRegistry, delta);
	mEditorCameraController.onUpdateFrame(mRegistry, delta);

	mMaterialController.applyMaterial<GridMaterial>(mRegistry);
	mMaterialController.applyMaterial<ColorMaterial>(mRegistry);
	mMaterialController.applyMaterialWithFinder<StandardOpaqueMaterial>(mRegistry);
	mMaterialController.applyMaterialWithFinder<SkyBoxMaterial>(mRegistry);
}

void EditorSystem::onDrawGui()
{
	auto dockSpace = ui::DockSpace{"Spatial"};

	static ecs::Entity selectedEntity{ecs::NullEntity};
	static bool showDebugEntities{false};
	static bool showDebugComponents{false};
	static std::filesystem::path rootPath = std::filesystem::current_path();

	const auto cameraEntity = mRegistry.getFirstEntity<const ecs::Transform, EditorCamera>();
	const auto* cameraTransform = mRegistry.tryGetComponent<const ecs::Transform>(cameraEntity);
	const auto createEntityPosition =
		cameraTransform
			? (cameraTransform->position + (cameraTransform->getForwardVector() * 10.0f) - (math::axisY * 0.1f))
			: math::float3{};


	ui::MenuBar::show([&]() {
		ui::EditorMainMenu::fileMenu(*mIconTexture);
		ui::EditorMainMenu::viewOptionsMenu(showDebugEntities, showDebugComponents);
		ui::EditorMainMenu::createMenu(mRegistry, selectedEntity, createEntityPosition);

		if (ui::OpenProjectModal::show(rootPath))
			mJobQueue.enqueue<OpenProjectEvent>(rootPath);

		if (ui::NewSceneModal::show())
			mJobQueue.enqueue<ClearSceneEvent>();

		if (ui::OpenSceneModal::show(mScenePath))
			mJobQueue.enqueue<LoadSceneEvent>(mScenePath);

		if (ui::SaveSceneModal::show(mScenePath))
			mJobQueue.enqueue<SaveSceneEvent>(mScenePath);
	});


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
				if (!menu.isOpen()) return;
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

	ui::Window::show("Assets Manager", [&]() {
		static std::string search;
		ui::Search::searchText(search);
		ui::AssetsManager::list(mRegistry, selectedEntity, search, showDebugEntities);

		ui::EditorDragAndDrop::loadMesh(mRegistry, selectedEntity);
		ui::EditorDragAndDrop::loadScript(mRegistry, selectedEntity);

		ui::Popup::show("Asset Manager Popup", [&]() {
			ui::AssetsManager::createMenu(mRegistry, selectedEntity);
			ui::SceneOptionsMenu::removeMenu(mRegistry, selectedEntity);
		});
	});

	ui::Window::show("Components", [&]() {
		ui::EntityProperties::popup(mRegistry, selectedEntity);
		ui::EntityProperties::displayComponents(
			mRegistry, selectedEntity, *mIconTexture,
			[&](const auto& res) { return mMaterialController.findResource(res); }, showDebugComponents);
	});

	ui::Window::show("Assets Explorer",
					 [&]() { ui::AssetsExplorer::displayFiles(mFileSystem, mCurrentPath, *mIconTexture); });
}

void EditorSystem::onPublishRegistry(ecs::RegistryCollection& publisher)
{
	publisher.append(mRegistry);
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
	createDefaultEditorEntities(mRegistry);
}

void EditorSystem::loadScene()
{
	try
	{
		auto stream = mFileSystem.openReadStream(mScenePath.c_str());
		mRegistry = parseRegistry(*stream);
		createDefaultEditorEntities(mRegistry);
	}
	catch (const std::exception& e)
	{
		gLogger.warn("Could not load scene: {}", e.what());
	}
}

void EditorSystem::saveScene()
{
	try
	{
		auto path = mScenePath;
		if (mScenePath.root_directory() != "project")
			path = "project" / path;

		if (!boost::ends_with(mScenePath.filename().c_str(), ".spatial.json"))
			path = std::filesystem::path{path.string() + ".spatial.json"};

		auto stream = mFileSystem.openWriteStream(path.c_str());
		writeRegistry(mRegistry, *stream);
	}
	catch (const std::exception& e)
	{
		gLogger.warn("Could not save scene: {}", e.what());
	}
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

} // namespace spatial::editor
