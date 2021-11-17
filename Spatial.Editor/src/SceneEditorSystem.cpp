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
	  isReloadSceneFlagEnabled{false},
	  isClearSceneFlagEnabled{false},
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
	if (isClearSceneFlagEnabled)
	{
		mRegistry = ecs::Registry{};
	}

	if (isReloadSceneFlagEnabled)
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
	}

	if (isSaveSceneFlagEnabled)
	{
		auto ss = std::ofstream{mRootPath / mScenePath};
		if (!ss)
			return;

		auto xml = XMLOutputArchive{ss};
		ecs::serialize<DefaultMaterial, EditorCamera, tags::IsEditorEntity>(xml, mRegistry);
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

	auto mainMenu = ui::EditorMainMenu{mRootPath, mScenePath};
	bool isDndLoadScene = false;

	{
		auto view = mRegistry.getEntities<const ecs::EntityName, render::TextureView, tags::IsEditorEntity>();
		for (auto entity : view)
		{
			const auto& name = mRegistry.getComponent<const ecs::EntityName>(entity);
			auto& textureView = mRegistry.getComponent<render::TextureView>(entity);

			auto style = ui::WindowPaddingStyle{};
			auto window = ui::Window{fmt::format("Scene View ({0})", name.name)};
			const auto imageSize = window.getSize() - math::float2{0, 24};
			const auto aspectRatio = static_cast<double>(imageSize.x) / static_cast<double>(imageSize.y);

			ui::image(textureView.getColorTexture().ref(), imageSize, math::float4{0, 1, 1, 0});

			if (auto* perspectiveCamera = mRegistry.getComponentIfExists<ecs::PerspectiveCamera>(entity);
				perspectiveCamera)
			{
				perspectiveCamera->aspectRatio = aspectRatio;
			}

			if (auto* orthographicCamera = mRegistry.getComponentIfExists<ecs::OrthographicCamera>(entity);
				orthographicCamera)
			{
				orthographicCamera->left = -aspectRatio;
				orthographicCamera->right = aspectRatio;
			}

			// TODO: Move cursor exactly to the center of the scene window (Not the center of the screen)
			if (ImGui::IsItemClicked() && mEditorCameraController.toggleControl())
				mWindow.warpMouse(mWindow.getSize() * .5f);

			{
				auto dnd = ui::DragAndDropTarget{};
				if (dnd.isStarted())
				{
					auto result = dnd.getPathPayload(ui::AssetsExplorer::DND_SELECTED_FILE);
					if (result && boost::algorithm::ends_with(result->filename().string(), ".xml"))
					{
						isDndLoadScene = true;
						selectedEntity = ecs::NullEntity;
						mScenePath = result.value();
					}

					if (result && boost::algorithm::ends_with(result->filename().string(), ".filamesh"))
					{
						ecs::build(mRegistry).withName(result->stem().string()).asTransform().asMesh(result.value());
					}
				}
			}
		}
	}

	{
		static std::string search{};
		static bool showDebugEntities{false};

		auto sceneGraph = ui::SceneGraphWindow{showDebugEntities};
		sceneGraph.header(mRegistry, search);
		sceneGraph.listTree(mRegistry, selectedEntity, search);
	}

	{
		static std::string search{};

		auto sceneGraph = ui::MaterialsWindow{};
		sceneGraph.header(mRegistry, search);
		sceneGraph.list(mRegistry, selectedEntity, search);
	}

	{
		auto panel = ui::PropertiesPanel{mRegistry, selectedEntity};
	}

	{
		auto assets = ui::AssetsExplorer{mRootPath, mIconTexture.ref()};
		assets.header(currentPath);
		assets.onSelectPath(currentPath);
	}

	if (mainMenu.onOpenProject())
		currentPath = mRootPath;

	isClearSceneFlagEnabled = mainMenu.onNewScene();
	isReloadSceneFlagEnabled = mainMenu.onOpenScene() || isDndLoadScene;
	isSaveSceneFlagEnabled = mainMenu.onSaveScene();
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
