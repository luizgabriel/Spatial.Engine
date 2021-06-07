#include "SceneEditorSystem.h"

#include <assets/generated.h>
#include <filament/LightManager.h>
#include <spatial/core/Logger.h>
#include <spatial/ecs/RegistryUtils.h>
#include <spatial/render/Camera.h>
#include <spatial/render/ResourceLoaders.h>
#include <spatial/render/SkyboxResources.h>
#include <spatial/render/Transform.h>

#include "DefaultMaterial.h"
#include "EditorCamera.h"
#include "ImGuiComponents.h"

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
		light.add(ecs::DirectionalLight{10000, math::float3{.34f, -.66f, -.67f}});
	}

	{
		auto m1 = ecs::createEntity(mMainStageRegistry);
		m1.add(DefaultMaterial{math::float3{.4f, 0.1f, 0.1f}});

		auto cube = ecs::createEntity(mMainStageRegistry, "Cube");
		cube.add(ecs::Transform{math::float3{.0f}});
		cube.add(ecs::MeshRenderer{true, true, {m1}, 1});
	}

	{
		auto m2 = ecs::createEntity(mMainStageRegistry);
		m2.add(DefaultMaterial{math::float3{.8f, .8f, .8f}});

		auto plane = ecs::createEntity(mMainStageRegistry, "Plane");
		plane.add(ecs::Transform{math::float3{3.0f, -1.0f, .0f}, math::float3{10.0f}});
		plane.add(ecs::MeshRenderer{false, true, {m2}, 1});
	}

	{
		auto m3 = ecs::createEntity(mMainStageRegistry);
		m3.add(DefaultMaterial{math::float3{.1f, 0.4f, 0.1f}});

		auto cylinder = ecs::createEntity(mMainStageRegistry, "Cylinder");
		cylinder.add(ecs::Transform{math::float3{6.0f, .0f, .0f}});
		cylinder.add(ecs::MeshRenderer{true, true, {m3}, 1});
	}

	{
		auto m4 = ecs::createEntity(mMainStageRegistry);
		m4.add(DefaultMaterial{math::float3{.1f, 0.1f, 0.4f}});

		auto sphere = ecs::createEntity(mMainStageRegistry, "Sphere");
		sphere.add(ecs::Transform{math::float3{3.0f, .0f, .0f}});
		sphere.add(ecs::MeshRenderer{true, true, {m4}, 1});
	}

	mMaterialInstancesSystem.synchronize<DefaultMaterial>(mMainStageRegistry, mDefaultMaterial.ref());

	onSceneWindowResized({1280, 720});
}

void SceneEditorSystem::onUpdateFrame(float delta)
{
	mCameraEditorScript.onUpdateFrame(delta);

	mSceneControllerSystem.synchronize(mMainStageRegistry);
	mMaterialInstancesSystem.synchronize<DefaultMaterial>(mMainStageRegistry, mDefaultMaterial.ref());
	mTransformSystem.synchronize(mMainStageRegistry);
	mCameraSystem.synchronize(mMainStageRegistry);
	mLightSystem.synchronize(mMainStageRegistry);
	mRenderableMeshSystem.synchronize(mMainStageRegistry);

	auto cameraEntity = mMainStageRegistry.getFirstEntity<EditorCamera, render::Camera>();
	auto& camera = mMainStageRegistry.getComponent<render::Camera>(cameraEntity);
	mEditorView->setCamera(camera.getInstance());
}

void SceneEditorSystem::onDrawGui()
{
	// ImGui::ShowDemoWindow();
	static ImGuiDockNodeFlags dockFlags = ImGuiDockNodeFlags_None;
	static ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoTitleBar
										  | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize
										  | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus
										  | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoBackground;

	ImGuiViewport* viewport = ImGui::GetMainViewport();

	ImGui::SetNextWindowPos(viewport->Pos);
	ImGui::SetNextWindowSize(viewport->Size);
	ImGui::SetNextWindowViewport(viewport->ID);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace", nullptr, windowFlags);
	ImGui::PopStyleVar();

	ImGui::PopStyleVar(2);

	// DockSpace
	static ImGuiID dockSpaceId = ImGui::GetID("SpatialDockSpace");
	ImGui::DockSpace(dockSpaceId, ImVec2(0.0f, 0.0f), dockFlags);

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

	ImGui::Begin("Scene Graph");

	editor::instancesTreeView(mMainStageRegistry, mSelectedEntity);

	ImGui::End(); // Scene Graph Window

	auto selectedEntity = handleOf(mMainStageRegistry, mSelectedEntity);

	ImGui::Begin("Properties");
	if (selectedEntity.isValid())
	{
		{
			auto& node = selectedEntity.get<ecs::SceneNode>();
			editor::inputText("##Name", node.name);

			ImGui::SameLine();
			ImGui::PushItemWidth(-1);
		}

		ImGui::Spacing();
		ImGui::Spacing();

		ImGui::PopItemWidth();

		componentGroup<ecs::Transform>("Transform", selectedEntity);
		componentGroup<ecs::PerspectiveCamera>("Perspective Camera", selectedEntity);
		componentGroup<ecs::OrthographicCamera>("Orthographic Camera", selectedEntity);
		componentGroup<ecs::CustomCamera>("Custom Camera", selectedEntity);
		componentGroup<editor::EditorCamera>("Editor Camera", selectedEntity);
		componentGroup<ecs::DirectionalLight>("Directional Light", selectedEntity);
		componentGroup<ecs::SpotLight>("Spot Light", selectedEntity);
		componentGroup<ecs::PointLight>("Point Light", selectedEntity);
		componentGroup<ecs::Mesh>("Mesh", selectedEntity);
	}
	else
	{
		ImGui::Text("No actor selected.");
	}
	ImGui::End(); // Properties Window

	ImGui::End(); // DockSpace
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
