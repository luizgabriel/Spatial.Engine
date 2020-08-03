#include "EditorSystem.h"

#include <spatial/spatial.h>
#include <spatial/ecs/RegistryUtils.h>

#include <imgui.h>

#include "Components.h"

namespace fl = filament;
using namespace filament::math;

namespace spatial
{

EditorSystem::EditorSystem(fl::Engine& engine, const assets::ResourcesLoader& resources)
	: mResources{resources},
	  mEngine{engine},
	  mSceneView{toShared(createView(mEngine))},

	  mRenderColorTexture{
		  createTexture(mEngine, {1280, 720}, filament::backend::TextureFormat::RGBA16F,
						filament::Texture::Usage::COLOR_ATTACHMENT | filament::Texture::Usage::SAMPLEABLE)},
	  mRenderDepthTexture{createTexture(mEngine, {1280, 720}, filament::backend::TextureFormat::DEPTH16,
										filament::Texture::Usage::DEPTH_ATTACHMENT)},
	  mRenderTarget{createRenderTarget(mEngine, mRenderColorTexture.ref(), mRenderDepthTexture.ref())},

	  mScene{createScene(mEngine)},
	  mCameraEntity{createEntity(mEngine)},
	  mCameraComponent{createCamera(mEngine, mCameraEntity.get())},

	  mIblTexture{createKtxTexture(mEngine, mResources("editor/textures/default_skybox/ibl.ktx").value())},
	  mSkyboxTexture{createKtxTexture(mEngine, mResources("editor/textures/default_skybox/skybox.ktx").value())},
	  mSkyboxLight{createImageBasedLight(mEngine, mIblTexture.ref(),
										 mResources("editor/textures/default_skybox/sh.txt").value())},
	  mSkybox{createSkybox(mEngine, mSkyboxTexture.ref())},

	  mCam{{.0f, .0f}, {300.0f, 300.0f, 300.0f}},
	  mCameraData{.5f, 100.0f},

	  mRegistry{},
	  mRenderableSystem{mEngine, mScene.ref()},
	  mTransformSystem{mEngine},
	  mShapeSystem{mEngine, mResources}
{
	mShapeSystem.setMaterial("editor/materials/default.filamat");
	ecs::connect<ecs::Shape>(mRegistry, mShapeSystem);

	mSceneView->setRenderTarget(mRenderTarget.get());
	mSceneView->setCamera(mCameraComponent.get());
	mSceneView->setScene(mScene.get());
	mSceneView->setBlendMode(fl::View::BlendMode::OPAQUE);

	mScene->setIndirectLight(mSkyboxLight.get());
	mScene->setSkybox(mSkybox.get());

	auto entity = mRegistry.create();
	mRegistry.emplace<ecs::Transform>(entity);
	mRegistry.emplace<ecs::Shape>(entity, "editor/meshes/cube.filamesh");

	mSceneView->setViewport({0, 0, 1280, 720});
	refreshMainViewSize({1280, 720});
}

void EditorSystem::onEvent(const MouseMovedEvent&)
{
	if (enabledCameraController)
	{
		mCam.onMouseMoved(Input::mouse(), mCameraData.sensitivity);
		Input::warpMouse({.5f, .5f});
	}
}

float3 defaultInputAxis()
{
	return {
		Input::axis(Key::W, Key::S),
		Input::axis(Key::D, Key::A),
		Input::axis(Key::Space, Key::LShift),
	};
}

void EditorSystem::onUpdateFrame(float delta)
{
	if (Input::released(Key::MouseLeft))
		enabledCameraController = false;

	if (Input::combined(Key::LControl, Key::MouseLeft))
		enabledCameraController = true;

	if (Input::released(Key::G))
		showEngineGui = !showEngineGui;

	mTransformSystem.onUpdate(mRegistry);

	if (!enabledCameraController)
		delta = 0;

	mCam.onUpdate(mCameraComponent.ref(), delta * mCameraData.velocity * defaultInputAxis());
}

void EditorSystem::onDrawGui()
{
	if (!showEngineGui)
		return;

	static ImGuiDockNodeFlags dockFlags = ImGuiDockNodeFlags_None;
	static ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoBackground;

	ImGuiViewport *viewport = ImGui::GetMainViewport();

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
	ImGuiID dockSpaceId = ImGui::GetID("SpatialDockSpace");
	ImGui::DockSpace(dockSpaceId, ImVec2(0.0f, 0.0f), dockFlags);

	ImGui::BeginMainMenuBar();
		ImGui::Text("Spatial Engine");
		ImGui::Separator();

		if (ImGui::BeginMenu("Options"))
		{
			//ImGui::MenuItem("Properties", NULL, openedPropertiesPtr);
			//ImGui::MenuItem("Console", NULL, &gOpenedLogging);
			ImGui::EndMenu();
		}
	ImGui::EndMainMenuBar();

	ImGui::Begin("Camera Settings");
		if (enabledCameraController)
		{
			ImGui::Text("Use the WASD to move in the scene.");
			ImGui::Text("Click to turn OFF:");
		}
		else
		{
			ImGui::Text("Ctrl + Click to turn ON:");
		}

		if (ImGui::Checkbox("Camera Movement", &enabledCameraController))
		{
			Input::warpMouse({.5f, .5f});
		}

		ImGui::Separator();

		ImGui::DragFloat("Yaw", &mCam.rotation.x, .001f);
		ImGui::DragFloat("Pitch", &mCam.rotation.y, .001f, -halfPi<float>, halfPi<float>);

		ImGui::Separator();

		ImGui::DragFloat("Sensitivity", &mCameraData.sensitivity, .001f, .1f, 5.0f);
		ImGui::DragFloat("Velocity", &mCameraData.velocity, 1.0f, 100.0f, 2000.0f);
	ImGui::End();

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::Begin("Game (Edit Mode)");
	auto size = ImGui::GetWindowSize();
	size.y -= 25;
	refreshMainViewSize(size);
	ImGui::Image(mRenderColorTexture.get(), size);
	ImGui::End();
	ImGui::PopStyleVar();
}

void EditorSystem::refreshMainViewSize(const ImVec2& size)
{
	auto width = static_cast<double>(size.x);
	auto height = static_cast<double>(size.y);

	mCameraComponent->setProjection(45.0, width / height, 0.1, 1000000.0, fl::Camera::Fov::VERTICAL);
}

EditorSystem::EditorSystem(RenderingSystem& renderingSystem, const assets::ResourcesLoader& resources)
	: EditorSystem(renderingSystem.getEngine(), resources)
{
	renderingSystem.pushBackView(mSceneView);
}

} // namespace spatial
