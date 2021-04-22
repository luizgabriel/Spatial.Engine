#include "EditorCameraScript.h"
#include "Components.h"
#include "ImGuiComponents.h"
#include <spatial/render/InstanceBuilder.h>

using namespace spatial::math;

namespace spatial::editor
{

void EditorCameraScript::onStart()
{
	mCamera = createInstance(mStage, "Main Camera")
				  .withPosition({3.0f, 3.0f, 20.0f})
				  .asCamera()
				  .withPerspectiveProjection(45.0, 19 / 6.0, .1, 1000.0)
				  .add<EditorCamera>(.5f, 10.0f);
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
	auto& transform = mCamera.get<Transform>();
	auto& options = mCamera.get<EditorCamera>();

	const auto keyboardDelta = delta * options.velocity * getInputAxis();
	const auto forward = transform.getForward();
	const auto right = cross(forward, math::axisY);
	auto position = transform.getPosition();

	position += forward * keyboardDelta.x;
	position += right * keyboardDelta.y;
	position += math::axisY * keyboardDelta.z;
	transform.setPosition(position);

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
		const auto rotation = transform.getRotation();

		transform.setRotation({
			std::clamp(rotation.x + mouseRotation.y, -math::pi, math::pi), // clamp the pitch
			rotation.y + mouseRotation.x,
			.0f // remove the roll
		});
	}
}

template <>
void drawComponentView(InstanceHandle&, EditorCamera& component)
{
	ImGui::DragFloat("Velocity", &component.velocity);
	ImGui::DragFloat("Sensitivity", &component.sensitivity);
	component.startPressed = ImGui::Button("Free Camera Control");
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::Text("Left Ctrl + Left Mouse Click");
		ImGui::EndTooltip();
	}
}

EditorCameraScript::EditorCameraScript(Stage& stage, desktop::Window& window, const desktop::InputState& inputState)
	: mStage{stage}, mWindow{window}, mInputState{inputState}, mCamera{mStage, NullInstance}
{
}

} // namespace spatial::editor