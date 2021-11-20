#include "SceneEditorSystem.h"
#include "CustomComponents.h"
#include "DefaultMaterial.h"
#include "EditorCamera.h"
#include "Serialization.h"
#include "Tags.h"

#include <assets/generated.h>

#include <spatial/core/Logger.h>
#include <spatial/render/Camera.h>
#include <spatial/render/ResourceLoaders.h>
#include <spatial/render/SkyboxResources.h>
#include <spatial/serialization/Archives.h>
#include <spatial/serialization/Registry.h>

#include <spatial/ui/components/AssetsExplorer.h>
#include <spatial/ui/components/Components.h>
#include <spatial/ui/components/DockSpace.h>
#include <spatial/ui/components/DragAndDrop.h>
#include <spatial/ui/components/Window.h>

#include <spatial/ui/components/styles/WindowPaddingStyle.h>

#include <boost/algorithm/string/predicate.hpp>

#include <fstream>
#include <spatial/ui/components/Search.h>
#include <string>

namespace fl = filament;
namespace fs = std::filesystem;

using namespace spatial::math;

namespace spatial::editor
{

auto gLogger = createDefaultLogger();

SceneEditorSystem::SceneEditorSystem(filament::Engine& engine, desktop::Window& window)
	: mEngine{engine},
	  mWindow{window},

	  mEditorScene{render::createScene(mEngine)},

	  mDefaultMaterial{render::createMaterial(mEngine, ASSETS_DEFAULT_FILAMAT, ASSETS_DEFAULT_FILAMAT_SIZE)},

	  mIblTexture{render::createKtxTexture(mEngine, ASSETS_DEFAULT_SKYBOX_IBL_KTX, ASSETS_DEFAULT_SKYBOX_IBL_KTX_SIZE)},
	  mSkyboxTexture{
		  render::createKtxTexture(mEngine, ASSETS_DEFAULT_SKYBOX_SKYBOX_KTX, ASSETS_DEFAULT_SKYBOX_SKYBOX_KTX_SIZE)},
	  mSkyboxLight{render::createImageBasedLight(mEngine, mIblTexture.ref(), ASSETS_SH_TXT, ASSETS_SH_TXT_SIZE)},
	  mSkybox{render::createSkybox(mEngine, mSkyboxTexture.ref())},

	  mIconTexture{render::createTexture(mEngine, ASSETS_ICONS_PNG, ASSETS_ICONS_PNG_SIZE)},

	  mRegistry{},

	  mEditorCameraController{},
	  mSceneController{mEngine, mEditorScene.ref()},
	  mMaterialController{mEngine},
	  mTransformController{mEngine},
	  mCameraController{mEngine, mEditorScene.ref()},
	  mLightController{mEngine},
	  mMeshController{mEngine},

