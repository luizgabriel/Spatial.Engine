#include "EditorCameraController.h"
#include "Components.h"
#include <spatial/ecs/Camera.h>
#include <spatial/ecs/SceneView.h>

namespace spatial::editor {

void EditorCameraController::readCameraInputs(ecs::Registry& registry, const desktop::InputState& state)
{
	registry.getEntities<editor::EditorCamera>().each([&](auto& camera) {
		camera.viewOffset = state.getMouseOffset();
		camera.movementOffset = math::vec3{
			state.axis(Key::W, Key::S),
			state.axis(Key::D, Key::A),
			state.axis(Key::Space, Key::LShift),
		};
	});
}

void EditorCameraController::updateCameraTransforms(ecs::Registry& registry, float delta)
{
	registry.getEntities<editor::EditorCamera, ecs::Transform>().each([delta](auto& camera, auto& transform) {
		const auto movementDelta = delta * camera.velocity * camera.movementOffset;
		const auto forward = transform.getForwardVector();
		const auto right = cross(forward, math::axisY);

		transform.position += forward * movementDelta.x;
		transform.position += right * movementDelta.y;
		transform.position += math::axisY * movementDelta.z;

		const auto viewRotation = camera.viewOffset * math::pi * camera.sensitivity * delta;
		transform.rotation = math::vec3{
			std::clamp(transform.rotation.x + viewRotation.y, -math::halfPi,
					   math::halfPi), // clamp the pitch
			transform.rotation.y + viewRotation.x,
			.0f // remove the roll
		};
	});
}

void EditorCamera::replaceCamera(ecs::Registry& registry, ecs::Entity newControlledEntity)
{
	registry.removeComponentFromEntities<editor::EditorCamera>();
	if (registry.isValid(newControlledEntity))
		registry.addComponent<editor::EditorCamera>(newControlledEntity);
}

void EditorCamera::replaceView(ecs::Registry& registry, ecs::Entity newControlledView)
{
	if (!registry.hasAllComponents<ecs::SceneView>(newControlledView))
		return;

	auto& sceneView = registry.getComponent<ecs::SceneView>(newControlledView);
	replaceCamera(registry, sceneView.camera);
}

} // namespace spatial::editor