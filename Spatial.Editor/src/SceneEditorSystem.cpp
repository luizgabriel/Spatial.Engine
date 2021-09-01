#include "SceneEditorSystem.h"

#include <assets/generated.h>

#include <spatial/core/Logger.h>
#include <spatial/ecs/RegistryUtils.h>
#include <spatial/ecs/Tags.h>
#include <spatial/render/Camera.h>
#include <spatial/render/ResourceLoaders.h>
#include <spatial/render/SkyboxResources.h>
#include <spatial/resources/FilameshFile.h>
#include <spatial/serialization/Archives.h>
#include <spatial/serialization/Registry.h>

#include <spatial/ui/components/Collapse.h>
#include <spatial/ui/components/Components.h>
#include <spatial/ui/components/DockSpace.h>
#include <spatial/ui/components/Menu.h>
#include <spatial/ui/components/MenuBar.h>
#include <spatial/ui/components/NewSceneModal.h>
#include <spatial/ui/components/OpenSceneModal.h>
#include <spatial/ui/components/PropertiesPanel.h>
#include <spatial/ui/components/SaveSceneModal.h>
#include <spatial/ui/components/Window.h>

#include <spatial/ui/components/styles/WindowPaddingStyle.h>

#include "CustomComponents.h"
#include "DefaultMaterial.h"
#include "EditorCamera.h"
#include "Serialization.h"
#include "Tags.h"

#include <fstream>
#include <spatial/ui/components/ComponentCollapse.h>

namespace fl = filament;
namespace fs = ghc::filesystem;

using namespace spatial::math;

namespace spatial::editor
{

auto gLogger = createDefaultLogger();

SceneEditorSystem::SceneEditorSystem(filament::Engine& engine, desktop::Window& window)
	: mEngine{engine},
	  mWindow{window},

	  mEditorView{mEngine, window.getSize()},
	  mEditorScene{render::createScene(mEngine)},

	  mDefaultMaterial{render::createMaterial(mEngine, ASSETS_DEFAULT_FILAMAT, ASSETS_DEFAULT_FILAMAT_SIZE)},

	  mIblTexture{
		  render::createKtxTexture(mEngine, ASSETS_DEFAULT_SKYBOX_IBL_KTX, ASSETS_DEFAULT_SKYBOX_IBL_KTX_SIZE)},
	  mSkyboxTexture{
		  render::createKtxTexture(mEngine, ASSETS_DEFAULT_SKYBOX_SKYBOX_KTX, ASSETS_DEFAULT_SKYBOX_SKYBOX_KTX_SIZE)},
	  mSkyboxLight{render::createImageBasedLight(mEngine, mIblTexture.ref(), ASSETS_SH_TXT, ASSETS_SH_TXT_SIZE)},
	  mSkybox{render::createSkybox(mEngine, mSkyboxTexture.ref())},

	  mSelectedEntity{ecs::NullEntity},

	  mRegistry{},

	  mEditorCameraController{},
	  mSceneController{mEngine, mEditorScene.ref()},
	  mMaterialController{mEngine},
	  mTransformController{mEngine},
	  mCameraController{mEngine},
	  mLightController{mEngine},
	  mMeshController{mEngine}

{
	mMeshController.load("editor://meshes/cube"_hs,
						 loadFilameshFromMemory(ASSETS_CUBE_FILAMESH, ASSETS_CUBE_FILAMESH_SIZE));
	mMeshController.load("editor://meshes/sphere"_hs,
						 loadFilameshFromMemory(ASSETS_SPHERE_FILAMESH, ASSETS_SPHERE_FILAMESH_SIZE));
	mMeshController.load("editor://meshes/plane"_hs,
						 loadFilameshFromMemory(ASSETS_PLANE_FILAMESH, ASSETS_PLANE_FILAMESH_SIZE));
	mMeshController.load("editor://meshes/cylinder"_hs,
						 loadFilameshFromMemory(ASSETS_CYLINDER_FILAMESH, ASSETS_CYLINDER_FILAMESH_SIZE));

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

	auto m1 = ecs::build(registry).withName("Red Material").asMaterial(DefaultMaterial{float3{.4f, 0.1f, 0.1f}});
	auto m2 = ecs::build(registry).withName("White Material").asMaterial(DefaultMaterial{float3{.8f, .8f, .8f}});
	auto m3 = ecs::build(registry).withName("Green Material").asMaterial(DefaultMaterial{float3{.1f, 0.4f, 0.1f}});
	auto m4 = ecs::build(registry).withName("Blue Material").asMaterial(DefaultMaterial{float3{.1f, 0.1f, 0.4}});

	ecs::build(registry)
		.withName("Cube")
		.asTransform()
		.withPosition({.0f})
		.asMesh("editor://meshes/cube"_hs)
		.withShadowOptions(true, true)
		.withMaterialAt(0, m1);

	ecs::build(registry)
		.withName("Plane")
		.asTransform()
		.withPosition({3.0f, -1.0f, .0f})
		.withScale({10.0f})
		.asMesh("editor://meshes/plane"_hs)
		.withShadowOptions(false, true)
		.withMaterialAt(0, m2);

	ecs::build(registry)
		.withName("Cylinder")
		.asTransform()
		.withPosition({6.0f, .0f, .0f})
		.asMesh("editor://meshes/cylinder"_hs)
		.withShadowOptions(true, true)
		.withMaterialAt(0, m3);

	return ecs::build(registry)
		.withName("Sphere")
		.asTransform()
		.withPosition({3.0f, .0f, .0f})
		.asMesh("editor://meshes/sphere"_hs)
		.withShadowOptions(true, true)
		.withMaterialAt(0, m4);
}

void SceneEditorSystem::onUpdateFrame(float delta)
{
	mEditorCameraController.onUpdateFrame(mRegistry, delta);
	mSceneController.onUpdateFrame(mRegistry);
	mTransformController.onUpdateFrame(mRegistry);
	mCameraController.onUpdateFrame(mRegistry);
	mLightController.onUpdateFrame(mRegistry);
	mMeshController.onUpdateFrame(mRegistry);
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
		const auto imageSize = window.getSize() - math::float2{0, 24};

		ui::image(mEditorView.getColorTexture().get(), imageSize);
		onSceneWindowResized(imageSize);

		if (ImGui::IsItemClicked() && mEditorCameraController.toggleControl())
			mWindow.warpMouse(mWindow.getSize() * .5f);
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

	if (menuPopup.data())
		ImGui::OpenPopup(menuPopup.data());

	static std::string scenePath{"scenes/scene.xml"};
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
	mEditorCameraController.onEditorViewResized(mRegistry, size.x / static_cast<double>(size.y));
}

void SceneEditorSystem::onRender(filament::Renderer& renderer) const
{
	assert(hasEditorCamera());
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
