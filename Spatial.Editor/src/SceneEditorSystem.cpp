#include "SceneEditorSystem.h"
#include "CustomComponents.h"
#include "EditorCamera.h"
#include "Materials.h"
#include "Serialization.h"
#include "Tags.h"

#include <assets/generated.h>

#include <spatial/render/Resources.h>
#include <spatial/render/SkyboxResources.h>

#include <spatial/ui/components/AssetsExplorer.h>
#include <spatial/ui/components/Components.h>
#include <spatial/ui/components/DockSpace.h>
#include <spatial/ui/components/Window.h>

#include <spatial/ui/components/styles/WindowPaddingStyle.h>

#include <fstream>
#include <spatial/ecs/SceneView.h>
#include <spatial/render/EntityBuilder.h>
#include <spatial/ui/components/MenuBar.h>
#include <spatial/ui/components/SceneView.h>
#include <spatial/ui/components/Search.h>

namespace fl = filament;
namespace fs = std::filesystem;

using namespace spatial::math;

namespace spatial::editor
{

SceneEditorSystem::SceneEditorSystem(filament::Engine& engine, desktop::Window& window)
	: mEngine{engine},
	  mWindow{window},

	  mStandardLitMaterial{
		  render::createMaterial(mEngine, ASSETS_STANDARD_LIT_FILAMAT, ASSETS_STANDARD_LIT_FILAMAT_SIZE)},
	  mColorMaterial{render::createMaterial(mEngine, ASSETS_COLOR_FILAMAT, ASSETS_COLOR_FILAMAT_SIZE)},
	  mSkyBoxMaterial{render::createMaterial(mEngine, ASSETS_SKYBOX_FILAMAT, ASSETS_SKYBOX_FILAMAT_SIZE)},
	  mGridMaterial{render::createMaterial(mEngine, ASSETS_GRID_FILAMAT, ASSETS_GRID_FILAMAT_SIZE)},
	  mIconTexture{render::createTexture(mEngine, ASSETS_ICONS_PNG, ASSETS_ICONS_PNG_SIZE)},

	  mRegistry{},

	  mEditorCameraController{},
	  mSceneController{mEngine},
	  mMaterialController{mEngine},
	  mTransformController{mEngine},
	  mCameraController{mEngine},
	  mLightController{mEngine},
	  mMeshController{mEngine},
	  mIndirectLightController{mEngine},

	  mJobQueue{},