	  mRootPath{},
	  mScenePath{"scenes/default.spatial.xml"},
	  mIsReloadSceneFlagEnabled{false},
	  mIsClearSceneFlagEnabled{false},
	  isSaveSceneFlagEnabled{false}
{
	auto materialInstance = toShared(render::createMaterialInstance(mEngine, mDefaultMaterial.ref()));
	materialInstance->setParameter("baseColor", math::float3{1.0f});
	materialInstance->setParameter("metallic", .1f);
	materialInstance->setParameter("roughness", 1.0f);
	materialInstance->setParameter("reflectance", .1f);

	mMeshController.setDefaultMaterialInstance(materialInstance);
}

void SceneEditorSystem::onStart()
{
	mEditorScene->setIndirectLight(mSkyboxLight.get());
	mEditorScene->setSkybox(mSkybox.get());

	mMeshController.load("editor://meshes/cube.filamesh"_hs,
						 loadFilameshFromMemory(ASSETS_CUBE_FILAMESH, ASSETS_CUBE_FILAMESH_SIZE));
	mMeshController.load("editor://meshes/sphere.filamesh"_hs,
						 loadFilameshFromMemory(ASSETS_SPHERE_FILAMESH, ASSETS_SPHERE_FILAMESH_SIZE));
	mMeshController.load("editor://meshes/plane.filamesh"_hs,
						 loadFilameshFromMemory(ASSETS_PLANE_FILAMESH, ASSETS_PLANE_FILAMESH_SIZE));
	mMeshController.load("editor://meshes/cylinder.filamesh"_hs,
						 loadFilameshFromMemory(ASSETS_CYLINDER_FILAMESH, ASSETS_CYLINDER_FILAMESH_SIZE));
}

void SceneEditorSystem::onStartFrame(float)
{
	if (mIsClearSceneFlagEnabled)
	{
		mRegistry = ecs::Registry{};
		mIsClearSceneFlagEnabled = false;
	}

	if (mIsReloadSceneFlagEnabled)
	{
		auto ss = std::ifstream{mRootPath / mScenePath};
		if (!ss)
			return;

		mRegistry = ecs::Registry{};
		auto xml = XMLInputArchive{ss};
		try
		{
			ecs::deserialize<DefaultMaterial, EditorCamera, tags::IsEditorEntity>(xml, mRegistry);
		}
		catch (const std::exception& e)
		{
			gLogger.error(
				fmt::format("Could not load scene: {0}\nReason: {1}", (mRootPath / mScenePath).string(), e.what()));
		}

		mIsReloadSceneFlagEnabled = false;
	}

	if (isSaveSceneFlagEnabled)
	{
		auto ss = std::ofstream{mRootPath / mScenePath};
		if (!ss)
			return;

		auto xml = XMLOutputArchive{ss};
		ecs::serialize<DefaultMaterial, EditorCamera, tags::IsEditorEntity>(xml, mRegistry);
		isSaveSceneFlagEnabled = false;
	}
}

void SceneEditorSystem::onUpdateFrame(float delta)
{
	mEditorCameraController.onUpdateFrame(mRegistry, delta);
	mSceneController.onUpdateFrame(mRegistry);
	mTransformController.onUpdateFrame(mRegistry);
	mCameraController.onUpdateFrame(mRegistry);
	mLightController.onUpdateFrame(mRegistry);
	mMeshController.onUpdateFrame(mRegistry, delta);
	mMaterialController.onUpdateFrame<DefaultMaterial>(mRegistry, mDefaultMaterial.ref());
}

void SceneEditorSystem::onDrawGui()
{
	auto dockSpace = ui::DockSpace{"Spatial"};

	static fs::path currentPath{""};
	static ecs::Entity selectedEntity{ecs::NullEntity};
	static bool showDebugEntities{false};

	auto mainMenu = ui::EditorMainMenu{mRootPath, currentPath};
	mIsReloadSceneFlagEnabled = mainMenu.onOpenScene();
	isSaveSceneFlagEnabled = mainMenu.onSaveScene();
	if (mainMenu.onOpenScene() || mainMenu.onNewScene())
	{
		mIsClearSceneFlagEnabled = true;
		mScenePath = currentPath;
	}

	const auto cameraEntity = mRegistry.getFirstEntity<const ecs::EntityName, const ecs::Transform,
													   const render::TextureView, tags::IsEditorEntity>();
	const auto* cameraTransform = mRegistry.getComponentIfExists<const ecs::Transform>(cameraEntity);
	const auto createEntityPosition =
		cameraTransform ? (cameraTransform->position + cameraTransform->getForwardVector() * 10.0f) : math::float3{};
	const auto* cameraRenderTextureView = mRegistry.getComponentIfExists<const render::TextureView>(cameraEntity);
	auto* perspectiveCamera = mRegistry.getComponentIfExists<ecs::PerspectiveCamera>(cameraEntity);
	auto* orthographicCamera = mRegistry.getComponentIfExists<ecs::OrthographicCamera>(cameraEntity);

	{
		auto style = ui::WindowPaddingStyle{};
		auto window = ui::Window{"Scene View"};

		const auto imageSize = window.getSize() - math::float2{0, 24};
		const auto aspectRatio = static_cast<double>(imageSize.x) / static_cast<double>(imageSize.y);

		if (perspectiveCamera)
			perspectiveCamera->aspectRatio = aspectRatio;

		if (orthographicCamera)
			orthographicCamera->setAspectRatio(aspectRatio);

		{
			auto style2 = ui::WindowPaddingStyle{4};
			auto popup = ui::Popup{"Scene View Popup"};
			if (popup.isOpen())
				ui::SceneOptionsMenu::createEntitiesMenu(mRegistry, selectedEntity, createEntityPosition);
		}

		if (cameraRenderTextureView)
			ui::image(cameraRenderTextureView->getColorTexture().ref(), imageSize, math::float4{0, 1, 1, 0});

		// TODO: Move cursor exactly to the center of the scene window (Not the center of the screen)
		if (ImGui::IsItemClicked() && mEditorCameraController.toggleControl())
			mWindow.warpMouse(mWindow.getSize() * .5f);

		mIsReloadSceneFlagEnabled |= ui::EditorDragAndDrop::loadScene(mScenePath, selectedEntity);
		ui::EditorDragAndDrop::loadMesh(mRegistry, selectedEntity, createEntityPosition);
	}

	ui::Window::show("Scene Tree", [&]() {
		ui::Popup::show("Scene Graph Popup", [&]() {
			ui::SceneOptionsMenu::createEntitiesMenu(mRegistry, selectedEntity, createEntityPosition);
			ui::SceneOptionsMenu::viewOptionsMenu(showDebugEntities);
		});

		static std::string search;
		ui::Search::searchText(search);
		ui::SceneTree::displayTree(mRegistry, selectedEntity, showDebugEntities, search);
	});

	ui::Window::show("Materials Manager", [&]() {
		ui::MaterialsManager::popup(mRegistry, selectedEntity);

		static std::string search;
		ui::Search::searchText(search);
		ui::MaterialsManager::list(mRegistry, selectedEntity, search);
	});

	ui::Window::show("Properties", [&]() {
		ui::EntityProperties::popup(mRegistry, selectedEntity);
		ui::EntityProperties::displayComponents(mRegistry, selectedEntity);
	});

	ui::Window::show("Assets Explorer", [&]() {
		ui::AssetsExplorer::displayFiles(mRootPath, currentPath, mIconTexture.get());
	});
}

void SceneEditorSystem::onRender(filament::Renderer& renderer) const
{
	mRegistry.getEntities<const render::TextureView, const tags::IsEditorEntity>().each(
		[&](const auto& textureView) { renderer.render(textureView.getView().get()); });
}

void SceneEditorSystem::onUpdateInput(const desktop::InputState& input)
{
	mEditorCameraController.onUpdateInput(input);
	if (input.released(Key::Escape))
		mEditorCameraController.disable();
}

void SceneEditorSystem::setRootPath(const std::filesystem::path& path)
{
	if (!std::filesystem::exists(path) && !std::filesystem::is_directory(path))
		return;
	mRootPath = path;
}

} // namespace spatial::editor
