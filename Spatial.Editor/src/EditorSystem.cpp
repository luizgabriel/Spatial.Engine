#include "EditorSystem.h"

#include <spatial/input/Input.h>

#include <imgui.h>

#include <filament/LightManager.h>
#include <filament/TextureSampler.h>

#include <spatial/ecs/Components.h>
#include <spatial/ecs/RegistryUtils.h>
#include <spatial/render/ResourceLoaders.h>
#include <spatial/render/SkyboxResources.h>

namespace fl = filament;
using namespace filament::math;

namespace spatial
{

EditorSystem::EditorSystem(RenderingSystem& renderingSystem)
	: mEngine{renderingSystem.getEngine()},
	  mMainView{renderingSystem.getMainView()},

	  mScene{createScene(mEngine)},
	  mCameraEntity{createEntity(mEngine)},
	  mCameraComponent{createCamera(mEngine, mCameraEntity.get())},

	  mCam{{3.89263f, -0.413847}, {300.0f, 300.0f, 300.0f}},
	  mCameraData{.5f, 500.0f},

	  mRegistry{},
	  mRenderableSystem{mEngine, mScene.ref()},
	  mTransformSystem{mEngine}
{
	mMainView.setScene(mScene.get());

	auto entity = mRegistry.create();
	mRegistry.emplace<ecs::Transform>(entity);
}

void EditorSystem::onEvent(const MouseMovedEvent& e)
{
	if (enabledCameraController)
	{
		mCam.onMouseMoved({e.x, e.y}, mCameraData.sensitivity);
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
}

} // namespace spatial