	  mRootPath{}

{
	mJobQueue.connect<OpenProjectEvent>(*this);
	mJobQueue.connect<ClearSceneEvent>(*this);
	mJobQueue.connect<LoadSceneEvent>(*this);
	mJobQueue.connect<SaveSceneEvent>(*this);
}

void SceneEditorSystem::onStart()
{
	{
		auto ib = render::createFullScreenIndexBuffer(mEngine);
		ecs::build(mRegistry)
			.asMesh()
			.withResource("engine://fullscreen")
			.with<ecs::tags::IsMeshLoaded>()
			.with(render::createFullScreenVertexBuffer(mEngine))
			.with(render::MeshGeometries{{0, ib->getIndexCount()}})
			.with(std::move(ib))
			.with<tags::IsEditorEntity>();
	}

	{
		const auto filamesh = loadFilameshFromMemory(ASSETS_CUBE_FILAMESH, ASSETS_CUBE_FILAMESH_SIZE);
		render::build(mRegistry)
			.asFilamesh()
			.withFile(mEngine, filamesh)
			.asMesh()
			.withResource("editor://meshes/cube.filamesh")
			.with<tags::IsEditorEntity>();
	}

	{
		const auto filamesh = loadFilameshFromMemory(ASSETS_SPHERE_FILAMESH, ASSETS_SPHERE_FILAMESH_SIZE);
		render::build(mRegistry)
			.asFilamesh()
			.withFile(mEngine, filamesh)
			.asMesh()
			.withResource("editor://meshes/sphere.filamesh")
			.with<tags::IsEditorEntity>();
	}

	{
		const auto filamesh = loadFilameshFromMemory(ASSETS_PLANE_FILAMESH, ASSETS_PLANE_FILAMESH_SIZE);
		render::build(mRegistry)
			.asFilamesh()
			.withFile(mEngine, filamesh)
			.asMesh()
			.withResource("editor://meshes/plane.filamesh")
			.with<tags::IsEditorEntity>();
	}

	{
		const auto filamesh = loadFilameshFromMemory(ASSETS_CYLINDER_FILAMESH, ASSETS_CYLINDER_FILAMESH_SIZE);
		render::build(mRegistry)
			.asFilamesh()
			.withFile(mEngine, filamesh)
			.asMesh()
			.withResource("editor://meshes/cylinder.filamesh")
			.with<tags::IsEditorEntity>();
	}

	mMaterialController.load(
		"editor://textures/default_skybox/texture.ktx"_hs,
		render::createKtxTexture(mEngine, ASSETS_DEFAULT_SKYBOX_SKYBOX_KTX, ASSETS_DEFAULT_SKYBOX_SKYBOX_KTX_SIZE));

	mMaterialController.load("engine://dummy_cubemap"_hs, render::createDummyCubemap(mEngine));
	mMaterialController.load("engine://dummy_texture_white"_hs, render::createDummyTexture<0xFFFFFFFF>(mEngine));
	mMaterialController.load("engine://dummy_texture_black"_hs, render::createDummyTexture<0x00000000>(mEngine));

	mIndirectLightController.loadTexture("editor://textures/default_skybox/ibl.ktx"_hs, ASSETS_DEFAULT_SKYBOX_IBL_KTX,
										 ASSETS_DEFAULT_SKYBOX_IBL_KTX_SIZE);
	mIndirectLightController.loadIrradianceValues("editor://textures/default_skybox/sh.txt"_hs,
												  render::parseShFile(ASSETS_SH_TXT, ASSETS_SH_TXT_SIZE));
}

void createDefaultEditorEntities(ecs::Registry& registry)
{
	if (!registry.isValid(registry.getFirstEntity<SkyBoxMaterial>()))
		ecs::build(registry)
			.withName("Default SkyBox")
			.asMaterial<SkyBoxMaterial>({
				false,
				{math::float3{.0f}, 1.0f},
				{"editor://textures/default_skybox/texture.ktx"},
			});

	if (!registry.isValid(registry.getFirstEntity<GridMaterial>()))
		ecs::build(registry).withName("Grid Material").with<tags::IsEditorEntity>().asMaterial<GridMaterial>();

	if (!registry.isValid(registry.getFirstEntity<tags::IsGridPlane>()))
		ecs::build(registry)
			.withName("Grid Plane")
			.with<tags::IsEditorEntity>()
			.with<tags::IsGridPlane>()
			.asTransform()
			.withScale({100.0f, 1.0f, 100.0f})
			.withPosition({.0f, -0.01f, .0f})
			.asMeshInstance()
			.withMesh(ecs::Mesh::findByResource(registry, "editor://meshes/plane.filamesh"))
			.withMaterialAt(0, registry.getFirstEntity<GridMaterial>());

	if (!registry.isValid(registry.getFirstEntity<tags::IsSkyBoxMesh>()))
		ecs::build(registry)
			.withName("SkyBox")
			.with<tags::IsSkyBoxMesh>()
			.with<tags::IsEditorEntity>()
			.asMeshInstance()
			.withMesh(ecs::Mesh::findByResource(registry, "engine://fullscreen"))
			.withMaterialAt(0, registry.getFirstEntity<SkyBoxMaterial>())
			.withShadowOptions(false, false)
			.withCulling(false)
			.withPriority(0x7);

	if (!registry.isValid(registry.getFirstEntity<tags::IsEditorView>()))
		ecs::build(registry)
			.withName("Editor View")
			.with<tags::IsEditorEntity>()
			.with<tags::IsEditorView>()
			.asSceneView()
			.withIndirectLight(ecs::build(registry)
								   .withName("Indirect Light")
								   .with<tags::IsEditorEntity>()
								   .asIndirectLight()
								   .withReflectionsTexturePath("editor://textures/default_skybox/ibl.ktx")
								   .withIrradianceValuesPath("editor://textures/default_skybox/sh.txt"))
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
	mMaterialController.onStartFrame();
	createDefaultEditorEntities(mRegistry);

	mMeshController.onStartFrame(mRegistry, mRootPath);
}

void SceneEditorSystem::onUpdateFrame(float delta)
{
	mEditorCameraController.onUpdateFrame(mRegistry, delta);
	mSceneController.onUpdateFrame(mRegistry);
	mTransformController.onUpdateFrame(mRegistry);
	mCameraController.onUpdateFrame(mRegistry);
	mLightController.onUpdateFrame(mRegistry);
	mIndirectLightController.onUpdateFrame(mRegistry);

	mMaterialController.onUpdateFrame<GridMaterial>(mRegistry, mGridMaterial.ref());
	mMaterialController.onUpdateFrame<ColorMaterial>(mRegistry, mColorMaterial.ref());
	mMaterialController.onUpdateFrameWithFinder<StandardOpaqueMaterial>(mRegistry, mStandardLitMaterial.ref());
	mMaterialController.onUpdateFrameWithFinder<SkyBoxMaterial>(mRegistry, mSkyBoxMaterial.ref());

	mMeshController.onUpdateFrame(mRegistry);
}

void SceneEditorSystem::onDrawGui()
{
	auto dockSpace = ui::DockSpace{"Spatial"};

	static ecs::Entity selectedEntity{ecs::NullEntity};
	static bool showDebugEntities{false};

	ui::MenuBar::show([&]() {
		ui::EditorMainMenu::fileMenu(mIconTexture.ref());
		ui::EditorMainMenu::viewOptionsMenu(showDebugEntities);

		if (ui::OpenProjectModal::show(mRootPath))
			mJobQueue.enqueue<OpenProjectEvent>(mRootPath);

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
		auto window = ui::Window{"Scene View", ui::WindowFlags::NoTitleBar | ui::WindowFlags::NoScrollbar};

		const auto imageSize = window.getSize() - math::float2{0, 24};
		ui::SceneView::image(mRegistry, sceneView, imageSize);

		{
			auto style2 = ui::WindowPaddingStyle{4};
			auto popup = ui::Popup{"Scene View Popup"};
			if (popup.isOpen())
				ui::SceneOptionsMenu::createEntitiesMenu(mRegistry, selectedEntity, createEntityPosition);
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
					 [&]() { ui::AssetsExplorer::displayFiles(mRootPath, mCurrentPath, mIconTexture.ref()); });
}

void SceneEditorSystem::onRender(filament::Renderer& renderer) const
{
	mRegistry.getEntities<const render::TextureView, const tags::IsEditorView>().each(
		[&](const auto& textureView) { renderer.render(textureView.getView().get()); });
}

void SceneEditorSystem::onUpdateInput(const desktop::InputState& input)
{
	mEditorCameraController.onUpdateInput(input);
	if (input.released(Key::Escape))
		mEditorCameraController.disable();
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
	auto result = makeAbsolutePath(mRootPath, mScenePath)
					  .and_then(validateResourcePath)
					  .and_then(openFileReadStream)
					  .and_then(parseRegistry)
					  .map_error(logResourceError);

	if (result.has_value())
		mRegistry = std::move(result.value());
}

void SceneEditorSystem::saveScene()
{
	auto result = makeAbsolutePath(mRootPath, mScenePath).and_then(openFileWriteStream).map_error(logResourceError);

	if (result.has_value())
		writeRegistry(mRegistry, std::move(result.value()));
}

void SceneEditorSystem::setRootPath(const std::filesystem::path& path)
{
	if (!std::filesystem::exists(path) && !std::filesystem::is_directory(path))
		return;

	mRootPath = path;
	mCurrentPath = path;
	mIndirectLightController.setRootPath(mRootPath);
	mMaterialController.setRootPath(mRootPath);
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
