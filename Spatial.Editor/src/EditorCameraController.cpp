#include "EditorCameraController.h"
#include "EditorCamera.h"
#include "Tags.h"
#include <spatial/ecs/Camera.h>
#include <spatial/ecs/RegistryUtils.h>

using namespace spatial::math;

namespace spatial::editor
{

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

void EditorCameraController::beforeUpdateEntities(ecs::Registry& registry)
{
	if (!registry.hasAnyEntity<EditorCamera>())
		createCamera(registry);

	if (mEnabled && mJustStarted < 11)
		mJustStarted++;
}

void EditorCameraController::onUpdateEntity(float delta, ecs::Transform& transform, EditorCamera& camera) const noexcept
{
	if (!hasEnabledControls())
		return;

	const auto movementDelta = delta * camera.velocity * mControls.movementOffset;
	const auto forward = math::forwardVector(transform.getMatrix());
	const auto right = cross(forward, math::axisY);

	transform.position += forward * movementDelta.x;
	transform.position += right * movementDelta.y;
	transform.position += math::axisY * movementDelta.z;

	const auto viewRotation = mControls.viewOffset * math::pi * camera.sensitivity * delta;
	transform.rotation = math::float3{
		std::clamp(transform.rotation.x + viewRotation.y, -math::pi / 2.0f,
				   math::pi / 2.0f), // clamp the pitch
		transform.rotation.y + viewRotation.x,
		.0f // remove the roll
	};
}

bool EditorCameraController::toggleControl() noexcept
{
	mEnabled = !mEnabled;
	if (mEnabled)
		mJustStarted = 0;

	return mEnabled;
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

void EditorCameraController::disable() noexcept
{
	mEnabled = false;
}

} // namespace spatial::editor