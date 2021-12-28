#include "SceneEditorSystem.h"
#include "CustomComponents.h"
#include "EditorCamera.h"
#include "Materials.h"
#include "Serialization.h"
#include "Tags.h"

#include <assets/generated.h>

#include <spatial/common/EventQueue.h>
#include <spatial/render/Camera.h>
#include <spatial/render/SkyboxResources.h>
#include <spatial/serialization/Archives.h>
#include <spatial/serialization/Registry.h>

#include <spatial/ui/components/AssetsExplorer.h>
#include <spatial/ui/components/Components.h>
#include <spatial/ui/components/DockSpace.h>
#include <spatial/ui/components/Window.h>

#include <spatial/ui/components/styles/WindowPaddingStyle.h>

#include <fstream>
#include <spatial/ecs/SceneView.h>
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

	  mDefaultMaterial{render::createMaterial(mEngine, ASSETS_DEFAULT_FILAMAT, ASSETS_DEFAULT_FILAMAT_SIZE)},
	  mSkyBoxMaterial{render::createMaterial(mEngine, ASSETS_SKYBOX_FILAMAT, ASSETS_SKYBOX_FILAMAT_SIZE)},
	  mGridMaterial{render::createMaterial(mEngine, ASSETS_GRID_FILAMAT, ASSETS_GRID_FILAMAT_SIZE)},

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

	  mTextures{},

	  mRootPath{}

{
	mJobQueue.connect<OpenProjectEvent>(*this);
	mJobQueue.connect<ClearSceneEvent>(*this);
	mJobQueue.connect<LoadSceneEvent>(*this);
	mJobQueue.connect<SaveSceneEvent>(*this);
}

void SceneEditorSystem::onStart()
{
	mTextures.emplace("editor://textures/icons.png"_hs,
					  render::createTexture(mEngine, ASSETS_ICONS_PNG, ASSETS_ICONS_PNG_SIZE));
	mTextures.emplace(
		"editor://textures/default_skybox/texture.ktx"_hs,
		render::createKtxTexture(mEngine, ASSETS_DEFAULT_SKYBOX_SKYBOX_KTX, ASSETS_DEFAULT_SKYBOX_SKYBOX_KTX_SIZE));

	mTextures.emplace("engine://dummy_cubemap"_hs, render::createDummyCubemap(mEngine));

	mMeshController.load("editor://meshes/cube.filamesh"_hs,
						 loadFilameshFromMemory(ASSETS_CUBE_FILAMESH, ASSETS_CUBE_FILAMESH_SIZE));
	mMeshController.load("editor://meshes/sphere.filamesh"_hs,
						 loadFilameshFromMemory(ASSETS_SPHERE_FILAMESH, ASSETS_SPHERE_FILAMESH_SIZE));
	mMeshController.load("editor://meshes/plane.filamesh"_hs,
						 loadFilameshFromMemory(ASSETS_PLANE_FILAMESH, ASSETS_PLANE_FILAMESH_SIZE));
	mMeshController.load("editor://meshes/cylinder.filamesh"_hs,
						 loadFilameshFromMemory(ASSETS_CYLINDER_FILAMESH, ASSETS_CYLINDER_FILAMESH_SIZE));

	mIndirectLightController.loadTexture("editor://textures/default_skybox/ibl.ktx"_hs, ASSETS_DEFAULT_SKYBOX_IBL_KTX,
										 ASSETS_DEFAULT_SKYBOX_IBL_KTX_SIZE);
	mIndirectLightController.loadIrradianceValues("editor://textures/default_skybox/sh.txt"_hs,
												  render::parseShFile(ASSETS_SH_TXT, ASSETS_SH_TXT_SIZE));
}

