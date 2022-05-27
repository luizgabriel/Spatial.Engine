#include "EditorCameraController.h"
#include "EditorCamera.h"
#include <spatial/ecs/Camera.h>

using namespace spatial::math;

namespace spatial::editor
{

void EditorCameraController::onUpdateEntity(float delta, ecs::Transform& transform, EditorCamera& camera) const noexcept
{
	const auto movementDelta = delta * camera.velocity * mControls.movementOffset;
	const auto forward = transform.getForwardVector();
	const auto right = cross(forward, math::axisY);

	transform.position += forward * movementDelta.x;
	transform.position += right * movementDelta.y;
	transform.position += math::axisY * movementDelta.z;

	const auto viewRotation = mControls.viewOffset * math::pi * camera.sensitivity * delta;
	transform.rotation = math::vec3{
		std::clamp(transform.rotation.x + viewRotation.y, -math::pi / 2.0f,
				   math::pi / 2.0f), // clamp the pitch
		transform.rotation.y + viewRotation.x,
		.0f // remove the roll
	};
}

void EditorCameraController::onUpdateInput(const desktop::InputState& input)
{
	mControls = {
		{
			input.axis(Key::W, Key::S),
			input.axis(Key::D, Key::A),
			input.axis(Key::Space, Key::LShift),
		},
		input.getMouseOffset(),
	};
}

} // namespace spatial::editor