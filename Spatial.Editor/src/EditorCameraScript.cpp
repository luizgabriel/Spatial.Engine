#include "EditorCameraScript.h"
#include "EditorCamera.h"
#include "Tags.h"
#include <spatial/ecs/Camera.h>
#include <spatial/ecs/RegistryUtils.h>
#include <spatial/ui/components/ImGuiComponents.h>

using namespace spatial::math;

namespace spatial::editor
{

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
	int count = 0;

	mRegistry.getEntities<ecs::Transform, EditorCamera>().each([&count, delta, this](auto& transform, auto& options) {
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
			options.startPressed = false;
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

		count++;
	});

	if (count == 0)
		createCamera();
}

void EditorCameraScript::createCamera()
{
	ecs::build(mRegistry)
		.withName("Editor Camera")
		.with(EditorCamera{.5f, 10.0f})
		.with<tags::IsEditorEntity>()
		.asTransform()
		.withPosition({3.0f, 3.0f, 20.0f})
		.asPerspectiveCamera()
		.withFieldOfView(60.0)
		.withAspectRatio(19.0 / 6.0);
}

EditorCameraScript::EditorCameraScript(ecs::Registry& stage, const desktop::Window& window,
									   const desktop::InputState& inputState)
	: mRegistry{stage}, mWindow{window}, mInputState{inputState}
{
}

void EditorCameraScript::onEditorViewResized(double aspectRatio)
{
	mRegistry.getEntities<ecs::PerspectiveCamera, EditorCamera>().each(
		[=](auto& camera, auto&) { camera.aspectRatio = aspectRatio; });

	mRegistry.getEntities<ecs::OrthographicCamera, EditorCamera>().each([=](auto& camera, auto&) {
		camera.left = -aspectRatio;
		camera.right = aspectRatio;
	});
}

} // namespace spatial::editor

namespace spatial::ui
{

template <>
void componentInput<editor::EditorCamera>(ecs::Registry& registry, ecs::Entity entity)
{
	auto& data = registry.getComponent<editor::EditorCamera>(entity);

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

} // namespace spatial::ui