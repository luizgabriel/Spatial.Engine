#include "EditorCameraController.h"
#include "EditorCamera.h"
#include "Tags.h"
#include <spatial/ecs/Camera.h>
#include <spatial/ecs/RegistryUtils.h>
#include <spatial/ui/components/Components.h>

using namespace spatial::math;

namespace spatial::editor
{

float3 getInputAxis(const desktop::InputState& input)
{
	return {
		input.axis(Key::W, Key::S),
		input.axis(Key::D, Key::A),
		input.axis(Key::Space, Key::LShift),
	};
}

void createCamera(ecs::Registry& registry)
{
	ecs::build(registry)
		.withName("Editor Camera")
		.with(EditorCamera{.5f, 10.0f})
		.with<tags::IsEditorEntity>()
		.asTransform()
		.withPosition({3.0f, 3.0f, 20.0f})
		.asPerspectiveCamera()
		.withFieldOfView(60.0)
		.withAspectRatio(19.0 / 6.0);
}

void EditorCameraController::onUpdateFrame(ecs::Registry& registry, float delta)
{
	if (!registry.hasAnyEntity<EditorCamera>())
		createCamera(registry);

	const auto axis = getInputAxis(mInputState);

	registry.getEntities<ecs::Transform, EditorCamera>().each([&](auto& transform, auto& options) {
		const auto keyboardDelta = delta * options.velocity * axis;
		const auto forward = math::forwardVector(transform.getMatrix());
		const auto right = cross(forward, math::axisY);

		transform.position += forward * keyboardDelta.x;
		transform.position += right * keyboardDelta.y;
		transform.position += math::axisY * keyboardDelta.z;

		if (mInputState.released(Key::Escape))
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
				std::clamp(transform.rotation.x + mouseRotation.y, -math::pi / 2.0f,
						   math::pi / 2.0f), // clamp the pitch
				transform.rotation.y + mouseRotation.x,
				.0f // remove the roll
			};
		}
	});
}

EditorCameraController::EditorCameraController(const desktop::Window& window, const desktop::InputState& inputState)
	: mWindow{window}, mInputState{inputState}
{
}

void EditorCameraController::onEditorViewResized(ecs::Registry& registry, double aspectRatio)
{
	registry.getEntities<ecs::PerspectiveCamera, EditorCamera>().each(
		[=](auto& camera, auto&) { camera.aspectRatio = aspectRatio; });

	registry.getEntities<ecs::OrthographicCamera, EditorCamera>().each([=](auto& camera, auto&) {
		camera.left = -aspectRatio;
		camera.right = aspectRatio;
	});
}

void EditorCameraController::toggleControl(ecs::Registry& registry)
{
	registry.getEntities<EditorCamera>().each([](auto& options) {
		if (options.enabled)
		{
			options.justStarted = 0;
			options.enabled = false;
		}
		else
		{
			options.startPressed = true;
		}
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