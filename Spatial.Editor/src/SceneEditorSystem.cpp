#include "SceneEditorSystem.h"
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
#include <spatial/ecs/SceneView.h>
#include <spatial/ecs/RegistryUtils.h>
#include <spatial/resources/MemoryFileSystem.h>
#include <spatial/ui/components/MenuBar.h>
#include <spatial/ui/components/SceneView.h>
#include <spatial/ui/components/Search.h>

namespace fl = filament;
namespace fs = std::filesystem;

using namespace spatial::math;

namespace spatial::editor
{

static auto gLogger = createDefaultLogger();

SceneEditorSystem::SceneEditorSystem(filament::Engine& engine, desktop::Window& window, FileSystem& fileSystem, script::PlatformContext& scriptContext)
	: mEngine{engine},
	  mWindow{window},

	  mStandardLitMaterial{
		  render::createMaterial(mEngine, ASSETS_STANDARD_LIT_FILAMAT, ASSETS_STANDARD_LIT_FILAMAT_SIZE)},
	  mColorMaterial{render::createMaterial(mEngine, ASSETS_COLOR_FILAMAT, ASSETS_COLOR_FILAMAT_SIZE)},
	  mSkyBoxMaterial{render::createMaterial(mEngine, ASSETS_SKYBOX_FILAMAT, ASSETS_SKYBOX_FILAMAT_SIZE)},
	  mGridMaterial{render::createMaterial(mEngine, ASSETS_GRID_FILAMAT, ASSETS_GRID_FILAMAT_SIZE)},
	  mIconTexture{render::createTexture(mEngine, ASSETS_ICONS_PNG, ASSETS_ICONS_PNG_SIZE)},

	  mFileSystem{fileSystem},
	  mProjectFileSystem{mFileSystem.mount<PhysicalFileSystem>(PROJECT_DIR)},

	  mRegistry{},

	  mEditorCameraController{},
	  mMaterialController{mEngine, mFileSystem},
	  mScriptController{scriptContext.createIsolate()},

	  mJobQueue{},

