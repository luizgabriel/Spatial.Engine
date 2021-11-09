#include "SceneEditorSystem.h"
#include "CustomComponents.h"
#include "DefaultMaterial.h"
#include "EditorCamera.h"
#include "Serialization.h"
#include "Settings.h"
#include "Tags.h"

#include <assets/generated.h>

#include <spatial/core/Logger.h>
#include <spatial/ecs/Tags.h>
#include <spatial/render/Camera.h>
#include <spatial/render/ResourceLoaders.h>
#include <spatial/render/SkyboxResources.h>
#include <spatial/serialization/Archives.h>
#include <spatial/serialization/Registry.h>

#include <spatial/ui/components/Components.h>
#include <spatial/ui/components/DockSpace.h>
#include <spatial/ui/components/Menu.h>
#include <spatial/ui/components/MenuBar.h>
#include <spatial/ui/components/NewSceneModal.h>
#include <spatial/ui/components/OpenSceneModal.h>
#include <spatial/ui/components/PropertiesPanel.h>
#include <spatial/ui/components/SaveSceneModal.h>
#include <spatial/ui/components/Window.h>
#include <spatial/ui/components/AssetsExplorer.h>
#include <spatial/ui/components/ComponentCollapse.h>
#include <spatial/ui/components/DragAndDrop.h>

#include <spatial/ui/components/styles/WindowPaddingStyle.h>

#include <fstream>

#include <string>

namespace fl = filament;
namespace fs = std::filesystem;

using namespace spatial::math;

namespace spatial::editor
{

auto gLogger = createDefaultLogger();

SceneEditorSystem::SceneEditorSystem(Settings settings, filament::Engine& engine, desktop::Window& window)
	: mSettings{std::move(settings)},
	  mEngine{engine},
	  mWindow{window},

	  mEditorView{mEngine, window.getSize()},
	  mEditorScene{render::createScene(mEngine)},

	  mDefaultMaterial{render::createMaterial(mEngine, ASSETS_DEFAULT_FILAMAT, ASSETS_DEFAULT_FILAMAT_SIZE)},

	  mIblTexture{render::createKtxTexture(mEngine, ASSETS_DEFAULT_SKYBOX_IBL_KTX, ASSETS_DEFAULT_SKYBOX_IBL_KTX_SIZE)},
	  mSkyboxTexture{
		  render::createKtxTexture(mEngine, ASSETS_DEFAULT_SKYBOX_SKYBOX_KTX, ASSETS_DEFAULT_SKYBOX_SKYBOX_KTX_SIZE)},
	  mSkyboxLight{render::createImageBasedLight(mEngine, mIblTexture.ref(), ASSETS_SH_TXT, ASSETS_SH_TXT_SIZE)},
	  mSkybox{render::createSkybox(mEngine, mSkyboxTexture.ref())},

	  mIconTexture{render::createTexture(mEngine, ASSETS_ICONS_PNG, ASSETS_ICONS_PNG_SIZE)},

	  mSelectedEntity{ecs::NullEntity},

	  mRegistry{},

	  mEditorCameraController{},
	  mSceneController{mEngine, mEditorScene.ref()},
	  mMaterialController{mEngine},
	  mTransformController{mEngine},
	  mCameraController{mEngine},
	  mLightController{mEngine},
	  mMeshController{mEngine, mSettings.assetsFolder},

