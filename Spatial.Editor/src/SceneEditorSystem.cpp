#include "SceneEditorSystem.h"

#include <assets/generated.h>

#include <spatial/core/Logger.h>
#include <spatial/ecs/RegistryUtils.h>
#include <spatial/ecs/Tags.h>
#include <spatial/render/Camera.h>
#include <spatial/render/ResourceLoaders.h>
#include <spatial/render/SkyboxResources.h>
#include <spatial/resources/Common.h>
#include <spatial/resources/FilameshFile.h>
#include <spatial/serialization/Archives.h>
#include <spatial/serialization/Registry.h>

#include <spatial/ui/components/Collapse.h>
#include <spatial/ui/components/Components.h>
#include <spatial/ui/components/DockSpace.h>
#include <spatial/ui/components/Menu.h>
#include <spatial/ui/components/MenuBar.h>
#include <spatial/ui/components/PopupModal.h>
#include <spatial/ui/components/PropertiesPanel.h>
#include <spatial/ui/components/Window.h>
#include <spatial/ui/components/NewSceneModal.h>
#include <spatial/ui/components/OpenSceneModal.h>
#include <spatial/ui/components/SaveSceneModal.h>

#include <spatial/ui/components/styles/WindowPaddingStyle.h>

#include "DefaultMaterial.h"
#include "EditorCamera.h"
#include "Serialization.h"
#include "Tags.h"

#include <fstream>


namespace fl = filament;
namespace fs = ghc::filesystem;

using namespace spatial::math;

namespace spatial::editor
{

auto gLogger = createDefaultLogger();

SceneEditorSystem::SceneEditorSystem(filament::Engine& engine, desktop::Window& window,
									 const desktop::InputState& inputState)
	: mEngine{engine},
	  mWindow{window},
	  mInputState{inputState},

	  mEditorView{mEngine, window.getSize()},
	  mEditorScene{render::createScene(mEngine)},

	  mDefaultMaterial{render::createMaterial(mEngine, {ASSETS_DEFAULT_FILAMAT, ASSETS_DEFAULT_FILAMAT_SIZE})},

	  mIblTexture{
		  render::createKtxTexture(mEngine, {ASSETS_DEFAULT_SKYBOX_IBL_KTX, ASSETS_DEFAULT_SKYBOX_IBL_KTX_SIZE})},
	  mSkyboxTexture{
		  render::createKtxTexture(mEngine, {ASSETS_DEFAULT_SKYBOX_SKYBOX_KTX, ASSETS_DEFAULT_SKYBOX_SKYBOX_KTX_SIZE})},
	  mSkyboxLight{render::createImageBasedLight(mEngine, mIblTexture.ref(), {ASSETS_SH_TXT, ASSETS_SH_TXT_SIZE})},
	  mSkybox{render::createSkybox(mEngine, mSkyboxTexture.ref())},

	  mSelectedEntity{ecs::NullEntity},

	  mRegistry{},

	  mCameraEditorScript{mRegistry, mWindow, mInputState},

