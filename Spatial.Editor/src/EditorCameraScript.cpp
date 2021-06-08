#include "EditorCameraScript.h"
#include "EditorCamera.h"
#include "ImGuiComponents.h"
#include <spatial/ecs/Camera.h>
#include <spatial/ecs/RegistryUtils.h>
#include <spatial/ecs/Tags.h>

using namespace spatial::math;

namespace spatial::editor
{

void EditorCameraScript::onStart()
{
	auto camera = ecs::createEntity(mRegistry, "Main Camera");
	camera.tag<ecs::tags::IsRenderable>();
	camera.add(ecs::Transform{math::float3 {3.0f, 3.0f, 20.0f}});
	camera.add(ecs::PerspectiveCamera{45.0, 19 / 6.0, .1, 1000.0});
	camera.add(EditorCamera{.5f, 10.0f});
	mCamera = camera;
}

float3 EditorCameraScript::getInputAxis()
{
	return {
		mInputState.axis(Key::W, Key::S),
		mInputState.axis(Key::D, Key::A),
		mInputState.axis(Key::Space, Key::LShift),
	};
}

void EditorCameraScript::onUpdateFrame(float delta)
{
	if (mRegistry.isValid(mCamera))
		return;

	auto& transform = mRegistry.getComponent<ecs::Transform>(mCamera);
	auto& options = mRegistry.getComponent<EditorCamera>(mCamera);

	const auto keyboardDelta = delta * options.velocity * getInputAxis();
	const auto forward = math::forwardVector(transform.getMatrix());
	const auto right = cross(forward, math::axisY);

	transform.position += forward * keyboardDelta.x;
	transform.position += right * keyboardDelta.y;
	transform.position += math::axisY * keyboardDelta.z;

	if (mInputState.released(Key::MouseLeft))
	{
		options.enabled = false;
		options.justStarted = 0;
	}

	if (mInputState.combined(Key::LControl, Key::MouseLeft) || options.startPressed)
	{
		mWindow.warpMouse(mWindow.getSize() * .5f);
		options.enabled = true;
		options.justStarted = 1;
	}

	if (options.enabled && options.justStarted <= 10)
		options.justStarted++;

	if (options.enabled && options.justStarted >= 10)
	{
		const auto mouseRotation = mInputState.getMouseOffset() * math::pi * options.sensitivity * delta;
		transform.rotation = math::float3{
			std::clamp(transform.rotation.x + mouseRotation.y, -math::pi, math::pi), // clamp the pitch
			transform.rotation.y + mouseRotation.x,
			.0f // remove the roll
		};
	}
}

template <>
void componentInput<EditorCamera>(ecs::Registry& registry, ecs::Entity entity)
{
	auto& data = registry.getComponent<EditorCamera>(entity);

	ImGui::DragFloat("Velocity", &data.velocity);
	ImGui::DragFloat("Sensitivity", &data.sensitivity);

	data.startPressed = ImGui::Button("Free Camera Control");
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::Text("Left Ctrl + Left Mouse Click");
		ImGui::EndTooltip();
	}
}

EditorCameraScript::EditorCameraScript(ecs::Registry& stage, const desktop::Window& window, const desktop::InputState& inputState)
	: mRegistry{stage}, mWindow{window}, mInputState{inputState}, mCamera{ecs::NullEntity}
{
}

void EditorCameraScript::onEditorViewResized(double aspectRatio)
{
	if (mRegistry.hasAllComponents<ecs::PerspectiveCamera>(mCamera))
	{
		auto& camera = mRegistry.getComponent<ecs::PerspectiveCamera>(mCamera);
		camera.aspectRatio = aspectRatio;
	}

	if (mRegistry.hasAllComponents<ecs::OrthographicCamera>(mCamera))
	{
		auto& camera = mRegistry.getComponent<ecs::OrthographicCamera>(mCamera);
		camera.left = -aspectRatio;
		camera.right = aspectRatio;
	}
}

} // namespace spatial::editor