	  mCurrentAssetsPath{mSettings.assetsFolder}
{
}

void SceneEditorSystem::onStart()
{
	mEditorScene->setIndirectLight(mSkyboxLight.get());
	mEditorScene->setSkybox(mSkybox.get());
	mEditorView.getView()->setScene(mEditorScene.get());
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

	auto cameraEntity = mRegistry.getFirstEntity<EditorCamera, render::Camera>();
	if (mRegistry.isValid(cameraEntity))
	{
		auto& camera = mRegistry.getComponent<render::Camera>(cameraEntity);
		mEditorView.getView()->setCamera(camera.getInstance());
	}
}

void SceneEditorSystem::onDrawGui()
{
	auto dockSpace = ui::DockSpace{"Spatial"};
	std::string_view menuPopup{};
	ui::mainMenu(menuPopup);

	static std::string scenePath{std::filesystem::path{"scenes"} / "scene.xml"};

	{
		auto style = ui::WindowPaddingStyle{};
		auto window = ui::Window{"Scene View"};
		const auto imageSize = window.getSize() - math::float2{0, 24};

		ui::image(mEditorView.getColorTexture().ref(), imageSize, math::float4{0, 1, 1, 0});
		onSceneWindowResized(imageSize);

		if (ImGui::IsItemClicked() && mEditorCameraController.toggleControl())
			mWindow.warpMouse(mWindow.getSize() * .5f);

		{
			auto dnd = ui::DragAndDropTarget{};
			if (dnd.isStarted()) {
				auto result = dnd.getPathPayload(ui::AssetsExplorer::DND_SELECTED_FILE);
				if (result) {
					scenePath = result.value().string();
					menuPopup = "Open Scene";
				}
			}
		}
	}

	ui::entitiesListPanel<tags::IsEditorEntity>("Debug", mRegistry, mSelectedEntity);
	ui::entitiesListPanel<ecs::tags::IsMeshMaterial>("Materials", mRegistry, mSelectedEntity);
	ui::entitiesListPanel("Scene Graph", mRegistry, mSelectedEntity,
						  ecs::ExcludeComponents<ecs::tags::IsMeshMaterial, tags::IsEditorEntity>);

	{
		auto panel = ui::PropertiesPanel{mRegistry, mSelectedEntity};
		if (mRegistry.isValid(mSelectedEntity))
		{
			auto collapse = ui::ComponentCollapse{mRegistry, mSelectedEntity};
			if (collapse.hasComponentAndIsOpen<EditorCamera>("Editor Camera"))
				ui::editorCameraComponent(mRegistry, mSelectedEntity);

			if (collapse.hasComponentAndIsOpen<DefaultMaterial>("Default Material"))
				ui::defaultMaterialComponent(mRegistry, mSelectedEntity);
		}
	}

	{
		auto assets = ui::AssetsExplorer{mSettings.assetsFolder, mIconTexture.ref()};
		assets.header(mCurrentAssetsPath);
		assets.onSelectPath(mCurrentAssetsPath);
	}


	if (menuPopup.data())
		ImGui::OpenPopup(menuPopup.data());

	const ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));


	{
		auto modal = ui::NewSceneModal{};
		if (modal.onConfirm())
			newScene();
	}

	{
		auto modal = ui::SaveSceneModal{scenePath};
		if (modal.onConfirm())
			saveScene(fs::path{scenePath});
	}

	{
		auto modal = ui::OpenSceneModal{scenePath};
		if (modal.onConfirm())
			loadScene(fs::path{scenePath});
	}
}

void SceneEditorSystem::saveScene(const fs::path& outputPath)
{
	auto ss = std::ofstream{mSettings.assetsFolder / outputPath};
	if (!ss)
		return;

	auto xml = XMLOutputArchive{ss};
	ecs::serialize<DefaultMaterial, EditorCamera, tags::IsEditorEntity>(xml, mRegistry);
}

void SceneEditorSystem::loadScene(const fs::path& inputPath)
{
	auto ss = std::ifstream{mSettings.assetsFolder / inputPath};
	if (!ss)
		return;

	newScene();
	auto xml = XMLInputArchive{ss};
	ecs::deserialize<DefaultMaterial, EditorCamera, tags::IsEditorEntity>(xml, mRegistry);
}

void SceneEditorSystem::onSceneWindowResized(const math::int2& size)
{
	mEditorCameraController.onEditorViewResized(mRegistry, size.x / static_cast<double>(size.y));
}

void SceneEditorSystem::onRender(filament::Renderer& renderer) const
{
	if (hasEditorCamera())
		renderer.render(mEditorView.getView().get());
}

void SceneEditorSystem::newScene()
{
	mRegistry = ecs::Registry{};
}

void SceneEditorSystem::onUpdateInput(const desktop::InputState& input)
{
	mEditorCameraController.onUpdateInput(input);
	if (input.released(Key::Escape))
		mEditorCameraController.disable();
}

bool SceneEditorSystem::hasEditorCamera() const
{
	return mRegistry.hasAnyEntity<EditorCamera, render::Camera>();
}

} // namespace spatial::editor