	  mScenePath{},
	  mCurrentPath{}

{
	mJobQueue.connect<OpenProjectEvent>(*this);
	mJobQueue.connect<ClearSceneEvent>(*this);
	mJobQueue.connect<LoadSceneEvent>(*this);
	mJobQueue.connect<SaveSceneEvent>(*this);
}

void SceneEditorSystem::onStart()
{
	mMaterialController.load("engine/dummy_cubemap"_hs, render::createDummyCubemap(mEngine));
	mMaterialController.load("engine/dummy_texture_white"_hs, render::createDummyTexture<0xFFFFFFFF>(mEngine));
	mMaterialController.load("engine/dummy_texture_black"_hs, render::createDummyTexture<0x00000000>(mEngine));
}

void createDefaultEditorEntities(ecs::Registry& registry)
{
	if (!ecs::handleOf<tags::IsSkyBoxMeshResource>(registry))
		ecs::build(registry).with<tags::IsEditorEntity>().with<tags::IsSkyBoxMeshResource>().asMesh().withResource(
			"engine/skybox");

	if (!ecs::handleOf<SkyBoxMaterial>(registry))
		ecs::build(registry)
			.withName("Default SkyBox")
			.asMaterial<SkyBoxMaterial>({
				false,
				{math::float3{.0f}, 1.0f},
				{"editor/textures/skybox/texture.ktx"},
			});

	if (!ecs::handleOf<GridMaterial>(registry))
		ecs::build(registry).withName("Grid Material").with<tags::IsEditorEntity>().asMaterial<GridMaterial>();

	if (!ecs::handleOf<tags::IsGridPlane>(registry))
		ecs::build(registry)
			.withName("Grid Plane")
			.with<tags::IsEditorEntity>()
			.with<tags::IsGridPlane>()
			.asTransform()
			.withScale({100.0f, 1.0f, 100.0f})
			.withPosition({.0f, -0.01f, .0f})
			.asMeshInstance()
			.withMesh(ecs::Mesh::findOrCreate(registry, "editor/meshes/plane.filamesh"))
			.withMaterialAt(0, registry.getFirstEntity<GridMaterial>());

	if (!ecs::handleOf<tags::IsSkyBoxMeshInstance>(registry))
		ecs::build(registry)
			.withName("SkyBox")
			.with<tags::IsSkyBoxMeshInstance>()
			.asMeshInstance()
			.withMesh(registry.getFirstEntity<tags::IsSkyBoxMeshResource>())
			.withMaterialAt(0, registry.getFirstEntity<SkyBoxMaterial>())
			.withShadowOptions(false, false)
			.withCulling(false)
			.withPriority(0x7);

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

void SceneEditorSystem::onStartFrame(float)
{
	mJobQueue.update();

	createDefaultEditorEntities(mRegistry);

	auto skyboxMeshResource = ecs::handleOf<tags::IsSkyBoxMeshResource>(mRegistry);
	if (!skyboxMeshResource.has<ecs::tags::IsMeshLoaded>())
	{
		auto ib = render::createFullScreenIndexBuffer(mEngine);
		skyboxMeshResource.add<ecs::tags::IsMeshLoaded>();
		skyboxMeshResource.add(render::createFullScreenVertexBuffer(mEngine));
		skyboxMeshResource.add(render::MeshGeometries{{0, ib->getIndexCount()}});
		skyboxMeshResource.add(std::move(ib));
	}

	mMaterialController.onStartFrame();
}

void SceneEditorSystem::onUpdateFrame(float delta)
{
	mEditorCameraController.onUpdateFrame(mRegistry, delta);

	mMaterialController.onUpdateFrame<GridMaterial>(mRegistry, mGridMaterial.ref());
	mMaterialController.onUpdateFrame<ColorMaterial>(mRegistry, mColorMaterial.ref());
	mMaterialController.onUpdateFrameWithFinder<StandardOpaqueMaterial>(mRegistry, mStandardLitMaterial.ref());
	mMaterialController.onUpdateFrameWithFinder<SkyBoxMaterial>(mRegistry, mSkyBoxMaterial.ref());
}

void SceneEditorSystem::onDrawGui()
{
	auto dockSpace = ui::DockSpace{"Spatial"};

	static ecs::Entity selectedEntity{ecs::NullEntity};
	static bool showDebugEntities{false};
	static std::filesystem::path rootPath = std::filesystem::current_path();

	ui::MenuBar::show([&]() {
		ui::EditorMainMenu::fileMenu(mIconTexture.ref());
		ui::EditorMainMenu::viewOptionsMenu(showDebugEntities);

		if (ui::OpenProjectModal::show(rootPath))
			mJobQueue.enqueue<OpenProjectEvent>(rootPath);

		if (ui::NewSceneModal::show())
			mJobQueue.enqueue<ClearSceneEvent>();

		if (ui::OpenSceneModal::show(mScenePath))
			mJobQueue.enqueue<LoadSceneEvent>(mScenePath);

		if (ui::SaveSceneModal::show(mScenePath))
			mJobQueue.enqueue<SaveSceneEvent>(mScenePath);
	});

	const auto cameraEntity = mRegistry.getFirstEntity<const ecs::Transform, EditorCamera>();
	const auto* cameraTransform = mRegistry.tryGetComponent<const ecs::Transform>(cameraEntity);
	const auto createEntityPosition =
		cameraTransform ? (cameraTransform->position + cameraTransform->getForwardVector() * 10.0f) : math::float3{};

	{
		auto sceneView = mRegistry.getFirstEntity<ecs::SceneView, render::TextureView>();
		auto style = ui::WindowPaddingStyle{};
		auto window = ui::Window{"Scene View", ui::WindowFlags::NoScrollbar};

		const auto imageSize = window.getSize() - math::float2{0, 24};
		ui::SceneView::image(mRegistry, sceneView, imageSize);

		{
			auto style2 = ui::WindowPaddingStyle{4};
			ui::Popup::show("Scene View Popup", [&]() {
				ui::SceneOptionsMenu::createEntitiesMenu(mRegistry, selectedEntity, createEntityPosition);
			});
		}

		// TODO: Move cursor exactly to the center of the scene window (Not the center of the screen)
		if (ImGui::IsItemClicked() && mEditorCameraController.toggleControl())
			mWindow.warpMouse(mWindow.getSize() * .5f);

		if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Right))
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
		ui::AssetsManager::popup(mRegistry, selectedEntity);

		static std::string search;
		ui::Search::searchText(search);
		ui::AssetsManager::list(mRegistry, selectedEntity, search, showDebugEntities);
		ui::EditorDragAndDrop::loadMesh(mRegistry, selectedEntity);
	});

	ui::Window::show("Properties", [&]() {
		ui::EntityProperties::popup(mRegistry, selectedEntity);
		ui::EntityProperties::displayComponents(mRegistry, selectedEntity, mIconTexture.ref(),
												[&](const auto& res) { return mMaterialController.findResource(res); });
	});

	ui::Window::show("Assets Explorer",
					 [&]() { ui::AssetsExplorer::displayFiles(mFileSystem, mCurrentPath, mIconTexture.ref()); });
}

void SceneEditorSystem::onPublishRegistry(ecs::RegistryCollection& publisher)
{
	publisher.append(mRegistry);
}

void SceneEditorSystem::onUpdateInput(const desktop::InputState& input)
{
	mEditorCameraController.onUpdateInput(input);
	if (input.released(Key::Escape))
		mEditorCameraController.disable();

	if (input.combined(Key::LControl, Key::S))
		mJobQueue.enqueue<SaveSceneEvent>();
}

void SceneEditorSystem::setScenePath(const std::filesystem::path& path)
{
	mScenePath = path;
}

void SceneEditorSystem::clearScene()
{
	mRegistry = ecs::Registry{};
}

void SceneEditorSystem::loadScene()
{
	try
	{
		auto stream = mFileSystem.openReadStream(mScenePath.c_str());
		mRegistry = parseRegistry(*stream);
	}
	catch (const std::exception& e)
	{
		gLogger.warn("Could not load scene: {}", e.what());
	}
}

void SceneEditorSystem::saveScene()
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

void SceneEditorSystem::setRootPath(const std::filesystem::path& path)
{
	if (!std::filesystem::exists(path) && !std::filesystem::is_directory(path))
		return;

	mCurrentPath = PROJECT_DIR;
	mProjectFileSystem->setRootPath(path);
}

void SceneEditorSystem::onEvent(const ClearSceneEvent&)
{
	clearScene();
}

void SceneEditorSystem::onEvent(const LoadSceneEvent& event)
{
	setScenePath(event.path);
	loadScene();
}

void SceneEditorSystem::onEvent(const SaveSceneEvent& event)
{
	setScenePath(event.path);
	saveScene();
}

void SceneEditorSystem::onEvent(const OpenProjectEvent& event)
{
	clearScene();
	setRootPath(event.path);
	setScenePath("scenes/default.spatial.json");
}

} // namespace spatial::editor
