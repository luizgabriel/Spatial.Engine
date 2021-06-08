#include "SceneEditorSystem.h"

#include <assets/generated.h>
#include <filament/LightManager.h>
#include <spatial/common/StringHelpers.h>
#include <spatial/core/Logger.h>
#include <spatial/ecs/RegistryUtils.h>
#include <spatial/ecs/Tags.h>
#include <spatial/render/Camera.h>
#include <spatial/render/ResourceLoaders.h>
#include <spatial/render/SkyboxResources.h>
#include <spatial/render/Transform.h>
#include <spatial/resources/Common.h>
#include <spatial/resources/FilameshFile.h>
#include <spatial/serialization/Archives.h>
#include <spatial/serialization/Registry.h>

#include "DefaultMaterial.h"
#include "EditorCamera.h"
#include "ImGuiComponents.h"
#include "ImGuiDockSpace.h"
#include "ImGuiWindow.h"
#include "Serialization.h"

namespace fl = filament;

using namespace spatial::math;

namespace spatial::editor
{

auto gLogger = createDefaultLogger();

SceneEditorSystem::SceneEditorSystem(filament::Engine& engine, desktop::Window& window,
									 const desktop::InputState& inputState)
	: mEngine{engine},
	  mWindow{window},
	  mInputState{inputState},

	  mMainStageRegistry{},

	  mEditorView{render::createView(mEngine)},
	  mEditorScene{render::createScene(mEngine)},

	  mSceneControllerSystem{mEngine, mEditorScene.ref()},
	  mMaterialInstancesSystem{mEngine},
	  mTransformSystem{mEngine},
	  mCameraSystem{mEngine},
	  mLightSystem{mEngine},
	  mRenderableMeshSystem{mEngine},

	  mDefaultMaterial{render::createMaterial(mEngine, {ASSETS_DEFAULT_FILAMAT, ASSETS_DEFAULT_FILAMAT_SIZE})},

	  mIblTexture{
		  render::createKtxTexture(mEngine, {ASSETS_DEFAULT_SKYBOX_IBL_KTX, ASSETS_DEFAULT_SKYBOX_IBL_KTX_SIZE})},
	  mSkyboxTexture{
		  render::createKtxTexture(mEngine, {ASSETS_DEFAULT_SKYBOX_SKYBOX_KTX, ASSETS_DEFAULT_SKYBOX_SKYBOX_KTX_SIZE})},
	  mSkyboxLight{render::createImageBasedLight(mEngine, mIblTexture.ref(), {ASSETS_SH_TXT, ASSETS_SH_TXT_SIZE})},
	  mSkybox{render::createSkybox(mEngine, mSkyboxTexture.ref())},

	  mImGuiSceneWindow{mEngine, mWindow.getSize()},

	  mSelectedEntity{ecs::NullEntity},