void SceneEditorSystem::onStartFrame(float)
{
	mJobQueue.update();

	if (!mRegistry.isValid(mRegistry.getFirstEntity<SkyBoxMaterial>()))
	{
		ecs::build(mRegistry)
			.withName("Default SkyBox")
			.asMaterial<SkyBoxMaterial>({
				false,
				{math::float3{.0f}, 1.0f},
				{"editor://textures/default_skybox/texture.ktx"},
			});
	}

	if (!mRegistry.isValid(mRegistry.getFirstEntity<GridMaterial>()))
	{
		ecs::build(mRegistry).withName("Grid Material").with<tags::IsEditorEntity>().asMaterial<GridMaterial>();
	}

	if (!mRegistry.isValid(mRegistry.getFirstEntity<tags::IsGridPlane>()))
	{
		ecs::build(mRegistry)
			.withName("Grid Plane")
			.with<tags::IsEditorEntity>()
			.with<tags::IsGridPlane>()
			.asTransform()
			.withScale({100.0f})
			.asMesh()
			.withPath("editor://meshes/plane.filamesh")
			.withMaterialAt(0, mRegistry.getFirstEntity<GridMaterial>());
	}

	auto skyboxMeshEntity = mRegistry.getFirstEntity<tags::IsSkyBoxMesh>();
	if (!mRegistry.isValid(skyboxMeshEntity))
	{
		ecs::build(mRegistry)
			.withName("SkyBox")
			.with<tags::IsSkyBoxMesh>()
			.asMesh()
			.withPath("engine://fullscreen")
			.withMaterialAt(0, mRegistry.getFirstEntity<SkyBoxMaterial>())
			.withShadowOptions(false, false)
			.withCulling(false)
			.withPriority(0x7);
	}

	if (!mRegistry.isValid(mRegistry.getFirstEntity<tags::IsEditorView>()))
	{
		ecs::build(mRegistry)
			.withName("Editor View")
			.with<tags::IsEditorEntity>()
			.with<tags::IsEditorView>()
			.asSceneView()
			.withIndirectLight(ecs::build(mRegistry)
								   .withName("Indirect Light")
								   .with<tags::IsEditorEntity>()
								   .asIndirectLight()
								   .withReflectionsTexturePath("editor://textures/default_skybox/ibl.ktx")
								   .withIrradianceValuesPath("editor://textures/default_skybox/sh.txt"))
			.withCamera(ecs::build(mRegistry)
							.withName("Editor Camera")
							.with(EditorCamera{.5f, 10.0f})
							.with<tags::IsEditorEntity>()
							.asTransform()
							.withPosition({3.0f, 3.0f, 20.0f})
							.asPerspectiveCamera()
							.withFieldOfView(60.0)
							.withAspectRatio(19.0 / 6.0));
	}
}

void SceneEditorSystem::onUpdateFrame(float delta)
{
	mEditorCameraController.onUpdateFrame(mRegistry, delta);
	mSceneController.onUpdateFrame(mRegistry);
	mTransformController.onUpdateFrame(mRegistry);
	mCameraController.onUpdateFrame(mRegistry);
	mLightController.onUpdateFrame(mRegistry);
	mIndirectLightController.onUpdateFrame(mRegistry);

	mMaterialController.onUpdateFrame<DefaultMaterial>(mRegistry, mDefaultMaterial.ref());
	mMaterialController.onUpdateFrame<SkyBoxMaterial>(mRegistry, mSkyBoxMaterial.ref(),
													  [this](const auto& res) { return findResource(res); });
	mMaterialController.onUpdateFrame<GridMaterial>(mRegistry, mGridMaterial.ref());

	mMeshController.onUpdateFrame(mRegistry);
}

void SceneEditorSystem::onDrawGui()
{
	auto dockSpace = ui::DockSpace{"Spatial"};

	static ecs::Entity selectedEntity{ecs::NullEntity};
	static bool showDebugEntities{false};

	ui::MenuBar::show([this]() {
		ui::EditorMainMenu::fileMenu();
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
		auto window = ui::Window{"Scene View"};

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

		ui::EditorDragAndDrop::loadMesh(mRegistry, selectedEntity, createEntityPosition);
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

	ui::Window::show("Materials Manager", [&]() {
		ui::MaterialsManager::popup(mRegistry, selectedEntity);

		static std::string search;
		ui::Search::searchText(search);
		ui::MaterialsManager::list(mRegistry, selectedEntity, search, showDebugEntities);
	});

	ui::Window::show("Properties", [&]() {
		ui::EntityProperties::popup(mRegistry, selectedEntity);
		ui::EntityProperties::displayComponents(mRegistry, selectedEntity);
	});

	const auto& iconTexture = mTextures.at("editor://textures/icons.png"_hs);
	ui::Window::show("Assets Explorer",
					 [&]() { ui::AssetsExplorer::displayFiles(mRootPath, mCurrentPath, iconTexture.get()); });
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
	auto result = makeAbsolutePath(mRootPath, mScenePath)
					  .and_then(validateResourcePath)
					  .and_then(openFileWriteStream)
					  .map_error(logResourceError);

	if (result.has_value())
	{
		auto xml = XMLOutputArchive{result.value()};
		ecs::serialize<DefaultMaterial, SkyBoxMaterial, GridMaterial, EditorCamera, tags::IsEditorEntity, tags::IsSkyBoxMesh,
					   tags::IsEditorView>(xml, mRegistry);
	}
}

void SceneEditorSystem::setRootPath(const std::filesystem::path& path)
{
	if (!std::filesystem::exists(path) && !std::filesystem::is_directory(path))
		return;

	mRootPath = path;
	mCurrentPath = path;
	mMeshController.setRootPath(mRootPath);
	mIndirectLightController.setRootPath(mRootPath);
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
	setScenePath("scenes/scene.default.xml");
}

} // namespace spatial::editor