	  mSceneControllerSystem{mEngine, mEditorScene.ref()},
	  mMaterialInstancesSystem{mEngine},
	  mTransformSystem{mEngine},
	  mCameraSystem{mEngine},
	  mLightSystem{mEngine},
	  mRenderableMeshSystem{mEngine}

{
	mRenderableMeshSystem.load("editor://meshes/cube",
							   fromEmbed<FilameshFile>(ASSETS_CUBE_FILAMESH, ASSETS_CUBE_FILAMESH_SIZE));
	mRenderableMeshSystem.load("editor://meshes/sphere",
							   fromEmbed<FilameshFile>(ASSETS_SPHERE_FILAMESH, ASSETS_SPHERE_FILAMESH_SIZE));
	mRenderableMeshSystem.load("editor://meshes/plane",
							   fromEmbed<FilameshFile>(ASSETS_PLANE_FILAMESH, ASSETS_PLANE_FILAMESH_SIZE));
	mRenderableMeshSystem.load("editor://meshes/cylinder",
							   fromEmbed<FilameshFile>(ASSETS_CYLINDER_FILAMESH, ASSETS_CYLINDER_FILAMESH_SIZE));

	createDefaultScene(mRegistry);
}

void SceneEditorSystem::onStart()
{
	mEditorScene->setIndirectLight(mSkyboxLight.get());
	mEditorScene->setSkybox(mSkybox.get());
	mEditorView.getView()->setScene(mEditorScene.get());
}

ecs::Entity createDefaultScene(ecs::Registry& registry)
{
	ecs::build(registry).withName("Main Light").asDirectionalLight().withDirection({.34f, -.66f, -.67f});

	{
		auto m1 = ecs::build(registry).withName("Red Material").asMaterial(DefaultMaterial{float3{.4f, 0.1f, 0.1f}});

		ecs::build(registry)
			.withName("Cube")
			.asTransform()
			.withPosition({.0f})
			.asMesh("editor://meshes/cube")
			.withShadowOptions(true, true)
			.withMaterialAt(0, m1);
	}

	{
		auto m2 = ecs::build(registry).withName("White Material").asMaterial(DefaultMaterial{float3{.8f, .8f, .8f}});

		ecs::build(registry)
			.withName("Plane")
			.asTransform()
			.withPosition({3.0f, -1.0f, .0f})
			.withScale({10.0f})
			.asMesh("editor://meshes/plane")
			.withShadowOptions(false, true)
			.withMaterialAt(0, m2);
	}

	{
		auto m3 = ecs::build(registry).withName("Green Material").asMaterial(DefaultMaterial{float3{.1f, 0.4f, 0.1f}});

		ecs::build(registry)
			.withName("Cylinder")
			.asTransform()
			.withPosition({6.0f, .0f, .0f})
			.asMesh("editor://meshes/cylinder")
			.withShadowOptions(true, true)
			.withMaterialAt(0, m3);
	}

	{
		auto m4 = ecs::build(registry).withName("Blue Material").asMaterial(DefaultMaterial{float3{.1f, 0.1f, 0.4}});

		return ecs::build(registry)
			.withName("Sphere")
			.asTransform()
			.withPosition({3.0f, .0f, .0f})
			.asMesh("editor://meshes/sphere")
			.withShadowOptions(true, true)
			.withMaterialAt(0, m4);
	}
}

void SceneEditorSystem::onUpdateFrame(float delta)
{
	mCameraEditorScript.onUpdateFrame(delta);

	mSceneControllerSystem.synchronize(mRegistry);
	mTransformSystem.synchronize(mRegistry);
	mCameraSystem.synchronize(mRegistry);
	mLightSystem.synchronize(mRegistry);
	mRenderableMeshSystem.synchronize(mRegistry);

	mMaterialInstancesSystem.synchronize<DefaultMaterial>(mRegistry, mDefaultMaterial.ref());
	mMaterialInstancesSystem.clearRemovedMaterials<DefaultMaterial>(mRegistry);

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

	{
		auto mainMenu = ui::MenuBar{};

		{
			auto menu = ui::Menu{"File"};
			if (menu.isOpen())
			{
				if (menu.item("New", "CTRL+N"))
					menuPopup = "New Scene";
				if (menu.item("Open", "CTRL+O"))
					menuPopup = "Open Scene";
				if (menu.item("Save", "CTRL+S"))
					menuPopup = "Save Scene";
			}
		}

		{
			auto menu = ui::Menu{"Scene"};
			if (menu.isOpen())
			{
				if (menu.item("Create Default", "CTRL+N"))
					createDefaultScene(mRegistry);
			}
		}
	}

	{
		auto style = ui::WindowPaddingStyle{};
		auto window = ui::Window{"Scene View"};
		ui::image(mEditorView.getColorTexture().get(), window.getSize() - int2{0, 25});
		onSceneWindowResized(window.getSize());

		if (ImGui::IsItemClicked())
			mCameraEditorScript.toggleControl();
	}

	ui::entitiesListPanel<tags::IsEditorEntity>("Debug", mRegistry, mSelectedEntity);
	ui::entitiesListPanel<ecs::tags::IsMeshMaterial>("Materials", mRegistry, mSelectedEntity);
	ui::entitiesListPanel("Scene Graph", mRegistry, mSelectedEntity,
						  ecs::ExcludeComponents<ecs::tags::IsMeshMaterial, tags::IsEditorEntity>);

	{
		auto panel = ui::PropertiesPanel{mRegistry, mSelectedEntity};
		if (mRegistry.isValid(mSelectedEntity))
		{
			ui::collapseComponentInput<EditorCamera>("Editor Camera", mRegistry, mSelectedEntity);
			ui::collapseComponentInput<DefaultMaterial>("Default Material", mRegistry, mSelectedEntity);
		}
	}

	if (menuPopup.data())
		ImGui::OpenPopup(menuPopup.data());

	static std::string scenePath{"scene.xml"};
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
	gLogger.info("Saving scene: {}", fs::absolute(outputPath).string());
	auto ss = std::fstream{outputPath, std::ios_base::out | std::ios_base::trunc};
	if (!ss)
		return;

	auto xml = XMLOutputArchive{ss};
	ecs::serialize<DefaultMaterial, EditorCamera, tags::IsEditorEntity>(xml, mRegistry);
}

void SceneEditorSystem::loadScene(const fs::path& inputPath)
{
	gLogger.info("Loading scene: {}", fs::absolute(inputPath).string());
	auto ss = std::fstream{inputPath, std::ios_base::in};
	if (!ss)
		return;

	newScene();
	auto xml = XMLInputArchive{ss};
	ecs::deserialize<DefaultMaterial, EditorCamera, tags::IsEditorEntity>(xml, mRegistry);
}

void SceneEditorSystem::onSceneWindowResized(const math::int2& size)
{
	auto width = static_cast<double>(size.x);
	auto height = static_cast<double>(size.y);
	mCameraEditorScript.onEditorViewResized(width / height);
}

void SceneEditorSystem::onRender(filament::Renderer& renderer) const
{
	renderer.render(mEditorView.getView().get());
}

void SceneEditorSystem::newScene()
{
	mRegistry = ecs::Registry{};
}

} // namespace spatial::editor
