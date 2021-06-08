#include "SceneEditorSystem.h"

#include <assets/generated.h>
#include <filament/LightManager.h>
#include <spatial/core/Logger.h>
#include <spatial/ecs/RegistryUtils.h>
#include <spatial/ecs/Tags.h>
#include <spatial/render/Camera.h>
#include <spatial/render/ResourceLoaders.h>
#include <spatial/render/SkyboxResources.h>
#include <spatial/render/Transform.h>

#include "DefaultMaterial.h"
#include "EditorCamera.h"
#include "ImGuiComponents.h"
#include "ImGuiDockSpace.h"
#include "ImGuiWindow.h"

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

	  mMeshes{render::createFilamesh(mEngine, {ASSETS_CUBE_FILAMESH, ASSETS_CUBE_FILAMESH_SIZE}),
			  render::createFilamesh(mEngine, {ASSETS_PLANE_FILAMESH, ASSETS_PLANE_FILAMESH_SIZE}),
			  render::createFilamesh(mEngine, {ASSETS_CYLINDER_FILAMESH, ASSETS_CYLINDER_FILAMESH_SIZE}),
			  render::createFilamesh(mEngine, {ASSETS_SPHERE_FILAMESH, ASSETS_SPHERE_FILAMESH_SIZE})},

	  mCameraEditorScript{mMainStageRegistry, mWindow, mInputState}
{
	mImGuiSceneWindow >> *this; // register imgui window resize events
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

	{
		auto light = ecs::createEntity(mMainStageRegistry, "Main Light");
		light.tag<ecs::tags::IsRenderable>();
		light.add(ecs::DirectionalLight{10000, math::float3{.34f, -.66f, -.67f}});
	}

	{
		auto m1 = ecs::createEntity(mMainStageRegistry, "Red Material");
		m1.tag<ecs::tags::IsMeshMaterial>();
		m1.add(DefaultMaterial{math::float3{.4f, 0.1f, 0.1f}});

		auto cube = ecs::createEntity(mMainStageRegistry, "Cube");
		cube.tag<ecs::tags::IsRenderable>();
		cube.add(ecs::MeshResource{"editor://meshes/cube"});
		cube.add(ecs::Transform{math::float3{.0f}});
		cube.add(ecs::MeshRenderer{true, true, {m1}, 1});
	}

	{
		auto m2 = ecs::createEntity(mMainStageRegistry, "White Material");
		m2.tag<ecs::tags::IsMeshMaterial>();
		m2.add(DefaultMaterial{math::float3{.8f, .8f, .8f}});

		auto plane = ecs::createEntity(mMainStageRegistry, "Plane");
		plane.tag<ecs::tags::IsRenderable>();
		plane.add(ecs::MeshResource{"editor://meshes/plane"});
		plane.add(ecs::Transform{math::float3{3.0f, -1.0f, .0f}, math::float3{10.0f}});
		plane.add(ecs::MeshRenderer{false, true, {m2}, 1});
	}

	{
		auto m3 = ecs::createEntity(mMainStageRegistry, "Green Material");
		m3.tag<ecs::tags::IsMeshMaterial>();
		m3.add(DefaultMaterial{math::float3{.1f, 0.4f, 0.1f}});

		auto cylinder = ecs::createEntity(mMainStageRegistry, "Cylinder");
		cylinder.tag<ecs::tags::IsRenderable>();
		cylinder.add(ecs::MeshResource{"editor://meshes/cylinder"});
		cylinder.add(ecs::Transform{math::float3{6.0f, .0f, .0f}});
		cylinder.add(ecs::MeshRenderer{true, true, {m3}, 1});
	}

	{
		auto m4 = ecs::createEntity(mMainStageRegistry, "Blue Material");
		m4.tag<ecs::tags::IsMeshMaterial>();
		m4.add(DefaultMaterial{math::float3{.1f, 0.1f, 0.4f}});

		auto sphere = ecs::createEntity(mMainStageRegistry, "Sphere");
		sphere.tag<ecs::tags::IsRenderable>();
		sphere.add(ecs::MeshResource{"editor://meshes/sphere"});
		sphere.add(ecs::Transform{math::float3{3.0f, .0f, .0f}});
		sphere.add(ecs::MeshRenderer{true, true, {m4}, 1});
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
			componentGroup<ecs::MeshRenderer>("Mesh Renderer", mMainStageRegistry, mSelectedEntity);
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