	  mCameraEditorScript{mMainStageRegistry, mWindow, mInputState}
{
	mImGuiSceneWindow >> *this; // register imgui window resize events

	mRenderableMeshSystem.define("editor://meshes/cube", fromEmbed<FilameshFile>(ASSETS_CUBE_FILAMESH, ASSETS_CUBE_FILAMESH_SIZE));
	mRenderableMeshSystem.define("editor://meshes/sphere",
								 fromEmbed<FilameshFile>(ASSETS_SPHERE_FILAMESH, ASSETS_SPHERE_FILAMESH_SIZE));
	mRenderableMeshSystem.define("editor://meshes/plane", fromEmbed<FilameshFile>(ASSETS_PLANE_FILAMESH, ASSETS_PLANE_FILAMESH_SIZE));
	mRenderableMeshSystem.define("editor://meshes/cylinder",
								 fromEmbed<FilameshFile>(ASSETS_CYLINDER_FILAMESH, ASSETS_CYLINDER_FILAMESH_SIZE));
}

void SceneEditorSystem::onStart()
{
	mEditorScene->setIndirectLight(mSkyboxLight.get());
	mEditorScene->setSkybox(mSkybox.get());

	mCameraEditorScript.onStart();

	const auto windowSize = mWindow.getSize();
	mEditorView->setViewport({0, 0, static_cast<uint32_t>(windowSize.x), static_cast<uint32_t>(windowSize.y)});
	mEditorView->setRenderTarget(mImGuiSceneWindow.getRenderTarget());
	mEditorView->setScene(mEditorScene.get());

	ecs::build(mMainStageRegistry).withName("Main Light").asDirectionalLight().withDirection({.34f, -.66f, -.67f});

	{
		auto m1 = ecs::build(mMainStageRegistry)
					  .withName("Red Material")
					  .asMaterial(DefaultMaterial{math::float3{.4f, 0.1f, 0.1f}});

		ecs::build(mMainStageRegistry)
			.withName("Cube")
			.asTransform()
			.withPosition({.0f})
			.asMesh("editor://meshes/cube")
			.withShadowOptions(true, true)
			.withMaterialAt(0, m1);
	}

	{
		auto m2 = ecs::build(mMainStageRegistry)
					  .withName("White Material")
					  .asMaterial(DefaultMaterial{math::float3{.8f, .8f, .8f}});

		ecs::build(mMainStageRegistry)
			.withName("Plane")
			.asTransform()
			.withPosition({3.0f, -1.0f, .0f})
			.withScale({10.0f})
			.asMesh("editor://meshes/plane")
			.withShadowOptions(false, true)
			.withMaterialAt(0, m2);
	}

	{
		auto m3 = ecs::build(mMainStageRegistry)
					  .withName("Green Material")
					  .asMaterial(DefaultMaterial{math::float3{.1f, 0.4f, 0.1f}});

		ecs::build(mMainStageRegistry)
			.withName("Cylinder")
			.asTransform()
			.withPosition({6.0f, .0f, .0f})
			.asMesh("editor://meshes/cylinder")
			.withShadowOptions(true, true)
			.withMaterialAt(0, m3);
	}

	{
		auto m4 = ecs::build(mMainStageRegistry)
					  .withName("Blue Material")
					  .asMaterial(DefaultMaterial{math::float3{.1f, 0.1f, 0.4}});

		ecs::build(mMainStageRegistry)
			.withName("Sphere")
			.asTransform()
			.withPosition({3.0f, .0f, .0f})
			.asMesh("editor://meshes/sphere")
			.withShadowOptions(true, true)
			.withMaterialAt(0, m4);
	}

	mMaterialInstancesSystem.synchronize<DefaultMaterial>(mMainStageRegistry, mDefaultMaterial.ref());

	onSceneWindowResized({1280, 720});
}

void SceneEditorSystem::onUpdateFrame(float delta)
{
	mSceneControllerSystem.synchronize(mMainStageRegistry);
	mMaterialInstancesSystem.synchronize<DefaultMaterial>(mMainStageRegistry, mDefaultMaterial.ref());
	mTransformSystem.synchronize(mMainStageRegistry);
	mCameraSystem.synchronize(mMainStageRegistry);
	mLightSystem.synchronize(mMainStageRegistry);
	mRenderableMeshSystem.synchronize(mMainStageRegistry);

	auto cameraEntity = mMainStageRegistry.getFirstEntity<EditorCamera, render::Camera>();
	auto& camera = mMainStageRegistry.getComponent<render::Camera>(cameraEntity);
	mEditorView->setCamera(camera.getInstance());

	mCameraEditorScript.onUpdateFrame(delta);
}

void SceneEditorSystem::onDrawGui()
{
	// ImGui::ShowDemoWindow();
	auto dockSpace = ImGuiDockSpace{};

	ImGui::BeginMainMenuBar();
	ImGui::Text("Spatial Engine");
	ImGui::Separator();

	if (ImGui::BeginMenu("Options"))
	{
		// ImGui::MenuItem("Properties", NULL, openedPropertiesPtr);
		// ImGui::MenuItem("Console", NULL, &gOpenedLogging);
		ImGui::EndMenu();
	}
	ImGui::EndMainMenuBar();

	mImGuiSceneWindow.draw("Scene View");

	{
		auto window = ImGuiWindow{"Scene Graph"};
		editor::entitiesTreeView(mMainStageRegistry, mSelectedEntity);
	}

	{
		auto window = ImGuiWindow{"Properties"};
		if (mMainStageRegistry.isValid(mSelectedEntity))
		{
			{
				auto& node = mMainStageRegistry.getComponent<ecs::EntityName>(mSelectedEntity);
				editor::inputText("##Name", node.name);

				ImGui::SameLine();
				ImGui::PushItemWidth(-1);
			}

			ImGui::Spacing();
			ImGui::Spacing();

			ImGui::PopItemWidth();

			componentGroup<ecs::Transform>("Transform", mMainStageRegistry, mSelectedEntity);
			componentGroup<ecs::PerspectiveCamera>("Perspective Camera", mMainStageRegistry, mSelectedEntity);
			componentGroup<ecs::OrthographicCamera>("Orthographic Camera", mMainStageRegistry, mSelectedEntity);
			componentGroup<ecs::CustomCamera>("Custom Camera", mMainStageRegistry, mSelectedEntity);
			componentGroup<editor::EditorCamera>("Editor Camera", mMainStageRegistry, mSelectedEntity);
			componentGroup<ecs::DirectionalLight>("Directional Light", mMainStageRegistry, mSelectedEntity);
			componentGroup<ecs::SpotLight>("Spot Light", mMainStageRegistry, mSelectedEntity);
			componentGroup<ecs::PointLight>("Point Light", mMainStageRegistry, mSelectedEntity);
			componentGroup<ecs::Mesh>("Mesh", mMainStageRegistry, mSelectedEntity);
		}
		else
		{
			ImGui::Text("No entity selected.");
		}
	}

	{
		auto window = ImGuiWindow{"Materials"};
		static ecs::Entity selectedMaterialEntity{mMainStageRegistry.getFirstEntity<ecs::tags::IsMeshMaterial>()};

		ImGui::Text("Material");
		ImGui::SameLine();
		selectEntityInput<ecs::tags::IsMeshMaterial>("##Material", mMainStageRegistry, selectedMaterialEntity);

		ImGui::Separator();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();

		if (mMainStageRegistry.isValid(selectedMaterialEntity))
		{
			conditionalComponentInput<DefaultMaterial>(mMainStageRegistry, selectedMaterialEntity);
		}
		else
		{
			ImGui::Text("No material selected.");
		}
	}

	{
		auto window = ImGuiWindow{"Serialization Test"};
		auto ss = std::stringstream {};
		{
			auto xml = XMLOutputArchive{ ss };
			ecs::serialize<DefaultMaterial>(xml, mMainStageRegistry);
		}

		auto data = ss.str();
		ImGui::TextUnformatted(data.data());
	}
}

void SceneEditorSystem::onSceneWindowResized(const math::int2& size)
{
	auto width = static_cast<double>(size.x);
	auto height = static_cast<double>(size.y);
	mCameraEditorScript.onEditorViewResized(width / height);
}

void SceneEditorSystem::onRender(filament::Renderer& renderer) const
{
	renderer.render(mEditorView.get());
}

} // namespace spatial::editor
